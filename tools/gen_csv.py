#!/usr/bin/env python3
import csv
import os
import random
import string

def random_word(n):
    return ''.join(random.choice(string.ascii_lowercase) for _ in range(n))

def ensure_dir(path):
    os.makedirs(path, exist_ok=True)

def gen_large(path, rows):
    headers = ["id", "name", "price", "desc"]
    with open(path, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(headers)
        for i in range(rows):
            name = f"{random_word(5)}_{i}"
            price = f"{random.randint(1, 10_000)}.{random.randint(0,99):02d}"
            # include special XML chars to test escaping
            desc = f"Item & <{i}> \"{name}\" 'ok'"
            w.writerow([i, name, price, desc])

def gen_small_examples(base_dir):
    # Small example 1
    path1 = os.path.join(base_dir, "small.csv")
    with open(path1, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["Brand","Model","Year"])
        w.writerow(["Ford","Focus","2019"])
        w.writerow(["BMW","X5","2020"])
        w.writerow(["Audi","A4","2018"])
    # Special chars example
    path2 = os.path.join(base_dir, "with_specials.csv")
    with open(path2, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["A","B","C"])
        w.writerow(["1","A&B","<tag>"])
        w.writerow(["2","\"quoted\"","it's fine"])
        w.writerow(["3","mix & < > \" '","end"])

def main():
    base = os.path.join(os.path.dirname(__file__), "..", "tests")
    base = os.path.abspath(base)
    ensure_dir(base)
    gen_small_examples(base)
    gen_large(os.path.join(base, "large_10000.csv"), 10_000)
    print(f"Generated test data under: {base}")

if __name__ == "__main__":
    main()


