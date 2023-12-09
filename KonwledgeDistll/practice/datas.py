import datasets
from datasets import Dataset,load_dataset
ds = Dataset.from_text('./corpus/dataset.txt')
ds = ds.filter(lambda x: len(x['text'].strip()) > 0) # Filter out empty lines
print(ds)
for data in ds:
    print(data)
ds2 = load_dataset("text", data_dir="./corpus")
print(ds2)