import sys
from matplotlib import pyplot as plt

if len(sys.argv) < 2:
    print("Usage: python3 p.py <file1> <file2> ...")
    sys.exit(1)
files = sys.argv[1:]

for file in files:
    ops = ["add", "dsp"]
    for op in ops:
        x, y = [], []
        row = 1
        with open(file) as f:
            lines = []
            for line in f:
                if row == 750:
                    break
                if op in line:
                    x.append(int(line[line.find("R ")+2:]))
                    y.append(row)
                    row += 1
        plt.title(file)
        plt.xlabel('sectors') 
        plt.ylabel('order') 
        plt.figure(figsize=(5,15))
        plt.plot(x, y)
        type = "fifo" if op == "add" else "sstf"
        png = file.split(".")[0] + "-" + type + ".png"
        plt.savefig(png)