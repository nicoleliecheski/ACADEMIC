#include <stdio.h>
#include <math.h>

int main() {
    float x1, y1, x2, y2; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 02/05/2023

   //Escreva as coordenadas dos dois pontos A e B.

    scanf("%f\n%f\n%f\n%f", &x1, &y1, &x2, &y2);
    printf("A DISTANCIA ENTRE A e B = %.2f\n", sqrt(pow(x2-x1,2)+pow(y2-y1,2)));

return 0;
}
