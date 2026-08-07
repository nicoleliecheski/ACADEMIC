#include <stdio.h>

int main() {

    int K, N, i, Al[1000], cont = 0; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 17/07/2023

    //Um professor X tem uma turma de N alunos. Frustrado com a falta de disciplina, ele decide cancelar a
    //aula se menos de K alunos estão presentes quando a aula começa. Dado o tempo de chegada de cada aluno,
    //determinar se a aula é cancelada. Caso a aula não seja cancelada, imprima uma lista com os alunos que
    //chegaram antes do início da aula em ordem contrária à mostrada na entrada.
    //A primeira linha apresenta dois números inteiros separados por um espaço: N (alunos da turma) e K
    //(mínimo de presenças para que a aula não seja cancelada), com 0 ≤ N, K, ≤ 1000. Na segunda linha há
    //N inteiros separados por espaços (A1 , A2 ,... , An ) descrevendo os tempos de chegada para cada aluno.
    //Suponha que esta ordem seja a mesma da lista de presença do professor, com o primeiro aluno descrito na
    //entrada sendo o aluno 1 e assim por diante. Nota: horários de chegada não-positivos (Ai ≤ 0) indicam que
    //o aluno chegou cedo ou na hora; horários de chegada positivos (Ai>0) indicam o aluno chegou Ai minutos
    //tarde.

    scanf("%d %d", &N, &K);

    for(i = 0; i < N; i++){
        if(i < N-1){
            scanf("%d ", &Al[i]);
        } else{
            scanf("%d", &Al[i]);
        }
        if(Al[i] <= 0){
            cont++;
        }
    }    
    
    if(cont < K){
        printf("SIM\n");
    } else{
        printf("NAO\n");
        for(i = N-1; i > -1; i--){
            if(Al[i] <= 0){
                printf("%d\n", i+1);
            }
        }
    }
  
    return 0;
}
