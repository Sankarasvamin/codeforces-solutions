t = int(input())
for _ in range(t):
    n = int(input())
    s = input().strip()

    transitions = 0
    for i in range(1, n):
        if s[i] != s[i-1]:
            transitions += 1

    ans = transitions + 1
    if n > 1 and s[0] != s[-1]:
        ans += 1

    ans = min(n, ans)
    print(ans)