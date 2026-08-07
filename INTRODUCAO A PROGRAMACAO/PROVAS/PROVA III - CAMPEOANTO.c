#include <stdio.h>

int main() {
    int N, i, c, aux, E[100], P[100]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 29/06/2023

    scanf("%d", &N);

    for(i = 0; i < N; i++){
        scanf("%d", &E[i]);
    }

    for(i = 0; i < N; i++){
        scanf("%d", &P[i]);
    }

    if((N < 6)||(N > 100)){
        printf("CAMPEONATO INVALIDO!\n");
    } else{
        for(i = 0; i < N; i++){
            for(c = i+1; c < N; c++){
                if(P[i] < P[c]){
                    aux = P[c];
                    P[c] = P[i];
                    P[i] = aux;
                    aux = E[c];
                    E[c] = E[i];
                    E[i] = aux;
                } else if(P[i] == P[c]){
                    if(E[i] > E[c]){
                        aux = P[i];
                        P[i] = P[c];
                        P[c] = aux;
                        aux = E[i];
                        E[i] = E[c];
                        E[c] = aux;
                    }/* else{
                        aux = P[c];
                        P[c] = P[i];
                        P[i] = aux;
                        aux = E[c];
                        E[c] = E[i];
                        E[i] = aux;
                    }*/
                }
            }
        }
        printf("%d\n", N);
        for(i = 0; i < N; i++){
            printf("%d %d %d\n", i+1, E[i], P[i]);
        }
    }

return 0;
}
