from torch.utils.data import DataLoader
from dataset import MFCCDataset

csv_file = r'D:\Breathing\demo\mfccShuffle.csv'

dataset = MFCCDataset(csv_file)

batch_size = len(dataset)
test_loader = DataLoader(dataset, batch_size=batch_size, shuffle=True)


