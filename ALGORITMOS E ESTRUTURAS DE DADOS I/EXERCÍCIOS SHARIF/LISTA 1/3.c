#include <stdio.h>
#include <string.h>

long long DP[1000][1000];

long long int ack(long long int m, long long int n){
    if(DP[m][n] != -1){
        return DP[m][n];
    } else if(m == 0){
        return DP[m][n] = n+1;
    } else if((n == 0) && (m > 0)){
        return DP[m][n] = ack(m-1, 1);
    }
    
    return DP[m][n] = ack(m-1, ack(m, n-1));
}

int main(){
    int m, n;

    memset(DP, -1, sizeof(DP));

    scanf("%d %d", &m, &n);
    
    printf("%lld\n", ack(m, n));
}
