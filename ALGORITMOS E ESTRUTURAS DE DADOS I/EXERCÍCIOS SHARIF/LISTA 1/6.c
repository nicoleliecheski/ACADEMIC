#include <stdio.h>
 
int N;
 
long double f2(int n){
    if((N%2 == 0) && (n >= N)){
        return 1;
    } else if((N%2 != 0) && n == N){
        return n;
    } 
 
    return n*f2(n+2);
}
 
int main(){
 
    scanf("%d", &N);
    
    printf("%.0llf\n", f2(1));
}
