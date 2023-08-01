#include <stdio.h>

int main()
{
    int larg, alt, largB, valB, i, j, contL, contC, flagL, flagC; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 31/07/2023

    //Faça um programa que gere uma matriz de zeros, de tamanho definido pelo usuário, 
    //de no máximo 100×100, com uma borda de largura k de valor x. O programa deve ler
    //quatro números inteiros, os dois primeiros relacionados à largura e altura da matriz,
    //o terceio a largura da borda e o por último o valor da borda.

    scanf("%d %d %d %d", &larg, &alt, &largB, &valB);

    contL = largB;
    flagL = alt - 2*largB;

    printf("P2\n%d %d\n255\n", larg, alt); 

    for(i = 0; i < alt; i++){
        contC = largB;
        flagC = larg - 2*largB;
        for(j = 0; j < larg; j++){
            if(contL > 0){
                printf("%d ", valB);
            } else{
                if(contC > 0){
                    printf("%d ", valB);
                    contC--;
                } else{
                    if((flagC > 0) && (flagL > 0)){
                        printf("0 ");
                        flagC--;
                    } else{
                        printf("%d ", valB);
                    }
                }
            }
        }
        if(flagC == 0){
            flagL--;
        }
        contL--;
        printf("\n");
    }

    return 0;
}
