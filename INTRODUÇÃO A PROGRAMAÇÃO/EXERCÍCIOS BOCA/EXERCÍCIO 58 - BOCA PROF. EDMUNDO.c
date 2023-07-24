#include <stdio.h>

int main()
{
    int T, i, j, dig[11], val[1000], soma = 0, c = 9, d1, d2; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 18/07/2023
    
    //Verificar se o CPF dado na entrada é válido ou não.
    //A entrada conterá uma linha com um inteiro T, que indica o número de casos de testes. Esta linha é
    //seguida por T linhas, cada uma contendo uma sequência de 11 dígitos decimais, separados entre sis por um
    //espaço. Após o último dígito decimal segue o caractere de quebra de linha.
    //Para cada candidato a CPF da entrada, escreva "CPF valido", se ele for um CPF válido e, escreva "CPF
    //invalido", em caso contrário.
    
    scanf("%d", &T);
    
    for(i = 0; i < T; i++){
        for(j = 0; j < 11; j++){
            if(j < 10){
                scanf("%d ", &dig[j]);
            } else{
                scanf("%d", &dig[j]);
            }
        }
        soma = 0;
        for(j = 0; j < 9; j++){
            //printf("%d = %d + %d*%d\n", soma, soma, dig[j], j+1);
            soma = soma + dig[j]*(j+1);
        }
        if((soma%11) == 10){
            d1 = 0;
        } else{
            d1 = soma%11;
        }
        
        soma = 0;
        
        for(j = 0; j < 9; j++){
            soma = soma + dig[j]*c;
            c--;
        }
        c = 9;
        if((soma%11) == 10){
            d2 = 0;
        } else{
            d2 = soma%11;
        }
        
        //printf("\nd1 = %d d2 = %d\n", d1, d2);
        
        if((d1 != dig[9])||(d2 != dig[10])){
            val[i] = 1;
        }
    }
    
    for(i = 0; i < T; i++){
        if(val[i] == 1){
            printf("CPF invalido\n");
        } else{
            printf("CPF valido\n");
        }
    }

    return 0;
}
