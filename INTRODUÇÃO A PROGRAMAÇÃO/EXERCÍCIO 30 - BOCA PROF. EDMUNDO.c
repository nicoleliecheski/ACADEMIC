#include <stdio.h>

int main() {
    float a, b, c, ordem[3]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 11/05/2023

   //Escreva três valores reais.

    scanf("%f\n%f\n%f", &a, &b, &c);
    // testes pra a
    if ((a>b)&&(a>c)){
        ordem[0] = a; }
    else {
        if ((a>=b)||(a>=c)){
           ordem[1] = a;}
        else {
            ordem[2] = a;}
        if ((b==a)||(a==c)){
            ordem[1] = a;}
        if ((a==b)&&(a==c)){
            ordem[0] = a;}
        
    }
    
    // testes pra b
    if ((b>a)&&(b>c)){
        ordem[0] = b; }
    else {
        if ((b>a)||(b>c)){
            ordem[1] = b;}
        else {
            ordem[2] = b;}
        if ((b==a)||(b==c)){
            ordem[2] = b;}
        if ((a==b)&&(b==c)){
            ordem[1] = b;}
    }
    
    //testes pra c
    if ((c>b)&&(c>a)){
        ordem[0] = c; }
    else {
        if ((c>=b)||(c>=a)){
            ordem[1] = b;}
        else {
            ordem[2] = c;}
        if ((c==a)||(b==c)){
            ordem[2] = c;}
        if ((c==b)&&(a==c)){
            ordem[2] = c;}
    }
    
    printf("%.2f, %.2f, %.2f\n", ordem[2], ordem[1], ordem[0]);

return 0;
}
