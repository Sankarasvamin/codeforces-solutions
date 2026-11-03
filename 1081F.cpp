#include <iostream>
#include <vector>

using namespace std;

static const int MOD = 1000000007;

static inline int addmod(int a, int b) {
  a += b;
  if (a >= MOD) a -= MOD;
  return a;
}

static inline int halfmod(int x) {
  if (x & 1) x += MOD;
  return x >> 1;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    int n;
    cin >> n;

    int x, y;
    cin >> x >> y;

    vector<long long> a(n + 2), b(n + 2);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    for (int i = 1; i <= n; ++i) cin >> b[i];

    if (x > y) {
      vector<long long> ra(n + 2), rb(n + 2);
      for (int i = 1; i <= n; ++i) {
        ra[i] = a[n + 1 - i];
        rb[i] = b[n + 1 - i];
      }
      a.swap(ra);
      b.swap(rb);
      x = n + 1 - x;
      y = n + 1 - y;
    }

    const int L = x - 1;
    const int D = y - x - 1;
    const int R = n - y;

    vector<long long> prefLA(L + 1, 0), prefMA(D + 1, 0), prefRB(R + 1, 0), prefMB(D + 1, 0);
    for (int t = 1; t <= L; ++t) prefLA[t] = prefLA[t - 1] + b[x - t];
    for (int t = 1; t <= D; ++t) prefMA[t] = prefMA[t - 1] + b[x + t];
    for (int t = 1; t <= R; ++t) prefRB[t] = prefRB[t - 1] + b[y + t];
    for (int t = 1; t <= D; ++t) prefMB[t] = prefMB[t - 1] + b[y - t];

    const int W = D + 1;
    const int SZ = W * W;
    vector<int> cur(SZ, 0), nxt(SZ, 0);

    auto idOf = [W](int j, int k) { return j * W + k; };

    cur[idOf(0, 0)] = 1;
    int ans = 0;

    const int rMax = min(L + D, R + D);

    vector<long long> SA0(W), SA1L(W), SA1M(W), SB0(W);
    vector<unsigned char> aLeft(W), aMidBase(W), bRight(W), bMidBase(W);

    for (int r = 0; r <= rMax; ++r) {
      int jLo = r - L;
      if (jLo < 0) jLo = 0;
      int jHi = r;
      if (jHi > D) jHi = D;

      int kLo = r - R;
      if (kLo < 0) kLo = 0;
      int kHi = r;
      if (kHi > D) kHi = D;

      // Precompute row-wise values for current r.
      for (int j = jLo; j <= jHi; ++j) {
        int iOut = r - j;
        long long sa = a[x] + prefLA[iOut] + prefMA[j];
        SA0[j] = sa;

        if (iOut < L) {
          int idxAL = x - iOut - 1;
          aLeft[j] = (sa >= a[idxAL]) ? 1 : 0;
          SA1L[j] = a[x] + prefLA[iOut + 1] + prefMA[j];
        } else {
          aLeft[j] = 0;
          SA1L[j] = -1;
        }

        if (j < D) {
          int idxAM = x + j + 1;
          aMidBase[j] = (sa >= a[idxAM]) ? 1 : 0;
          SA1M[j] = a[x] + prefLA[iOut] + prefMA[j + 1];
        } else {
          aMidBase[j] = 0;
          SA1M[j] = -1;
        }
      }

      for (int k = kLo; k <= kHi; ++k) {
        int oOut = r - k;
        long long sb = a[y] + prefRB[oOut] + prefMB[k];
        SB0[k] = sb;

        if (oOut < R) {
          int idxBR = y + oOut + 1;
          bRight[k] = (sb >= a[idxBR]) ? 1 : 0;
        } else {
          bRight[k] = 0;
        }

        if (k < D) {
          int idxBM = y - k - 1;
          bMidBase[k] = (sb >= a[idxBM]) ? 1 : 0;
        } else {
          bMidBase[k] = 0;
        }
      }

      bool anyNext = false;

      for (int j = jLo; j <= jHi; ++j) {
        int kL = kLo;
        int cap = D - j;
        if (kL > cap) continue;
        int kR = kHi;
        if (kR > cap) kR = cap;

        int baseId = j * W;

        for (int k = kL; k <= kR; ++k) {
          int id = baseId + k;
          int p = cur[id];
          if (p == 0) continue;
          cur[id] = 0;

          long long sa0 = SA0[j];
          long long sb0 = SB0[k];

          unsigned char aL = aLeft[j];
          unsigned char aM;
          unsigned char aBob = 0;

          if (j + k < D) {
            aM = aMidBase[j];
          } else {
            aM = (sa0 >= sb0) ? 1 : 0;
            aBob = aM;
          }

          int cntA = (int)aL + (int)aM;
          if (cntA == 0) continue;

          int pA = (cntA == 2) ? halfmod(p) : p;

          // Alice eats left
          if (aL) {
            int j1 = j;
            long long sa1 = SA1L[j];

            unsigned char bR = bRight[k];
            unsigned char bM;
            unsigned char bAlice = 0;

            if (j1 + k < D) {
              bM = bMidBase[k];
            } else {
              bM = (sb0 >= sa1) ? 1 : 0;
              bAlice = bM;
            }

            int cntB = (int)bR + (int)bM;
            if (cntB == 0) {
              ans = addmod(ans, pA);
            } else {
              int pB = (cntB == 2) ? halfmod(pA) : pA;

              if (bR) {
                int nid = j1 * W + k;
                int old = nxt[nid];
                nxt[nid] = old ? addmod(old, pB) : pB;
                anyNext = true;
              }
              if (bM && !bAlice) {
                int k1 = k + 1;
                if (j1 + k1 <= D && r + 1 - k1 <= R) {
                  int nid = j1 * W + k1;
                  int old = nxt[nid];
                  nxt[nid] = old ? addmod(old, pB) : pB;
                  anyNext = true;
                }
              }
            }
          }

          // Alice eats middle / Bob
          if (aM) {
            if (aBob) {
              ans = addmod(ans, pA);
            } else {
              int j1 = j + 1;
              long long sa1 = SA1M[j];

              unsigned char bR = bRight[k];
              unsigned char bM;
              unsigned char bAlice = 0;

              if (j1 + k < D) {
                bM = bMidBase[k];
              } else {
                bM = (sb0 >= sa1) ? 1 : 0;
                bAlice = bM;
              }

              int cntB = (int)bR + (int)bM;
              if (cntB == 0) {
                ans = addmod(ans, pA);
              } else {
                int pB = (cntB == 2) ? halfmod(pA) : pA;

                if (bR) {
                  int nid = j1 * W + k;
                  int old = nxt[nid];
                  nxt[nid] = old ? addmod(old, pB) : pB;
                  anyNext = true;
                }
                if (bM && !bAlice) {
                  int k1 = k + 1;
                  if (j1 + k1 <= D && r + 1 - k1 <= R) {
                    int nid = j1 * W + k1;
                    int old = nxt[nid];
                    nxt[nid] = old ? addmod(old, pB) : pB;
                    anyNext = true;
                  }
                }
              }
            }
          }
        }
      }

      if (!anyNext) break;

      // Move nxt -> cur and clear nxt lazily by scanning feasible layer r+1.
      int nr = r + 1;
      int njLo = nr - L;
      if (njLo < 0) njLo = 0;
      int njHi = nr;
      if (njHi > D) njHi = D;

      int nkLo = nr - R;
      if (nkLo < 0) nkLo = 0;
      int nkHi = nr;
      if (nkHi > D) nkHi = D;

      for (int j = njLo; j <= njHi; ++j) {
        int kL = nkLo;
        int cap = D - j;
        if (kL > cap) continue;
        int kR = nkHi;
        if (kR > cap) kR = cap;

        int baseId = j * W;
        for (int k = kL; k <= kR; ++k) {
          int id = baseId + k;
          cur[id] = nxt[id];
          nxt[id] = 0;
        }
      }
    }

    cout << ans << '\n';
  }

  return 0;
}
