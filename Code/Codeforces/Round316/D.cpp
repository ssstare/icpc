#include <bits/stdc++.h>

const int N = 500000 + 5;
std::vector<int> edges[N];
int n,m;
char str[N];
int depth[N];
std::vector<int> shifts[N];
int st[N],ed[N],tim;
int rd[N];
bool flag[N][26];

struct Querier {
    int id,v;
};

std::vector<Querier> queries[N];

void dfs(int u) {
    rd[tim] = u;
    st[u] = tim ++;
    shifts[depth[u]].push_back(st[u]);
    for (int v : edges[u]) {
        depth[v] = depth[u] + 1;
        dfs(v);
    }
    ed[u] = tim - 1;
}

bool answer[N];

void work() {
    dfs(0);
    for (int i = 0; i < n; ++ i) {
        for (int c = 0; c < 26; ++ c) {
            flag[shifts[i].size()][c] = false;
        }
        for (int j = (int)shifts[i].size() - 1; j >= 0; -- j) {
            for (int c = 0; c < 26; ++ c) {
                flag[j][c] = flag[j + 1][c];
            }
            flag[j][str[rd[shifts[i][j]]] - 'a'] ^= 1;
        }
        for (auto q : queries[i]) {
            int l = std::lower_bound(shifts[i].begin(),shifts[i].end(),
                    st[q.v]) - shifts[i].begin();
            int r = std::upper_bound(shifts[i].begin(),shifts[i].end(),
                    ed[q.v]) - shifts[i].begin();
            int cnt = 0;
            for (int c = 0; c < 26; ++ c) {
                cnt += flag[l][c] ^ flag[r][c];
                if (cnt > 1) break;
            }
            answer[q.id] = cnt <= 1;
        }
    }
}

int main() {
    scanf("%d%d",&n,&m);
    for (int i = 1; i < n; ++ i) {
        int x;
        scanf("%d",&x); x --;
        edges[x].push_back(i);
    }
    scanf("%s",str);
    for (int i = 0; i < m; ++ i) {
        int v,h;
        scanf("%d%d",&v,&h); v --; h --;
        queries[h].push_back({i,v});
    }
    work();
    for (int i = 0; i < m; ++ i) {
        puts(answer[i] ? "Yes" : "No");
    }
}
