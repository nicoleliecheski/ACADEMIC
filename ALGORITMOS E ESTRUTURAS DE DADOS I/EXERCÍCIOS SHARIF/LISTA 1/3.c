#include <stdio.h>

long long int ack(long long int m, long long int n){
    if(m == 0){
        return n+1;
    } else if(n == 0){
        return ack(m-1, 1);
    }
    
    return ack(m-1, ack(m, n-1));
}

int main(){
    int m, n;

    scanf("%d %d", &m, &n);
    
    printf("%lld\n", ack(m, n));
}
