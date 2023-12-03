#include <stdio.h>

const int T = 1e4+100; // constante para o tamanho possivel do array

int R[15]; // array pra guardar as respostas finais

int main(){

    int i, j, k, N, y[15], max = -1, A[15][T]; // declaracao das variaveis dos For's (i,j,k), N pro numero de casos teste
                                            // y para o tamanho do array de cada caso teste, A para guardar a sequencia
                                            //numerica de cada caso teste e max é a varialvel de verificaçao para saber qual o valor maximo

    scanf("%d", &N); //input do numero de casos teste
    
    for (i = 0; i < N; i++)// For para o input
    {
        scanf("%d", &y[i]);
        for (j = 0; j < y[i]; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }
    
    for (i = 0; i < N; i++) //esse primeiro for é de cada caso teste
    {
        for (j = 0; j < y[i]; j++) //esse segundo é para percorrer cada elemento da sequencia
        {
            for (k = j+1; k < y[i]; k++) //esse terceiro é para calcular o par de cada elemento com os elementos seguintes a ele
            {
                if(A[i][j]+A[i][k] > max){ //aqui eu testo se a soma desse par é maior que o maximo atual, se for ela se torno o
                    max = A[i][j]+A[i][k]; //novo maximo e o numero de maximos daquele caso teste se torna 1
                    R[i] = 1;
                } else if(A[i][j]+A[i][k] == max){ //aqui caso a soma nao for maior que o maximo, mas for igual a ele,
                    R[i]++;                        //eu adiciono 1 ao numero de maximos daquele caso teste
                }
            }
        }
        max = -1; //aqui eu redefino a variavel max para -1 para que ela possa ser usada em todos os casos teste
    }

    for (i = 0; i < N; i++) //For de output
    {
        printf("%d\n", R[i]);
    }

}
