#include <bits/stdc++.h>

int A[100000],n;

int main() {
    scanf("%d",&n);
    bool flag = true;
    for (int i = 0; i < n; ++ i) {
        scanf("%d",A + i);
        while (A[i] % 2 == 0) A[i] /= 2;
        while (A[i] % 3 == 0) A[i] /= 3;
        if (i && A[i] != A[i - 1]) flag = false;
    }
    puts(flag ? "YES" : "NO");
}
