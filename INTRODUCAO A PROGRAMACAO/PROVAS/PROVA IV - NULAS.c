#include <stdio.h>

int main()
{
    int n, m, M[500][500], i, j, flag, SOMA; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 03/08/2023

    //Dada uma matriz Am×n verificar quais linhas e colunas são nulas.

    scanf("%d %d", &n, &m);

    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            scanf("%d", &M[i][j]);
        }
    }

    for(i = 0; i < n; i++){
        flag = 0;
        for(j = 0; j < m; j++){
            if(M[i][j] != 0){
                flag = 1;
            }
        }
        if(flag == 0){
            printf("LINHA %d\n", i+1);
        }
    }

    for(j = 0; j < m; j++){
        flag = 0;
        for(i = 0; i < n; i++){
            if(M[i][j] != 0){
                flag = 1;
            }
        }
        if(flag == 0){
            printf("COLUNA %d\n", j+1);
        }
    }
    
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            SOMA = SOMA + M[i][j];
        }
    }

    printf("SOMA: %d\n", SOMA);


    return 0;
}
