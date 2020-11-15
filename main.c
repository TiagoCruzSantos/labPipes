#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int x;
    scanf("%d", &x);
    if(x < 1 || x > 5){
        fprintf(stderr, "X fora da faixa\n");
        return 2;
    }
    int pipeP0P1[2];
    int pipeP1P2[2];
    if(pipe(pipeP0P1) < 0){
        perror("Não foi possível abrir o pipe");
        return 1;
    }
    if(pipe(pipeP1P2) < 0){
        perror("Não foi possivel abrir o pipe");
        close(pipeP0P1[0]);
        close(pipeP0P1[1]);
        return 1;
    }
    pid_t P1 = fork();
    if(P1 == -1){
        perror("Fork falhou");
        return 3;
    }
    if(P1 == 0){

    }else{
        int pipeP0P2[2];
        pid_t P2 = fork();
        if(P2 == -1){
            perror("Fork falhou");
            return 3;
        }
        if(P2 == 0){

        }else{

        }
    }
}