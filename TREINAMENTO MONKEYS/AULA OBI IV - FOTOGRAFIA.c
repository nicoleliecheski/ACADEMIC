#include <stdio.h>

int main(){

    int N, A, L, X[100], Y[100], tam[100] = {0}, pos = 1, res = -1;

    scanf("%d %d\n%d", &A, &L, &N);

    for(int i = 0; i<N; i++){
        scanf("%d %d", &X[i], &Y[i]);
    }

    for(int i = 0; i<N; i++){
        if((A > X[i])&&(A > Y[i])){
            tam[i] = -1;
            //printf("//1\n");
        } else {
            if((L > X[i])&&(L > Y[i])){
                tam[i] = -1;
                //printf("//2\n");
            } else {
                if(X[i]*Y[i] < A*L){
                    tam[i] = -1;
                } else{
                    tam[i] = X[i]*Y[i];
                    //printf("//3\n");
                    if(pos == 1){
                        //printf("//6\n");
                        pos = i;
                        res = pos + 1;
                    } else {
                        //printf("posdif0\n");
                        if (tam[i]<tam[i-1]){
                            //printf("//4\n");
                            if(tam[i] < tam[pos]){
                                pos = i;
                                res = pos + 1;
                                //printf("//5\n");
                            }
                        }
                    }
                }
            }
        }

    }

    printf("%d\n", res);
    

    return 0;
}
