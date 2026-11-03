import sys
weight = sys.stdin.read().split()
a = int(weight[0])
b = int(weight[1])
count = 0

while a <= b:
    a *= 3
    b *= 2
    count += 1

print(count)