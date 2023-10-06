#include <stdio.h>

int main(){
    int A, B;
    
    scanf("%d %d", &A, &B);
    
    //printf("%d\n", (A%1000) - A%100);
    
    A = (A%10)*100 + ((A%100) - A%10) + (((A%1000) - A%100)/100);
    B = (B%10)*100 + ((B%100) - B%10) + (((B%1000) - B%100)/100); 
    
    if(A > B){
        printf("%d", A);
    } else{
        printf("%d", B);
    }
    
    
    
    
    return 0;
}
