#include <stdio.h>

int main()
{
    int N, i, j, V[5000], freq[101], cont  = 0; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 18/07/2023
    
    //Elabore um programa que conte o número total de elementos únicos em um vetor de números inteiros.
    //A entrada contém duas linhas. A primeira, contém um valor inteiro n < 5000 que corresponde ao
    //número de elementos que aparecem na segunda linha. A segunda linha contém n valores inteiros, separados
    //entre si por um espaço.
    
    scanf("%d", &N);
    
    for(i = 0; i < N; i++){
        if(i < N-1){
            scanf("%d ", &V[i]);
        } else{
            scanf("%d", &V[i]);
        }
        freq[V[i]]++;
    }
    
    for(i = 0; i < 101; i++){
        if(freq[i] == 1){
            cont++;
        }
    }

    printf("%d\n", cont);

    return 0;
}
