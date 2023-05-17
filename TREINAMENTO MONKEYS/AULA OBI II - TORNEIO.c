#include <stdio.h>

int main(){

    int res, g;

    for (int i = 0; i < 12; i++){
        char R;
        scanf("%c", &R);
        if (R == 'V'){
            res++;
        }
        //printf("%d\n", i);
    }

    if (res>4){
        g = 1;}
    else {
        if (res>2){
            g = 2;}
        else {
            if (res == 0){
                g = -1;}
            else{
                g = 3;
            } 
        }
    }
    

    printf("%d", g);

    return 0;
}
