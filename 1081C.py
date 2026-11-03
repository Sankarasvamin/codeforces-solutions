import sys
input = sys.stdin.readline

t = int(input())
for _ in range(t):
    n, h, k = map(int, input().split())
    a = list(map(int, input().split()))

    S = sum(a)

    # prefix sums
    prefix = [0] * (n + 1)
    for i in range(n):
        prefix[i + 1] = prefix[i] + a[i]

    # suffix max: suffix_max[i] = max(a[i..n-1])
    suffix_max = [0] * (n + 1)
    suffix_max[n] = -10**30
    for i in range(n - 1, -1, -1):
        suffix_max[i] = max(suffix_max[i + 1], a[i])

    ans = 10**30
    min_pref = 10**30  # min in a[0..r-1]

    for r in range(1, n + 1):
        sum_r = prefix[r]
        min_pref = min(min_pref, a[r - 1])

        best = sum_r
        if r < n:
            # swap out the smallest in first r with the largest outside
            best = max(best, sum_r - min_pref + suffix_max[r])

        # Now compute time using best prefix for r
        if best >= h:
            ans = min(ans, r)
        else:
            need = h - best
            m = (need + S - 1) // S   # full magazines before final partial
            ans = min(ans, m * (n + k) + r)

    print(ans)