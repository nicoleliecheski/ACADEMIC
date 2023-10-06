#include <stdio.h>

int main(){
    int N1, N2, res, lim;
    char op;
    
    scanf("%d\n%d %c %d", &lim, &N1, &op, &N2);
    
    if(op == '+'){
        res = N1 + N2;
    } else if(op == 'x'){
        res = N1*N2;
    }
    
    if(res > lim){
        printf("overflow\n");
    } else{
        printf("no overflow\n");
    }
    
    return 0;
}
