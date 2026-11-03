import sys

lines = [line.strip() for line in sys.stdin]
n = int(lines[0])
x = 0

for i in range(1, n + 1):
    bit = lines[i]
    if "+" in bit:
        x += 1
    elif "-" in bit:
        x += -1

print(x)
