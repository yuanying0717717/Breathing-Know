import os
import random
import csv

def creat_csv():
    pathm = r'D:\Breathing\demo\mfcc.csv'
    with open(pathm, "w", newline='') as f:
        csv_write = csv.writer(f)

        csv_head = ['path','id', 'channel', 'chunks','label']
        csv_write.writerow(csv_head)

def write_csv(a, b, c,d,e):
    pathm = r'D:\Breathing\demo\mfcc.csv'
    with open(pathm, "a+", newline='') as f:
        csv_write = csv.writer(f)
        data_row = [a, b, c, d, e]
        csv_write.writerow(data_row)

def file_all():

    folder_path = r'D:\Breathing\demo\mat'

    for file_name in os.listdir(folder_path):

        if file_name.endswith('.mat'):
            path = folder_path + '\\' + file_name

            file_name_without_extension = file_name.split('.')[0]
            file_name_parts = file_name_without_extension.split('_')

            write_csv(path, file_name_parts[0],file_name_parts[1],file_name_parts[2],file_name_parts[3])

def shuffle():
    input_csv = r'D:\Breathing\demo\mfcc.csv'
    output_csv = r'D:\Breathing\demo\mfccShuffle.csv'

    rows = []
    with open(input_csv, 'r', newline='') as file:
        reader = csv.reader(file)
        header = next(reader)
        for row in reader:
            rows.append(row)

    random.shuffle(rows)

    with open(output_csv, 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(header)
        writer.writerows(rows)

# creat_csv()
