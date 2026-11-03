import sys
matrix = list(map(int,sys.stdin.read().split()))
location = next(i for i, x in enumerate(matrix) if x != 0)

x = location % 5 - 2
y = 2 - location // 5

print(abs(x) + abs(y))