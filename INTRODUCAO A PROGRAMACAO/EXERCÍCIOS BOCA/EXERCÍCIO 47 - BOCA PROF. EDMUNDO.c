#include <stdio.h>

int main() {

    int V[5000], N, i, soma; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 25/05/2023

	//Elabore um programa que armazene n valores em um vetor, e depois imprima a soma de todos os
	//elementos inseridos.
	//A entrada contém duas linhas. A primeira, contém um valor inteiro n < 5000 que corresponde ao
	//número de elementos que aparecem na segunda linha. A segunda linha contém n valores inteiros, separados
	//entre si por um espaço.

    scanf("%d", &N);

    for(i = 0; i < N; ++i){
        V[i] = 0;}
    for(i = 0; i < N; i++){
        if(i<N-1){
            scanf("%d ", &V[i]);
            soma = soma + V[i];
        }
        else{
            scanf("%d", &V[i]);
            soma = soma + V[i];
        }
    }

    printf("%d\n", soma);
  
    return 0;
}
