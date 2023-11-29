#include <stdio.h>

int i;

void nat(int a){
    if(a < i){
        a += 1;
        printf("%d ", a);
        nat(a);
    }
}

int main(){
    int n;

    scanf("%d", &n);
    
    i = n;
    
    nat(0);

    printf("\n");
}
