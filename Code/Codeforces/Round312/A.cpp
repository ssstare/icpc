#include <bits/stdc++.h>
using LL = long long ;

int n;
const int N = 100 + 5;
int A[N],B[N];

int work() {
    std::vector<std::pair<int,int>> X,Y;
    for (int i = 0; i < n; ++ i) {
        if (A[i] < 0) {
            X.push_back({-A[i],B[i]});
        } else {
            Y.push_back({A[i],B[i]});
        }
    }
    std::sort(X.begin(),X.end());
    std::sort(Y.begin(),Y.end());
    int now = 0;
    int ret = 0;
    for (int i = 0,j = 0; ; ) {
        if (now == 0) {
            if (i == X.size()) {
                break;
            }
            ret += X[i++].second;
        } else {
            if (j == Y.size()) {
                break;
            }
            ret += Y[j++].second;
        }
        now ^= 1;
    }
    return ret;
}

int main() {
    scanf("%d",&n);
    for (int i = 0; i < n; ++ i) {
        scanf("%d%d",A + i,B + i);
    }
    int answer = work();
    for (int i = 0; i < n; ++ i) {
        A[i] *= -1;
    }
    answer = std::max(answer,work());
    printf("%d\n",answer);
}
