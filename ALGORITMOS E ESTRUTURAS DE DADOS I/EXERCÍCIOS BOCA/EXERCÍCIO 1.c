#include <stdio.h>
#include <math.h>

int main(){
    int A, b, ans;
    double a;

    scanf("%d", &A);

    a = (A-1986)/76.00;
    b = ceil(a);
    
    //printf("%lf %d\n", a, b);
    
    
    if((a == 1) || (a == -1) || (a == 0)){
        //printf("//\n");
        ans = A + 76;
    } else{
        ans = 1986 + 76 * b;
    }
    
    if(ans%4 == 0){
        if(ans%100 == 0){
            if(ans%400 == 0){
                ans -= 1;
            }
        } else{
            ans -= 1;
        }
        
    }

    printf("%d\n", ans);

    return 0;
}
