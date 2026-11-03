#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;

        vector<int> a(n), b(n);
        for (int i = 0; i < n; i++) cin >> a[i];
        for (int i = 0; i < n; i++) cin >> b[i];

        vector<int> deg(n + 1, 0);
        for (int i = 0; i < n; i++) {
            deg[a[i]]++;
            deg[b[i]]++;
        }

        bool ok = true;
        for (int i = 1; i <= n; i++) {
            if (deg[i] % 2) {
                ok = false;
                break;
            }
        }

        if (!ok) {
            cout << -1 << "\n";
            continue;
        }

        vector<vector<pair<int,int>>> adj(n + 1);
        for (int i = 0; i < n; i++) {
            adj[a[i]].push_back({b[i], i});
            adj[b[i]].push_back({a[i], i});
        }

        vector<bool> used(n, false);
        vector<int> swap_flag(n, 0);

        for (int start = 1; start <= n; start++) {
            if (adj[start].empty()) continue;

            stack<int> st;
            stack<int> edge_st;
            st.push(start);

            while (!st.empty()) {
                int v = st.top();
                while (!adj[v].empty() && used[adj[v].back().second])
                    adj[v].pop_back();

                if (adj[v].empty()) {
                    st.pop();
                    continue;
                }

                auto [to, id] = adj[v].back();
                adj[v].pop_back();
                if (used[id]) continue;

                used[id] = true;
                st.push(to);

                if (v == b[id])
                    swap_flag[id] = 1;
            }
        }

        vector<int> ops;
        for (int i = 0; i < n; i++)
            if (swap_flag[i]) ops.push_back(i + 1);

        cout << ops.size() << "\n";
        for (int x : ops) cout << x << " ";
        cout << "\n";
    }
}