#include <stdio.h>

int main() {
    double massa, ac, seg; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 02/05/2023

    //Escreva o valor do preço de fábrica do carro, do percentual do distribuidor e do percentual de impostos.

    scanf("\n%lf\n%lf\n%lf", &massa, &ac, &seg);
    printf("VELOCIDADE = %.2lf\nESPACO PERCORRIDO = %.2lf\nTRABALHO REALIZADO = %.2lf\n", ac*seg*3.6, (ac*seg*seg)/2, ((massa*1000)*(ac*seg*ac*seg))/2);

return 0;
}
