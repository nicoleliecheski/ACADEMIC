#include <stdio.h>

int r = 0, i = 0;

int rev(int n){
    if(n <= 0){
        return 0;
    } else{
        r = r*10 + n%10;
        n = (n - n%10)/10;
    }
    
    return rev(n);
}

int main(){
    int n;

    scanf("%d", &n);

    rev(n);
    
    printf("%d\n", r);
}
