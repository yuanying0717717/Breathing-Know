import torch
from dataLoader import test_loader
from main import file_all, shuffle
from modelCNN import SimpleCNN
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score

def testing():
    model = SimpleCNN()

    path = r"./CNN10.pt"
    model.load_state_dict(torch.load(path))

    model.eval()
    test_loss = 0.0
    correct = 0
    total = 0
    true = []
    pred = []

    with torch.no_grad():
        for mfcc_features, labels in test_loader:
            mfcc_features = mfcc_features.unsqueeze(1)
            outputs = model(mfcc_features)
            y_ture = labels.numpy()
            _, predicted = torch.max(outputs.data, 1)
            y_pred = predicted.numpy()

            true.extend(y_ture)
            pred.extend(y_pred)

            total += labels.size(0)
            correct += (predicted == labels).sum().item()

    print("true: ", true)
    print("pred: ", pred)

    accuracy = accuracy_score(true, pred)
    print(f'Accuracy score: {accuracy:.6f}')

    precision = precision_score(true, pred, average='weighted', zero_division=0)
    print(f'Precision score:  {precision:.6f}')

    recall = recall_score(true, pred, average='weighted')
    print(f'recall:  {recall:.6f}')

    f1 = f1_score(true, pred, average='weighted')
    print(f'F1:  {f1:.6f}')

file_all()
shuffle()
testing()