Copy
#include <stdio.h>
 
int main() {
    int M, N, S, c;
    scanf("%d\n%d\n%d", &N, &M, &S);
 
    for (int i = N; i <= M; i++) {
        int d = 0;
        for (int f = i; f > 0; f = f/10) {
            d = d+f%10;
        }
        if (d == S) {
            c = i;
        }
    }
    if (c != 0) {
        printf("%d\n", c);
    } else {
        printf("-1\n");
    }
 
    return 0;
 
}
