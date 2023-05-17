#include <cstdio>
#include <algorithm>

int main(){
    long i, j, a, res = 0, num, chocolates[100000];

    scanf("%ld", &num); // lendo os valores
    for (i = 0; i < num; ++i){
        scanf("%ld", &chocolates[i]);    
    }

    std::sort(chocolates, chocolates+num); // organizando eles de forma crescente
    std::reverse(chocolates, chocolates+num); // revertendo pra descrecente

    /*for (i = 0; i < num; ++i){ // organizando eles de forma descrecente (não deu certo porque ultrapassa 1s)
        for (j = i+1; j < num; ++j){
            if (chocolates[i] < chocolates[j]) {
                a =  chocolates[i];
                chocolates[i] = chocolates[j];
                chocolates[j] = a;
            }
        }
    }*/

    for (long n = 0; n < num; ++n){ // calculo dos valores
        if ((n+1)%3!=0){
            res = res + chocolates[n];
        }   
    }
    
    printf("%ld", res);

    return 0;
}
