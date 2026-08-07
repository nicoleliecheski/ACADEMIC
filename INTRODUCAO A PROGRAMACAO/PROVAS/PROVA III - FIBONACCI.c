#include <stdio.h>

int main() {
    int N, i, F1, F2, V[1000], flag = 1; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 29/06/2023

    scanf("%d %d %d", &N, &F1, &F2);

    for(i = 0; i < N; i++){
        if(i < N-1){
            scanf("%d ", &V[i]);
        } else{
            scanf("%d", &V[i]);
        }
    }

    for(i = 0; i < N-2; i++){
        if((F1+F2) == V[i+2]){
            printf("OK\n");
            flag = 0;
            F1 = V[i+1];
            F2 = V[i+2];
        } else{
            printf("Nao e Fibonacci\n");
            i = N;
            flag = 1;
        }
    }
    if(flag == 0){
        printf("OK\nOK\n");
        printf("A serie informada e de Fibonacci\n");
    }


return 0;
}

