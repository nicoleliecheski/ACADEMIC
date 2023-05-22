#include <stdio.h>

int main() {
    int a, b, c, d, res; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 11/05/2023

   //Escreva quatro números inteiros;

    scanf("%d\n%d\n%d\n%d", &a, &b, &c, &d);
    if ((a>b)&&(a>c)&&(a>d)){
        res = b+c+d;}
    if ((b>a)&&(b>c)&&(b>d)){
        res = a+c+d;}
    if ((c>b)&&(c>a)&&(c>d)){
        res = b+a+d;}
    if ((d>b)&&(d>c)&&(d>a)){
        res = b+c+a;}
        
    printf("%d\n", res);
return 0;
}
