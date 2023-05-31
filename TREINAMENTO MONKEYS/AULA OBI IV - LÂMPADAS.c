#include <stdio.h>

int iA, iB, fA, fB, t;

c1(){
    if(iA == 1){
        iA = 0;
    } else{
        iA = 1;
    }
}

c2(){
    if(iA == 1){
        iA = 0;
    } else{
        iA = 1;
    }
    if(iB == 1){
        iB = 0;
    } else{
        iB = 1;
    }
}

int main(){

    scanf("%d %d %d %d", &iA, &iB, &fA, &fB);

    if((iA == fA)&&(iB ==fB)){
        t = 0;
    } else{
        if (iB != fB){
            c2();
            t++;
            if(iA != fA){
                c1();
                t++;
            }
        }
        if(iA != fA){
            c1();
            t++;
        }
    }

    printf("%d\n", t);

    return 0;
}
