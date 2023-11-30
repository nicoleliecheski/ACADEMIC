#include <stdio.h>
#include <string.h>

long long int E[1000];
char  R[25][1000];
int c = 0;

long long int BI(int n){
    if(n == 0){
        return 0;
    } 
    if(n%2 == 0){
        //printf("0 ");
        char a[25] = "0";
        strcat(a, R[c]);
        strcpy(R[c], a);
    } else if(n%2 == 1){
        //printf("1 ");
        char a[25] = "1";
        strcat(a, R[c]);
        strcpy(R[c], a);
    }
    return BI(n/2);
}

int main(){
    int k, i, a;

    scanf("%d", &k);

    for (i = 0; i < k; i++)
    {
        scanf("%d", &E[i]);
    }

    for (i = 0; i < k; i++)
    {
        strcpy(R[c], "");
        BI(E[i]);
        //printf("\n");
        c++;
    }

    for (i = 0; i < k; i++)
    {
        printf("%s\n", R[i]);
    }
    
}
