import sys

a, b = [line.strip().lower() for line in sys.stdin]

if a < b:
    print(-1)
elif a > b:
    print(1)
else:
    print(0)