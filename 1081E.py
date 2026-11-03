import sys
sys.setrecursionlimit(1_000_000)
input = sys.stdin.readline

t = int(input())
for _ in range(t):
    n = int(input())
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))

    deg = [0] * (n + 1)
    for x in a:
        deg[x] += 1
    for x in b:
        deg[x] += 1

    ok = True
    for x in range(1, n + 1):
        if deg[x] & 1:
            ok = False
            break
    if not ok:
        print(-1)
        continue

    adj = [[] for _ in range(n + 1)]
    for i in range(n):
        u, v = a[i], b[i]
        adj[u].append((v, i))
        adj[v].append((u, i))

    used = [False] * n
    dir_swap = [0] * n
    it = [0] * (n + 1)

    for start in range(1, n + 1):
        # 关键修正：先把 start 的指针跳到第一个未用边
        while it[start] < len(adj[start]) and used[adj[start][it[start]][1]]:
            it[start] += 1
        if it[start] == len(adj[start]):
            continue

        stack = [start]
        while stack:
            v = stack[-1]
            while it[v] < len(adj[v]) and used[adj[v][it[v]][1]]:
                it[v] += 1
            if it[v] == len(adj[v]):
                stack.pop()
                continue

            to, eid = adj[v][it[v]]
            it[v] += 1
            if used[eid]:
                continue
            used[eid] = True

            if v == b[eid] and to == a[eid]:
                dir_swap[eid] = 1

            stack.append(to)

    ops = [i + 1 for i in range(n) if dir_swap[i] == 1]
    print(len(ops))
    if ops:
        print(*ops)
    else:
        print()