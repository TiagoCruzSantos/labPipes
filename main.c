#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int x;
    scanf("%d", &x);
    if(x < 1 || x > 5){
        fprintf(stderr, "X fora da faixa\n");
        return 1;
    }
    pid_t P1 = fork();
    if(P1 == -1){
        perror("Fork falhou");
        return 1;
    }
    if(P1 == 0){

    }else{
        pid_t P2 = fork();
        if(P2 == -1){
            perror("Fork falhou");
            return 1;
        }
        if(P2 == 0){

        }else{
            
        }
    }
}