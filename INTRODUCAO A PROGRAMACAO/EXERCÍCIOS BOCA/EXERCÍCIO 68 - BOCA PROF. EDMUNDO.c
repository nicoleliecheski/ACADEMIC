#include <stdio.h>
#include <string.h>

int main() {

    int N, i, c;
    char P1[51][100], P2[51][100]; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 29/06/2023

    //Implemente um programa denominado combinador, que recebe duas strings e deve combiná-las,
    //alternando as letras de cada string, começando com a primeira letra da primeira string,
    //seguido pela primeira letra da segunda string, em seguida pela segunda letra da primeira
    //string, e assim sucessivamente. As letras restantes da cadeia mais longa devem ser adicionadas
    //ao fim da string resultante e retornada.
    //A entrada contém vários casos de teste. A primeira linha contém um inteiro N que indica a quantidade
    //de casos de teste que vem a seguir. Cada caso de teste é composto por uma linha que contém duas cadeias
    //de caracteres. Cada cadeia de caracteres contém entre 1 e 50 caracteres inclusive.

    scanf("%d", &N);

    for(i = 0; i < N; i++){
        scanf("%s", P1[i]);
        scanf("%s", P2[i]);
    }
    for(i = 0; i < N; i++){
        c = 0;
        while((P1[i][c] != '\0')||(P2[i][c] != '\0')){
            printf("%c%c", P1[i][c], P2[i][c]);
            c++;
        }
        printf("\n");
    }
    
    return 0;
}
