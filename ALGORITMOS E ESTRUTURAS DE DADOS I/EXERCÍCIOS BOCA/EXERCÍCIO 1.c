#include <stdio.h>
#include <math.h>

int main(){
    int A, b;
    double ans, a;

    scanf("%d", &A);

    a = ceil((A-1986)/76.00);
    b = a;
    
    //printf("%lf %d\n", a, b);
    
    
    if(b%76 == 0){
        ans = A + 76;
    } else{
        ans = 1986 + 76 * b;
    }

    printf("%.0lf\n", ans);

    return 0;
}
