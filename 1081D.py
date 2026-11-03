import sys

# 提高递归深度
sys.setrecursionlimit(500000)
input = sys.stdin.readline

def solve():
    line = input().split()
    if not line: return
    n = int(line[0])
    a = [0] + list(map(int, input().split()))
    
    adj = [[] for _ in range(n + 1)]
    for _ in range(n - 1):
        u, v = map(int, input().split())
        adj[u].append(v)
        adj[v].append(u)

    # 1. 建立以 1 为根的树结构并计算基础属性
    parent = [0] * (n + 1)
    order = []
    stack = [1]
    while stack:
        u = stack.pop()
        order.append(u)
        for v in adj[u]:
            if v != parent[u]:
                parent[v] = u
                stack.append(v)

    # sz[u]: 子树 u 的权值和
    # f[u]: 子树 u 内部的 cost (相对于 u)
    # max_d[u]: u 到其子树内最深节点的距离
    sz = [0] * (n + 1)
    f = [0] * (n + 1)
    max_d = [0] * (n + 1)

    for u in reversed(order):
        sz[u] = a[u]
        for v in adj[u]:
            if v != parent[u]:
                sz[u] += sz[v]
                f[u] += f[v] + sz[v]
                max_d[u] = max(max_d[u], max_d[v] + 1)

    # 2. 核心：计算每个节点 r 的最大代价
    # 对于固定的 r，我们需要在它的子树里找一个 u 移动。
    # 移动 u 后的代价 = f[r] + max(0, sz[u] * (其他分支的最大深度 + 1 - dist(r, u)))
    
    ans = [0] * (n + 1)

    # 为了高效处理每个 r，我们需要知道 r 子树内各分支的深度
    for r in order:
        base_f = f[r]
        
        # 收集直接子节点提供的深度
        depths = []
        for v in adj[r]:
            if v != parent[r]:
                depths.append([max_d[v] + 1, v])
        
        depths.sort(key=lambda x: x[0], reverse=True)
        
        max_gain = 0
        
        # 内部 DFS 遍历 r 的所有后代 u
        # 只有存在至少两个分支或者移动能带来正收益时才操作
        sub_stack = []
        for v in adj[r]:
            if v != parent[r]:
                sub_stack.append((v, 1, v)) # (当前节点, 距离r的深度, 所属r的直接子分支)

        while sub_stack:
            u, d_ru, root_v = sub_stack.pop()
            
            # 找到不包含 u 的路径的最深深度
            # 情况 A: u 在最深的分支里，我们只能把 u 挂在次深分支上
            # 情况 B: u 不在最深分支里，我们把 u 挂在最深分支上
            other_max_d = 0
            if depths:
                if root_v == depths[0][1]:
                    if len(depths) > 1:
                        other_max_d = depths[1][0]
                    else:
                        other_max_d = 0 # 只有一条路，只能挂回 r
                else:
                    other_max_d = depths[0][0]
            
            # 增益 = sz[u] * (目标深度 + 1 - 当前深度)
            gain = sz[u] * (other_max_d + 1 - d_ru)
            if gain > max_gain:
                max_gain = gain
            
            for v in adj[u]:
                if v != parent[u]:
                    sub_stack.append((v, d_ru + 1, root_v))
        
        ans[r] = base_f + max_gain

    print(*(ans[1:]))

t_input = input().strip()
if t_input:
    t = int(t_input)
    for _ in range(t):
        solve()