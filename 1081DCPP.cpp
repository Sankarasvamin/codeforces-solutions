che#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
using namespace std;

using ll = long long;
static const ll NEG_INF = -(1LL << 62);

struct LiChao {
  int xl, xr;
  vector<ll> m, b;
  vector<int> lc, rc;

  LiChao(int l, int r) : xl(l), xr(r) {
    m.push_back(0);
    b.push_back(NEG_INF);
    lc.push_back(0);
    rc.push_back(0);
  }

  inline ll eval(ll mm, ll bb, int x) const {
    return mm * x + bb;
  }

  int new_node(ll mm, ll bb) {
    m.push_back(mm);
    b.push_back(bb);
    lc.push_back(0);
    rc.push_back(0);
    return (int)m.size() - 1;
  }

  void add_line(ll mm, ll bb) { add(0, xl, xr, mm, bb); }

  void add(int idx, int l, int r, ll mm, ll bb) {
    int mid = (l + r) >> 1;

    ll cm = m[idx], cb = b[idx];
    if (cb == NEG_INF) {
      m[idx] = mm;
      b[idx] = bb;
      return;
    }

    if (eval(mm, bb, mid) > eval(cm, cb, mid)) {
      swap(m[idx], mm);
      swap(b[idx], bb);
      cm = m[idx];
      cb = b[idx];
    }

    if (l == r) return;

    if (eval(mm, bb, l) > eval(cm, cb, l)) {
      int left = lc[idx];
      if (left == 0) {
        left = new_node(0, NEG_INF);
        lc[idx] = left;
      }
      add(left, l, mid, mm, bb);
    } else if (eval(mm, bb, r) > eval(cm, cb, r)) {
      int right = rc[idx];
      if (right == 0) {
        right = new_node(0, NEG_INF);
        rc[idx] = right;
      }
      add(right, mid + 1, r, mm, bb);
    }
  }

  ll query(int x) const {
    int idx = 0;
    int l = xl, r = xr;
    ll ans = NEG_INF;

    while (true) {
      if (b[idx] != NEG_INF) {
        ans = max(ans, m[idx] * x + b[idx]);
      }
      if (l == r) break;
      int mid = (l + r) >> 1;
      if (x <= mid) {
        int nxt = lc[idx];
        if (nxt == 0) break;
        idx = nxt;
        r = mid;
      } else {
        int nxt = rc[idx];
        if (nxt == 0) break;
        idx = nxt;
        l = mid + 1;
      }
    }

    return ans;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;

    vector<ll> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; ++i) {
      int u, v;
      cin >> u >> v;
      g[u].push_back(v);
      g[v].push_back(u);
    }

    vector<int> parent(n + 1, 0), depth(n + 1, 0);
    vector<vector<int>> children(n + 1);
    vector<int> order;
    order.reserve(n);

    vector<int> st;
    st.reserve(n);
    st.push_back(1);
    parent[1] = -1;

    while (!st.empty()) {
      int u = st.back();
      st.pop_back();
      order.push_back(u);
      for (int v : g[u]) {
        if (v == parent[u]) continue;
        parent[v] = u;
        depth[v] = depth[u] + 1;
        children[u].push_back(v);
        st.push_back(v);
      }
    }

    vector<int> sub_size(n + 1, 1), dmax(n + 1, 0);
    vector<ll> sub_sum(n + 1, 0), sub_weighted(n + 1, 0), imp(n + 1, NEG_INF);

    vector<LiChao*> hull(n + 1, nullptr);
    vector<vector<pair<ll, ll>>> lines(n + 1);
    vector<unique_ptr<LiChao>> owned;
    owned.reserve(n);

    for (int i = n - 1; i >= 0; --i) {
      int u = order[i];
      ll su = a[u];
      ll sw = a[u] * depth[u];
      int dm = depth[u];
      int sz = 1;

      ll best_imp = NEG_INF;

      int top1 = -1, top2 = -1, cnt1 = 0;

      for (int c : children[u]) {
        su += sub_sum[c];
        sw += sub_weighted[c];
        sz += sub_size[c];
        dm = max(dm, dmax[c]);
        best_imp = max(best_imp, imp[c]);

        int dc = dmax[c];
        if (dc > top1) {
          top2 = top1;
          top1 = dc;
          cnt1 = 1;
        } else if (dc == top1) {
          cnt1++;
        } else if (dc > top2) {
          top2 = dc;
        }
      }

      sub_sum[u] = su;
      sub_weighted[u] = sw;
      dmax[u] = dm;
      sub_size[u] = sz;

      for (int c : children[u]) {
        int dc = dmax[c];
        int other = (dc == top1 && cnt1 == 1) ? top2 : top1;
        int bdepth = max(depth[u], other);
        ll cand = hull[c]->query(bdepth);
        best_imp = max(best_imp, cand);
      }

      imp[u] = best_imp;

      if (children[u].empty()) {
        owned.push_back(make_unique<LiChao>(0, n));
        LiChao* h = owned.back().get();
        ll m = su;
        ll b = su * (1LL - depth[u]);
        h->add_line(m, b);
        hull[u] = h;
        lines[u].push_back({m, b});
        continue;
      }

      int big = children[u][0];
      for (int c : children[u]) {
        if (sub_size[c] > sub_size[big]) big = c;
      }

      LiChao* h = hull[big];
      auto bag = std::move(lines[big]);

      for (int c : children[u]) {
        if (c == big) continue;
        for (auto &ln : lines[c]) {
          h->add_line(ln.first, ln.second);
        }
        bag.insert(bag.end(), lines[c].begin(), lines[c].end());
        vector<pair<ll, ll>>().swap(lines[c]);
      }

      ll m = su;
      ll b = su * (1LL - depth[u]);
      h->add_line(m, b);
      bag.push_back({m, b});

      hull[u] = h;
      lines[u] = std::move(bag);
    }

    for (int u = 1; u <= n; ++u) {
      ll base = sub_weighted[u] - 1LL * depth[u] * sub_sum[u];
      ll gain = max(0LL, imp[u]);
      ll ans = base + gain;
      cout << ans << (u == n ? '\n' : ' ');
    }
  }

  return 0;
}
