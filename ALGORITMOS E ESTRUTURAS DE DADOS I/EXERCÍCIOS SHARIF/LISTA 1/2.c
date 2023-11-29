#include <stdio.h>
#include <string.h>

long long DP[1050];

long long int fib(long long int a){
    if(DP[a] != -1){
        return DP[a];
    } else if(a == 0){
        DP[a] = a;
        return 0;
    } else if(a == 1){
        DP[a-1] = 0;
        DP[a] = a;
        return 1;
    }
    
    return DP[a] = fib(a-1) + fib(a-2);
}

int main(){
    int n, i;

    memset(DP, -1, sizeof(DP));

    scanf("%d", &n);

    fib(n);

    for (i = 0; i <= n; i++)
    {
        printf("%lld ", DP[i]);
    }
    

    printf("\n");
}
