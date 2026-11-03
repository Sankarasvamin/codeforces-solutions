import sys

t = 0

lines = [line.strip() for line in sys.stdin]
n = int(lines[0])

for i in range(1, n + 1):
    nums = list(map(int, lines[i].split()))
    if sum(nums) >= 2:
        t += 1

print(t)