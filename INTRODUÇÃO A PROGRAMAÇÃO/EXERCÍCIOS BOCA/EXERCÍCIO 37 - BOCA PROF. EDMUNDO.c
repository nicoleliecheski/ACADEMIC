#include <stdio.h>

int main() {
    int n1, n2, n3, m1, m2, m3;
    char x, y, z; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 22/05/2023

   //Escreva três números inteiros positivos e na linha seguinte a ordem desejada de impressão dos números.

    scanf("%d %d %d\n", &n1, &n2, &n3);
    scanf("%c %c %c", &x, &y, &z);
    
    if((n1>=n2)&&(n1>=n3)){
        m1 = n1;
        if(n2>=n3){
            m2 = n2;
            m3 = n3;}
        else{
            m2 = n3;
            m3 = n2;
        }
    }
    else{
        if((n2>=n1)&&(n2>=n3)){
            m1 = n2;
            if(n1>=n3){
                m2 = n1;
                m3 = n3;}
            else{
                m2 = n3;
                m3 = n1;
            }
        }
        else{
            m1 = n3;
            if(n1>=n2){
                m2 = n1;
                m3 = n2;}
            else{
                m2 = n2;
                m3 = n1;
            }
        }
    }

    if(x == 'A'){
        printf("%d ", m3);
        if(y == 'B'){
            printf("%d %d\n", m2, m1);}
        else{
            printf("%d %d\n", m1, m2);
        }
    }
    else{
        if(x == 'B'){
            printf("%d ", m2);
            if(y == 'A'){
                printf("%d %d\n", m3, m1);}
            else{
                printf("%d %d\n", m1, m3);
            }
        }
        else{
            printf("%d ", m1);
            if(y == 'B'){
                printf("%d %d\n", m2, m3);}
            else{
                printf("%d %d\n", m3, m2);
            }    
        }
    }

    //printf("\nx-%c y-%c z-%c", x, y, z);
    //printf("\nM1-%d M2-%d M3-%d\n", m1, m2, m3);

    
return 0;
}
