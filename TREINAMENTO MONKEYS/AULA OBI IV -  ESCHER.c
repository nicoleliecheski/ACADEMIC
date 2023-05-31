#include <stdio.h>

int main(){

    int N, A[10000], flag = 0, res;

    scanf("%d", &N);

    for(int i = 0; i<N; i++){
        if(i<N-1){
            scanf("%d ", &A[i]);}
        else{
            scanf("%d", &A[i]);
        }
    }

    for(int i = 0; i<N; i++){
        if((A[0]+A[N-1])==(A[1]+A[N-2])){
            flag = 0;
            res = A[0]+A[N-1];
            if((A[i]+A[N-i-1])== res){
                flag = 0;
            } else{
                flag = 1;
                i = N;
            }
        } else{
            flag = 1;
            i = N;
        }
    }

    if(flag == 1){
        printf("N\n");
    } else{
        printf("S\n");
    }
    

    return 0;
}
