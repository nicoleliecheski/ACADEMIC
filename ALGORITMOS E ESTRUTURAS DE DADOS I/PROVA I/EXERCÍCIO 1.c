#include <stdio.h>
#include <string.h>

int M[100][100], c = 0, N;

void matriz(int n, int x, int y){ // função para implementar a recursividade
    int i, j;
    for (i = c; i < n; i++) // há um for para cada lado da camada (essas que formam um quadrado)
    {
        M[c][i] = x;
    }

    // esses for de impressão são para testar se a matriz estava sendo preenchida corretamente

    /*for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }  
    printf("\n");*/

    for (i = c; i < n; i++)
    {
        M[n-1][i] = x;
    }

    /*for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }  
    printf("\n");*/

    for (i = c; i < n; i++)
    {
        M[i][c] = x;
    }

    /*for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }  
    printf("\n");*/

    for (i = c; i < n; i++)
    {
        M[i][n-1] = x;
    }

    /*for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }  
    printf("\n");*/
    
    if(n-1 != 0){ // aqui eu testo pra ver se a matriz já não chegou ao final
        c++; // implementado a contagem das camadas
        matriz(n-1, y, x); //chamo a funçao de novo dessa vez com x e y trocado para haver a alternacia dos valores 
    }
}

int main (){
    int x, y, n, i, j;

    memset(M, -1, sizeof(M)); // definindo todos os espaços da matriz como -1 (serve para verificar se aquela posição já foi preenchida)

    scanf("%d %d %d", &n, &x, &y); //  entrada

    N = n; // apenas aqui por causa dos for de impressao testes na funcao

    matriz(n, x, y); // chamo a a funçao pela primeira vez

    

    for (i = 0; i < n; i++) // impressao da matriz final
    {
        for (j = 0; j < n; j++)
        {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }   

    return 0;
}
