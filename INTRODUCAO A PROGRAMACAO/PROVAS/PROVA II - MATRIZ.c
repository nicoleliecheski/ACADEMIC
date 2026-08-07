#include <stdio.h>

int main() {
    int N, i = 1, j = 0;
    scanf("%d", &N);
    while(i <= N){
        j = i+1;
        while(j <= N){
            if(j == N){
                printf("(%d, %d)", i, j);
            } else{
                printf("(%d, %d) - ", i, j);
            }
            j++;
        }
        printf("\n");
        i++;
    }
return 0;
}
