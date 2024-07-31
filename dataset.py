import torch
from torch.utils.data import Dataset
import scipy.io
import pandas as pd

class MFCCDataset(Dataset):
    def __init__(self, csv_file):
        self.data_info = pd.read_csv(csv_file)

    def __len__(self):
        return len(self.data_info)

    def __getitem__(self, idx):
        if torch.is_tensor(idx):
            idx = idx.tolist()

        mfcc_path = self.data_info.iloc[idx, 0]
        label = self.data_info.iloc[idx, 4]

        mat = scipy.io.loadmat(mfcc_path)

        mfcc_features = mat['coeffs']

        mfcc_features = mfcc_features[:128, :14]

        mfcc_features = torch.tensor(mfcc_features, dtype=torch.float32)
        label = torch.tensor(label, dtype=torch.long)

        return mfcc_features, label
