#include <stdio.h>

int main() {

    int V[10000], N, i, maior[10000], pos[10000], t; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 25/05/2023

  //Faça um programa que receba vários vetores e informe para cada um deles qual o maior elemento e o
  //indice (da primeira ocorrência) em que encontra-se tal elemento.
  //O programa possui vários casos de testes. A primeira de cada caso contem um inteiro N, 1 < N ≤ 10000,
  //representando o tamanho do vetor. A segunda linha conterá N inteiros entre 0 e 1000, representando os N
  //elementos do vetor. A entrada termina quando N=0.

    t = 0;
    N = 1;
    while(N>0){
        scanf("%d", &N);
        t++;
        if(N==0){
            for(i = 1; i < t; i++){
                printf("%d %d\n", pos[i], maior[i]);
            }
            return 0;
        }
        else{
            for(i = 0; i < N; ++i){
            V[i] = 0;}
            for(i = 0; i < N; i++){
                if(i<N-1){
                    scanf("%d ", &V[i]);
                    if((V[i]>V[i-1])&&(V[i]>maior[t])){
                        maior[t] = V[i];
                        pos[t] = i;}
                }
                else{
                    scanf("%d", &V[i]);
                    if((V[i]>V[i-1])&&(V[i]>maior[t])){
                        maior[t] = V[i];
                        pos[t] = i;}
                }
            }
        }
    }


    return 0;
}
