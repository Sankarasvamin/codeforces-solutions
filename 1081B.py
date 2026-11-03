import sys
input = sys.stdin.readline

t = int(input())
for _ in range(t):
    n = int(input())
    s = input().strip()

    ones_pos = [i + 1 for i, ch in enumerate(s) if ch == '1']
    zeros_pos = [i + 1 for i, ch in enumerate(s) if ch == '0']

    ones = len(ones_pos)
    zeros = n - ones

    candidates = []

    if ones % 2 == 0:
        candidates.append(ones_pos)

    if zeros % 2 == 1:
        candidates.append(zeros_pos)

    if not candidates:
        print(-1)
        continue

    ops = min(candidates, key=len)
    print(len(ops))
    if len(ops) == 0:
        print()
    else:
        print(*ops)