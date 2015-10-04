#include <bits/stdc++.h>

void my_assert(bool flag) {
    if (!flag) {
        printf("%d\n",1 / 0);
    }
}

template<int N>
struct Sat {
    int head[N],etot,stack[N],top;
    bool mark[N];
    struct Edge {
        int v,next;
    } g[2000000 + 5];

    void add_edge(int u,int v) {
        g[etot].v = v; g[etot].next = head[u]; head[u] = etot ++;
    }

    void init() {
        memset(head,-1,sizeof(head)); etot = 0;
    }

    bool dfs(int u) {
        if (mark[u ^ 1]) return false;
        if (mark[u]) return true;
        mark[u] = true;
        stack[top++] = u;
        for (int i = head[u]; i != -1; i = g[i].next) {
            if (!dfs(g[i].v)) return false;
        }
        return true;
    }

    bool work(int n) {
        memset(mark,false,sizeof(mark));
        for (int i = 0; i < n; i += 2) {
            if (!mark[i]) {
                top = 0;
                if (!dfs(i)) {
                    while (top) mark[stack[--top]] = false;
                    if (!dfs(i + 1)) return false;
                }
            }
        }
        return true;
    }
};

Sat<400> sat;

const int N = 200 + 5;
bool isv[26];
int cnt[2];
char str[N];
int n,m,alpha;
int A[N],B[N];

struct Rule {
    int a,x,b,y;

    void read() {
        char q[2],w[2];
        scanf("%d%s%d%s",&a,q,&b,w); a --; b --;
        x = q[0] == 'V';
        y = w[0] == 'V';
    }
};
Rule rules[500000 + 5];

bool check(int tot) {
    sat.init();
    for (int i = 0; i < m; ++ i) {
        const Rule &t = rules[i];
        sat.add_edge(t.a << 1 | t.x,t.b << 1 | t.y);
        sat.add_edge(t.b << 1 | t.y ^ 1,t.a << 1 | t.x ^ 1);
    }
    for (int i = 0; i < tot; ++ i) {
        int c = isv[B[i]];
        sat.add_edge(i << 1 | c ^ 1,i << 1 | c);
    }
    if (cnt[0] == 0) {
        for (int i = tot; i < n; ++ i) {
            sat.add_edge(i << 1,i << 1 | 1);
        }
    } else if (cnt[1] == 0) {
        for (int i = tot; i < n; ++ i) {
            sat.add_edge(i << 1 | 1,i << 1);
        }
    }
    return sat.work(n << 1);
}

bool work() {
    for (int i = 0; i < alpha; ++ i) {
        cnt[isv[i]] ++;
    }
    for (int i = 0; i < n; ++ i) {
        B[i] = A[i];
    }
    if (check(n)) {
        puts(str);
        return true;
    }
    int p = -1;
    for (int i = n - 1; i >= 0; -- i) {
        int mask = 0;
        for (int j = A[i] + 1; j < alpha; ++ j) {
            if (mask >> isv[j] & 1) {
                continue;
            }
            mask |= 1 << isv[j];
            B[i] = j;
            if (check(i + 1)) {
                p = i;
                break;
            }
        }
        if (p != -1) break;
    }
    if (p == -1) {
        return false;
    }

    for (int i = p + 1; i < n; ++ i) {
        int mask = 0;
        bool flag = false;
        for (int j = 0; j < alpha; ++ j) {
            if (mask >> isv[j] & 1) {
                continue;
            }
            mask |= 1 << isv[j];
            B[i] = j;
            if (check(i + 1)) {
                flag = true;
                break;
            }
        }
        my_assert(flag);
    }
    for (int i = 0; i < n; ++ i) {
        printf("%c",B[i] + 'a');
    }
    puts("");
    return true;
}

int main() {
    scanf("%s",str);
    alpha = strlen(str);
    for (int i = 0; str[i]; ++ i) {
        if (str[i] == 'V') {
            isv[i] = true;
        }
    }
    scanf("%d%d",&n,&m);
    for (int i = 0; i < m; ++ i) {
        rules[i].read();
    }
    scanf("%s",str);
    for (int i = 0; i < n; ++ i) {
        A[i] = str[i] - 'a';
    }
    if (!work()) {
        puts("-1");
    }
}
