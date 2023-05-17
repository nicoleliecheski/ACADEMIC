#include <cstdio>
#include <algorithm>

int main(){

    int N;
    long num[100000], mat[100000];

    for(int i = 0; i < N; ++i){
        mat[i] = 0;
    }

    scanf("%d", &N);
    for(int i = 0; i < N; ++i){
        scanf("%ld", &num[i]);
        }

    std::sort(num, num+N);
    int j = 0;

    for(int i = 0; i < N; ++i){
        if (i == 0){
            mat[j] = num[i];
            j = 1;
        } else if(num[i]!= mat[j-1]){
            mat[j] = num[i];
            j++;
        }
    }

    printf("%d\n", j);


    return 0;
}
