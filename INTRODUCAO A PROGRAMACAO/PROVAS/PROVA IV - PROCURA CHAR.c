#include <stdio.h>
#include <string.h>

int N, f, i, j, r, V[20]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 03/08/2023

int procC(char C, char frase[500]){
    f = 0;
    j = 0;
    char *letra;
    letra = strchr(frase, C);
    if(letra == NULL){
        return -1;
    } else{
        while(f != -1){
            if(frase[j] == *letra){
                r = j;
                f = -1;
            }
            j++;
        }
        return r;
    }
}

int main()
{
    //Escreva um programa para ler várias linhas na entrada. Cada linha contém um caractere seguido por
    //um espaço e por uma string. Para cada linha, o programa deve chamar uma função do tipo int que receba
    //como primeiro parâmetro o caractere lido e como segundo parâmetro a string lida. A função deve retorna o
    //índice do vetor onde o caractere aparece pela primeira vez na string. Se o caractere não aparece na string, a
    //função deve retornar -1.

    char Cm, fraseM[500], imp[20];

    scanf("%d\n", &N);

    for(i = 0; i < N; i++){
        Cm = getchar();
        imp[i] = Cm;
        gets(fraseM);
        V[i] = procC(Cm, fraseM);
    }

    for(i = 0; i < N; i++){
        if(V[i] == -1){
            printf("\nCaractere %c nao encontrado.\n", imp[i]);
        } else{
            printf("\nCaractere %c encontrado no indice %d da string.\n", imp[i], V[i] - 1);
        }
    }

    return 0;
}
