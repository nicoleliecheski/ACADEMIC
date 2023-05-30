#include <stdio.h>

int main() {
    unsigned int mat[1000];
    int i, n, t, freq[1000];
    double provas[8], listas[5], notaf, media[1000], sp, sl; //declaração das variáveis
    //este programa foi feito pela aluna: Nicole Liecheski
    //na data 29/05/2023

    n = 0;
    notaf = 1;
    while(notaf>0){
        scanf("%u ", &mat[n]);
        t++;
        if(mat[n]==-1){
            for(i = 0; i < (t-1); i++){
                printf("Matricula: %u, Nota Final: %.2lf, Situacao Final: ", mat[i], media[i]);
                if((media[i]>=6.00)&&(freq[i]>=96)){
                    printf("APROVADO\n");
                }
                if((media[i]<6.00)&&(freq[i]>=96)){
                    printf("REPROVADO POR NOTA\n");
                }
                if((media[i]>=6.00)&&(freq[i]<96)){
                    printf("REPROVADO POR FREQUENCIA\n");
                }
                if((media[i]<6.00)&&(freq[i]<96)){
                    printf("REPROVADO POR NOTA E POR FREQUENCIA\n");
                }
            }
            return 0;
        }
        else{
            for(i = 0; i<8; i++){
                scanf("%lf ", &provas[i]);
                sp = sp + provas[i];
            }
            for(i = 0; i<5; i++){
                scanf("%lf ", &listas[i]);
                sl = sl + listas[i];
            }
            scanf("%lf %d", &notaf, &freq[n]);

            media[n] = (0.70*(sp/8.00))+(0.15*(sl/5.00))+(0.15*notaf);
            sp = 0.00;
            sl = 0.00;
        }
        n++;
    }
}
