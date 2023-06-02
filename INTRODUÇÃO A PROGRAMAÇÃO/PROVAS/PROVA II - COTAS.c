#include <stdio.h>

int main() {
    int qpessoas, escola, etnia, ncot = 0, l1 = 0, l2 = 0, l3 = 0, l4 = 0;
    float renda, rendaP = 0;
    scanf("%f %d %d %d", &renda, &qpessoas, &escola, &etnia);
    while(renda != -1){
        rendaP = renda/qpessoas;
        if(escola == 1){
            printf("ALUNO NAO COTISTA\n");
            ncot++;
        }
        else{
            if(rendaP <= 1405.5){
                if(etnia != 4){
                    printf("ALUNO COTISTA (L2)\n");
                    l2++;
                } else{
                    printf("ALUNO COTISTA (L1)\n");
                    l1++;
                }
            } else{
                if(etnia != 4){
                    printf("ALUNO COTISTA (L4)\n");
                    l4++;
                } else{
                    printf("ALUNO COTISTA (L3)\n");
                    l3++;
                }
            }
        }
        scanf("%f %d %d %d", &renda, &qpessoas, &escola, &etnia);
    }
    printf("TOTAL (L1): %d\nTOTAL (L2): %d\nTOTAL (L3): %d\nTOTAL (L4): %d\nTOTAL NAO COTISTA: %d\n", l1, l2, l3, l4, ncot);
return 0;
}
