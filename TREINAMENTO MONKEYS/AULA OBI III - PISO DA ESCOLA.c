#include <stdio.h>

int main(){

    int L, C, tp1, tp2;

    scanf("%d\n%d", &L, &C);

    tp1 = (L*C)*2 - 1;
    tp2 = (L-1)*2 + (C-1)*2;
    tp1 = tp1 - tp2/2;

    printf("%d\n%d", tp1, tp2);

    return 0;
}
