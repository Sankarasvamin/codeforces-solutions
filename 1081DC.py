import sys


NEG_INF = -10**30


class LiChao:
    __slots__ = ("xl", "xr", "m", "b", "lc", "rc")

    def __init__(self, xl: int, xr: int):
        self.xl = xl
        self.xr = xr
        self.m = [0]
        self.b = [NEG_INF]
        self.lc = [0]
        self.rc = [0]

    @staticmethod
    def _eval(m: int, b: int, x: int) -> int:
        return m * x + b

    def _new_node(self, m: int, b: int) -> int:
        self.m.append(m)
        self.b.append(b)
        self.lc.append(0)
        self.rc.append(0)
        return len(self.m) - 1

    def add_line(self, m: int, b: int) -> None:
        self._add(0, self.xl, self.xr, m, b)

    def _add(self, idx: int, l: int, r: int, m: int, b: int) -> None:
        mid = (l + r) >> 1

        cur_m = self.m[idx]
        cur_b = self.b[idx]

        if cur_b == NEG_INF:
            self.m[idx] = m
            self.b[idx] = b
            return

        if self._eval(m, b, mid) > self._eval(cur_m, cur_b, mid):
            self.m[idx], m = m, cur_m
            self.b[idx], b = b, cur_b
            cur_m = self.m[idx]
            cur_b = self.b[idx]

        if l == r:
            return

        if self._eval(m, b, l) > self._eval(cur_m, cur_b, l):
            left = self.lc[idx]
            if left == 0:
                left = self._new_node(0, NEG_INF)
                self.lc[idx] = left
            self._add(left, l, mid, m, b)
        elif self._eval(m, b, r) > self._eval(cur_m, cur_b, r):
            right = self.rc[idx]
            if right == 0:
                right = self._new_node(0, NEG_INF)
                self.rc[idx] = right
            self._add(right, mid + 1, r, m, b)

    def query(self, x: int) -> int:
        idx = 0
        l, r = self.xl, self.xr
        ans = NEG_INF
        while True:
            if self.b[idx] != NEG_INF:
                v = self.m[idx] * x + self.b[idx]
                if v > ans:
                    ans = v
            if l == r:
                break
            mid = (l + r) >> 1
            if x <= mid:
                nxt = self.lc[idx]
                if nxt == 0:
                    break
                idx = nxt
                r = mid
            else:
                nxt = self.rc[idx]
                if nxt == 0:
                    break
                idx = nxt
                l = mid + 1
        return ans


def solve_one(n: int, a, edges):
    adj = [[] for _ in range(n + 1)]
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)

    parent = [0] * (n + 1)
    depth = [0] * (n + 1)
    children = [[] for _ in range(n + 1)]
    order = []

    stack = [1]
    parent[1] = -1
    while stack:
        u = stack.pop()
        order.append(u)
        pu = parent[u]
        for v in adj[u]:
            if v == pu:
                continue
            parent[v] = u
            depth[v] = depth[u] + 1
            children[u].append(v)
            stack.append(v)

    sub_size = [1] * (n + 1)
    sub_sum = [0] * (n + 1)
    sub_weighted = [0] * (n + 1)
    dmax = [0] * (n + 1)
    imp = [NEG_INF] * (n + 1)

    hull = [None] * (n + 1)
    lines = [None] * (n + 1)

    for u in reversed(order):
        su = a[u]
        sw = a[u] * depth[u]
        dm = depth[u]
        sz = 1

        best_imp = NEG_INF

        ch = children[u]

        top1 = -1
        top2 = -1
        cnt1 = 0

        for c in ch:
            su += sub_sum[c]
            sw += sub_weighted[c]
            sz += sub_size[c]

            dc = dmax[c]
            if dc > dm:
                dm = dc

            if imp[c] > best_imp:
                best_imp = imp[c]

            if dc > top1:
                top2 = top1
                top1 = dc
                cnt1 = 1
            elif dc == top1:
                cnt1 += 1
            elif dc > top2:
                top2 = dc

        sub_sum[u] = su
        sub_weighted[u] = sw
        dmax[u] = dm
        sub_size[u] = sz

        for c in ch:
            dc = dmax[c]
            if dc == top1 and cnt1 == 1:
                other = top2
            else:
                other = top1
            if other < depth[u]:
                bdepth = depth[u]
            else:
                bdepth = other

            cand = hull[c].query(bdepth)
            if cand > best_imp:
                best_imp = cand

        imp[u] = best_imp

        if not ch:
            h = LiChao(0, n)
            line_m = su
            line_b = su * (1 - depth[u])
            h.add_line(line_m, line_b)
            hull[u] = h
            lines[u] = [(line_m, line_b)]
            continue

        big = ch[0]
        max_sz = sub_size[big]
        for c in ch[1:]:
            if sub_size[c] > max_sz:
                max_sz = sub_size[c]
                big = c

        h = hull[big]
        bag = lines[big]

        for c in ch:
            if c == big:
                continue
            lst = lines[c]
            for m, b in lst:
                h.add_line(m, b)
            bag.extend(lst)

        line_m = su
        line_b = su * (1 - depth[u])
        h.add_line(line_m, line_b)
        bag.append((line_m, line_b))

        hull[u] = h
        lines[u] = bag

    ans = [0] * (n + 1)
    for u in range(1, n + 1):
        base = sub_weighted[u] - depth[u] * sub_sum[u]
        gain = imp[u]
        if gain < 0:
            gain = 0
        ans[u] = base + gain

    return ans[1:]


def main():
    data = list(map(int, sys.stdin.buffer.read().split()))
    t = data[0]
    idx = 1

    out = []
    for _ in range(t):
        n = data[idx]
        idx += 1

        a = [0] * (n + 1)
        for i in range(1, n + 1):
            a[i] = data[idx]
            idx += 1

        edges = []
        for _ in range(n - 1):
            u = data[idx]
            v = data[idx + 1]
            idx += 2
            edges.append((u, v))

        res = solve_one(n, a, edges)
        out.append(" ".join(map(str, res)))

    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
