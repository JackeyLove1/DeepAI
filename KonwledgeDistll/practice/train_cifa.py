import torch
import torch.nn as nn
import torch.optim as optim
import torchvision.transforms as transforms
import torchvision.datasets as datasets
from transformers import Trainer, set_seed
import torch.nn.functional as F
import torchvision


def distillation_loss(y, labels, teacher_scores, T, alpha):
    return nn.KLDivLoss()(F.log_softmax(y / T), F.softmax(teacher_scores / T)) * (
            T * T * 2.0 * alpha) + F.cross_entropy(y, labels) * (1. - alpha)


batch_size = 32
learning_rate = 1e-3
num_epochs = 10
num_workers = 4
alpha = 0.9
T = 2.0

transform = transforms.Compose([
    # transforms.Resize(224),
    transforms.ToTensor(),
    transforms.Normalize((0.5,), (0.5,)),
])

train_dataset = torchvision.datasets.CIFAR10(root='./data', train=True, download=True, transform=transform)
train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=batch_size, shuffle=True, num_workers=num_workers)

test_dataset = torchvision.datasets.CIFAR10(root='./data', train=False, download=True, transform=transform)
test_loader = torch.utils.data.DataLoader(test_dataset, batch_size=batch_size, shuffle=False, num_workers=num_workers)

print(len(train_loader))
print(len(test_loader))