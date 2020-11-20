#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "include/utils.h"

int main()
{
    srand(time(NULL));

    int x;
    scanf("%d", &x);

    if (x < 1 || x > 5)
    {
        fprintf(stderr, "X menor que 1 ou maior que 5\n");
        return 2;
    }

    // Cria vetores para descritores
    int pipeP0P1[2];
    int pipeP1P2[2];

    // Cria pipes
    if (pipe(pipeP0P1) < 0)
    {
        perror("Não foi possível abrir o pipe.\n");
        return 1;
    }

    if (pipe(pipeP1P2) < 0)
    {
        perror("Não foi possivel abrir o pipe.\n");
        close(pipeP0P1[0]);
        close(pipeP0P1[1]);
        return 1;
    }


    pid_t P1 = fork(); // Cria filho 1
    if (P1 == -1)
    {
        perror("Fork falhou.\n");
        return 3;
    }
    if (P1 == 0) // Filho 1
    {
        close(pipeP0P1[1]);
        close(pipeP1P2[0]);
        int x = 0;
        read(pipeP0P1[0], &x, sizeof(x));
        printf("\nP1 na voz: x = %i\n", x);
        char *mensagemPai = NULL;
        read(pipeP0P1[0], &mensagemPai, sizeof(mensagemPai));
        printf("\nP1 na voz: %s\n", mensagemPai);
        int tamVetorP1 = rand() % 10 + 1;
        int vetorRandP1[tamVetorP1];
        printf("P1: Vetor randômico de tamanho %i:\n", tamVetorP1);
        for (int i = 0; i < tamVetorP1; i++)
        {
            vetorRandP1[i] = rand() % x + 1;
            printf("%i ", vetorRandP1[i]);
        }
        printf("\n");
        write(pipeP1P2[1], &tamVetorP1, sizeof(int));
        write(pipeP1P2[1], vetorRandP1, sizeof(int)*tamVetorP1);
        close(pipeP0P1[0]);
        close(pipeP1P2[1]);
    }
    else // Pai
    {
        // Cria vetor para descritores do pipe P0-P2
        int pipeP0P2[2];
        pipe(pipeP0P2);
        pid_t P2 = fork(); // Cria filho 2 (apenas)
        if (P2 == -1)
        {
            perror("Fork falhou");
            return 3;
        }
        if (P2 > 0) // Pai
        {
            // Escreve x no descritor de escrita do pipe P0-P1
            close(pipeP0P1[0]);
            close(pipeP0P2[1]);
            close(pipeP1P2[0]);
            close(pipeP1P2[1]);
            write(pipeP0P1[1], &x, sizeof(x));
            printf("Parent (%d) sent to P1: %d\n", getpid(), x);
            const char *MENSAGEM = "Meu filho, crie e envie para o seu irmão um array de números inteiros com valores randômicos entre 1 e o valor enviado anteriormente. O tamanho do array também deve ser randômico, na faixa de 1 a 10.";
            write(pipeP0P1[1], &MENSAGEM, sizeof(MENSAGEM));
            printf("Parent (%d) sent to P1: %s\n", getpid(), MENSAGEM);
            int soma;
            read(pipeP0P2[0], &soma, sizeof(int));
            printf("P0: resultado da soma: %d\n", soma);
            close(pipeP0P1[1]);
            close(pipeP0P2[0]);
            int P3 = fork();
            if(P3 < 0){
                perror("Fork falhou");
            }
            if(P3 == 0){
                int fileOut = open("PipePing.txt", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                dup2(fileOut, STDOUT_FILENO);
                close(fileOut);
                if(execlp("ping", "ping", "-c", "5", "ufes.br", NULL) < 0){
                    perror("erro");
                }
            }
        }
        else // Filho 2
        {
            close(pipeP0P1[0]);
            close(pipeP0P1[1]);
            close(pipeP1P2[1]);
            close(pipeP0P2[0]);
            int vecSizeFromP1;
            read(pipeP1P2[0], &vecSizeFromP1, sizeof(int));
            printf("P2 na voz: tamanho do vetor: %d\n", vecSizeFromP1);
            int vecFromP1[vecSizeFromP1];
            read(pipeP1P2[0], vecFromP1, sizeof(int)*vecSizeFromP1);
            printf("P2 na voz: vetor vindo de P1: ");
            for(int i = 0; i < vecSizeFromP1; i++){
                printf("%d ", vecFromP1[i]);
            }
            printf("\n");
            int sum = vecSum(vecSizeFromP1, vecFromP1);
            printf("P2: soma = %d\n", sum);
            write(pipeP0P2[1], &sum, sizeof(int));
            close(pipeP1P2[0]);
            close(pipeP0P2[1]);
        }
    }
}
