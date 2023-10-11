#include <stdio.h>

char pal[110][5000];
int cont[110];

int main()
{
    int N, num[110], i , j = 0;
    
    scanf("%d", &N);
    
    for(i = 0; i < N; i++){
        scanf("%d", &num[i]);
    }
    
    for(i = 0; i < N; i++){
        j = 0;
        while(num[i] != 0){
            switch (num[i]%4){
                case 0 :
                    pal[i][j] = 'A';
                    break;
                case 1 :
                    pal[i][j] = 'C';
                    break;
                case 2 :
                    pal[i][j] = 'G';
                    break;
                case 3 :
                    pal[i][j] = 'T';
                    break;
            }
            num[i] = num[i]/4;
            j++;
        }
        cont[i] = j;
    }
    
    for(i = 0; i < N; i++){
        for(j = cont[i]; j >= 0 ; j--){
            printf("%c", pal[i][j]);
        }
        printf("\n");
    }

    return 0;
}
