#include <stdio.h>

int main(){
    int num[1000], res[1000], N;
    
    scanf("%d", &N);
    
    for(int i = 0; i < N; i++){
        scanf("%d", &num[i]);
    }
    
    for(int i = 0; i < N; i++){
        if(num[i] > 999){
            res[i] = (num[i]%10)*1000 + ((num[i]%100) - num[i]%10)*10 + ((num[i]%1000) - num[i]%100)/10 + ((num[i]%10000) - num[i]%1000)/1000;
        } else if(num[i] > 99){
            res[i] = (num[i]%10)*100 + ((num[i]%100) - num[i]%10) + ((num[i]%1000) - num[i]%100)/100;
        } else if(num[i] > 9){
            res[i] = (num[i]%10)*10 + ((num[i]%100) - num[i]%10)/10;
        } else {
            res[i] = num[i];
        }
    }
    
    for(int i = 0; i < N; i++){
        if(num[i] == res[i]){
            printf("yes ");
        } else{
            printf("no ");
        }
    }
    
    return 0;
}
