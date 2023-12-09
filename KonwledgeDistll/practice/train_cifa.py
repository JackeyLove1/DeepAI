import os

import torch
import torch.nn as nn
import torch.optim as optim
import torchvision.transforms as transforms
import torchvision.datasets as datasets
from transformers import Trainer, set_seed
import torch.nn.functional as F
import torchvision
from torchvision import models
from accelerate import Accelerator
import timm

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
set_seed(42)
print("Use device:", device)

batch_size = 256 if torch.cuda.is_available() else 4
learning_rate = 1e-4
num_epochs = 10 if torch.cuda.is_available() else 1
num_workers = 16 if torch.cuda.is_available() else 1
num_classes = 10
alpha = 0.9
T = 2.0

accelerator = Accelerator()
transform = transforms.Compose([
    # transforms.Resize(224), # used for vit model
    transforms.ToTensor(),
    transforms.Normalize((0.5,), (0.5,)),
])

train_dataset = torchvision.datasets.CIFAR10(root='./data', train=True, download=False, transform=transform)
train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=batch_size, shuffle=True, num_workers=num_workers)

test_dataset = torchvision.datasets.CIFAR10(root='./data', train=False, download=False, transform=transform)
test_loader = torch.utils.data.DataLoader(test_dataset, batch_size=batch_size, shuffle=False, num_workers=num_workers)

class SimpleNet(nn.Module):
    def __init__(self):
        super(SimpleNet, self).__init__()
        self.fc = nn.Linear(32 * 32 * 3, 10)

    def forward(self, x):
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x

class LightNN(nn.Module):
    def __init__(self, num_classes=10):
        super(LightNN, self).__init__()
        self.features = nn.Sequential(
            nn.Conv2d(3, 16, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),
            nn.Conv2d(16, 16, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2),
        )
        self.classifier = nn.Sequential(
            nn.Linear(1024, 256),
            nn.ReLU(),
            nn.Dropout(0.1),
            nn.Linear(256, num_classes)
        )

    def forward(self, x):
        x = self.features(x)
        x = torch.flatten(x, 1)
        x = self.classifier(x)
        return x

# make student model
simple_model = LightNN()

res18_model = models.resnet18(pretrained=True)
res18_model.fc = nn.Linear(res18_model.fc.in_features, num_classes)

resnet_model = models.resnet50(pretrained=True)
resnet_model.fc = nn.Linear(resnet_model.fc.in_features, num_classes)

vit_model = timm.create_model('vit_base_patch16_224', pretrained=True)
vit_model.head = nn.Linear(vit_model.head.in_features, num_classes)


def distillation_loss(y, labels, teacher_scores, T, alpha):
    return nn.KLDivLoss()(F.log_softmax(y / T), F.softmax(teacher_scores / T)) * (
            T * T * 2.0 * alpha) + F.cross_entropy(y, labels) * (1. - alpha)


# TODO: use transformer trainer
def train(model):
    model = model.to(device)
    model.train()
    optimizer = optim.AdamW(model.parameters(), lr=learning_rate)
    loss_fn = nn.CrossEntropyLoss()
    global train_loader, test_loader
    model, optimizer, ds_train_loader, ds_test_loader = accelerator.prepare(
        model, optimizer, train_loader, test_loader
    )
    for epoch in range(num_epochs):
        for batch_idx, (data, target) in enumerate(ds_train_loader):
            data, target = data.to(device), target.to(device)
            optimizer.zero_grad()
            output = model(data)
            loss = loss_fn(output, target)
            accelerator.backward(loss)
            optimizer.step()
            if batch_idx % 500 == 0:
                print(
                    f"\nTrain Epoch: {epoch} [{batch_idx * len(data)}/{len(train_loader.dataset)} ({100. * batch_idx / len(train_loader):.0f}%)]\tLoss: {loss.item():.6f}")


def eval(model):
    correct, total = 0, 0
    model = model.to(device)
    model.eval()
    for images, labels in test_loader:
        images, labels = images.to(device), labels.to(device)
        outputs = model(images)
        _, predicted = torch.max(outputs.data, 1)
        total += labels.size(0)
        correct += (predicted == labels).sum().item()
    print('Test Accuracy of the model on the test images: {}%'.format(100 * correct / total))


def save_model(model: nn.Module, name: str):
    path = f"./models/{name}.pth"
    torch.save(model.state_dict(), path)


def load_model(model: nn.Module, name: str):
    path = f"./models/{name}.pth"
    if not os.path.exists(path):
        raise ValueError(f"model {name} not found")
    model.load_state_dict(torch.load(path))


def train_resnet():
    train(resnet_model)
    eval(resnet_model)


def train_vit():
    train(vit_model)
    eval(vit_model)


def train_simple():
    train(simple_model)
    eval(simple_model)

def distill_cross_entropy():
    train(res18_model)
    eval(res18_model)
    teacher = res18_model
    teacher.eval()
    student = LightNN().to(device)
    student.train()
    optimizer = optim.AdamW(student.parameters(), lr=learning_rate)
    ce_loss = nn.CrossEntropyLoss()
    soft_target_loss_weight = 0.25
    ce_loss_weight = 0.75
    for epoch in range(100):
        for batch_idx, (data, labels) in enumerate(train_loader):
            data, labels = data.to(device), labels.to(device)
            with torch.no_grad():
                teacher_logits = teacher(data)
            optimizer.zero_grad()
            student_logits = student(data)
            soft_targets = nn.functional.softmax(teacher_logits / T, dim=-1)
            soft_prob = nn.functional.log_softmax(student_logits / T, dim=-1)
            soft_targets_loss = -torch.sum(soft_targets * soft_prob) / soft_prob.size()[0] * (T ** 2)
            label_loss = ce_loss(student_logits, labels)
            loss = soft_target_loss_weight * soft_targets_loss + ce_loss_weight * label_loss
            loss.backward()
            optimizer.step()
            if batch_idx % 500 == 0:
                print(
                    f"\nTrain Epoch: {epoch} [{batch_idx * len(data)}/{len(train_loader.dataset)} ({100. * batch_idx / len(train_loader):.0f}%)]\tLoss: {loss.item():.6f}")
    eval(student)



def distill_kld():
    train(res18_model)
    eval(res18_model)
    teacher = res18_model
    teacher.eval()
    student = SimpleNet().to(device)
    student.train()
    optimizer = optim.AdamW(student.parameters(), lr=learning_rate)
    ce_loss = nn.CrossEntropyLoss()
    kld_loss = nn.KLDivLoss("none")
    kl_loss_weight = 1.0
    ce_loss_weight = 0.8
    for epoch in range(20):
        for batch_idx, (data, labels) in enumerate(train_loader):
            data, labels = data.to(device), labels.to(device)
            with torch.no_grad():
                teacher_logits = teacher(data)
            optimizer.zero_grad()
            student_logits = student(data)
            teacher_distribution = nn.functional.softmax(teacher_logits / T, dim=-1)
            student_distribution = nn.functional.log_softmax(student_logits / T, dim=-1)
            # soft_targets_loss = kld_loss(student_distribution, teacher_distribution).mean() * (T ** 2)
            soft_targets_loss = kld_loss(student_distribution, teacher_distribution).sum() * (T ** 2)
            label_loss = ce_loss(student_logits, labels)
            loss = kl_loss_weight * soft_targets_loss + ce_loss_weight * label_loss
            loss.backward()
            optimizer.step()
            if batch_idx % 500 == 0:
                print(
                    f"\nTrain Epoch: {epoch} [{batch_idx * len(data)}/{len(train_loader.dataset)} ({100. * batch_idx / len(train_loader):.0f}%)]\tLoss: {loss.item():.6f}")
    eval(student)


# simple model: 40.19%
# resnet50: 83.25%
# vit: 97.49%
# student: 81.51%
if __name__ == "__main__":
    train_simple()
    # distill_cross_entropy()
    distill_kld()