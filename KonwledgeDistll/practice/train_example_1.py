import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision
import torchvision.transforms as transforms

# Parameters
batch_size = 100
learning_rate = 0.001
num_epochs = 5
alpha = 0.9  # Weight for distillation loss
T = 2.0  # Temperature parameter

# CIFAR10 Dataset
transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])
train_dataset = torchvision.datasets.CIFAR10(root='./data', train=True, download=True, transform=transform)
train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=batch_size, shuffle=True, num_workers=2)

test_dataset = torchvision.datasets.CIFAR10(root='./data', train=False, download=True, transform=transform)
test_loader = torch.utils.data.DataLoader(test_dataset, batch_size=batch_size, shuffle=False)


# Define teacher and student network
class SimpleNet(nn.Module):
    def __init__(self):
        super(SimpleNet, self).__init__()
        self.fc = nn.Linear(32 * 32 * 3, 10)

    def forward(self, x):
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x


# Train Teacher
teacher = SimpleNet()
optim_teacher = torch.optim.Adam(teacher.parameters(), lr=learning_rate)

for epoch in range(num_epochs):
    for i, (images, labels) in enumerate(train_loader):
        images = images.view(batch_size, -1)

        # Forward pass
        outputs = teacher(images)
        loss = F.cross_entropy(outputs, labels)

        # Backward and optimize
        optim_teacher.zero_grad()
        loss.backward()
        optim_teacher.step()

# Distill knowledge to student
student = SimpleNet()
optim_student = torch.optim.Adam(student.parameters(), lr=learning_rate)

def distillation_loss(y, labels, teacher_scores, T, alpha):
    return nn.KLDivLoss()(F.log_softmax(y/T), F.softmax(teacher_scores/T)) * (T*T * 2.0 * alpha) + F.cross_entropy(y, labels) * (1. - alpha)

for epoch in range(num_epochs):
    for i, (images, labels) in enumerate(train_loader):
        images = images.view(batch_size, -1)
        teacher_outputs = teacher(images)

        # Forward pass
        outputs = student(images)
        loss = distillation_loss(outputs, labels, teacher_outputs, T, alpha)

        # Backward and optimize
        optim_student.zero_grad()
        loss.backward()
        optim_student.step()

# Test student
correct = 0
total = 0
for images, labels in test_loader:
    images = images.view(batch_size, -1)
    outputs = student(images)
    _, predicted = torch.max(outputs.data, 1)
    total += labels.size(0)
    correct += (predicted == labels).sum().item()

print('Test Accuracy of the student on the 10000 test images: {} %'.format(100 * correct / total))

import torch
import torch.nn.functional as F


def distill(teacher: nn.Module, student: nn.Module, dataloader, optimizer, T=2):
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    loss_func = nn.KLDivLoss()

    teacher.eval()
    student.train()

    total_loss = 0.0
    for (data, target) in dataloader:
        data = data.to(device)
        target = target.to(device)

        # Get the softer output from teacher model
        with torch.no_grad():
            teacher_output = teacher(data)

        teacher_output = F.log_softmax(teacher_output / T, dim=1)

        # Get the raw output from the student model
        student_output = student(data)
        loss = loss_func(F.log_softmax(student_output / T, dim=1), teacher_output) * (T * T)

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        total_loss += loss.item()

    print(f'Distillation Loss: {total_loss / len(dataloader)}')