#include <stdio.h>
#include <math.h>

int main() {

    int N, i;
    float x[1000], y[1000], z[1000], d[1000]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 19/06/2023

    //Faça um programa que leia vários pares de pontos e calcule a distância entre eles.
    //A entrada consiste de várias linhas. A primeira linha apresenta um número de pontos N, com 2 ≤ N ≤
    //1000. As N linhas seguintes apresentam pontos no espaço na forma x y z, com x, y e z números reais tais
    //que -1000 ≤ x, y, z ≤ 1000. Faça um programa que calcule a distância entre dois pontos consecutivos nesta
    //lista. Note que, com exceção do primeiro e último valor de entrada, todos os pontos serão utilizados duas
    //vezes, uma para o cálculo de distância com o ponto que veio antes na lista e outra para o ponto que veio
    //depois.

    scanf("%d", &N);

    for(i = 0; i < N; i++){
        scanf("%f %f %f", &x[i], &y[i], &z[i]);
        if(i != 0){
            d[i] = sqrt(((x[i] - x[i-1])*(x[i] - x[i-1])) + ((y[i] - y[i-1])*(y[i] - y[i-1])) + ((z[i] - z[i-1])*(z[i] - z[i-1])));
        }
    }

    for(i = 1; i < N; i++){
        printf("%.2f\n", d[i]);
    }

    return 0;
}
