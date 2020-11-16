#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

    // Escreve x no descritor de escrita do pipe P0-P1
    write(pipeP0P1[1], &x, sizeof(x));
    printf("Parent (%d) sent to P1: %d\n", getpid(), x);

    pid_t P1 = fork(); // Cria filho 1
    if (P1 == -1)
    {
        perror("Fork falhou.\n");
        return 3;
    }
    if (P1 == 0) // Filho 1
    {
    }
    else // Pai
    {
        // Cria vetor para descritores do pipe P0-P2
        int pipeP0P2[2];

        pid_t P2 = fork(); // Cria filho 2 (apenas)
        if (P2 == -1)
        {
            perror("Fork falhou");
            return 3;
        }
        if (P2 > 0) // Pai
        {
        }
        else // Filho 2
        {
        }
    }

    // Lê x enviado pelo pipe e cria vetor randômico
    if (P1 == 0) // Filho 1
    {
        close(pipeP0P1[1]);
        int x = 0;
        read(pipeP0P1[0], &x, sizeof(x));
        printf("\nP1 na voz: x = %i\n", x);

        int tamVetorP1 = rand() % 10 + 1;
        int vetorRandP1[tamVetorP1];
        printf("P1: Vetor randômico de tamanho %i:\n", tamVetorP1);
        for (int i = 0; i < tamVetorP1; i++)
        {
            vetorRandP1[i] = rand() % x + 1;
            printf("%i ", vetorRandP1[i]);
        }
        printf("\n");
    }

    // Envia MENSAGEM para filho através do pipe P0-P1
    if (P1 > 0) // Pai
    {
        // Envia MENSAGEM no pipe P0-P1
        const char *MENSAGEM = "Meu filho, crie e envie para o seu irmão um array de números inteiros com valores randômicos entre 1 e o valor enviado anteriormente. O tamanho do array também deve ser randômico, na faixa de 1 a 10.";
        write(pipeP0P1[1], &MENSAGEM, sizeof(MENSAGEM));
        printf("Parent (%d) sent to P1: %s\n", getpid(), MENSAGEM);
    }

    // Lê MENSAGEM do pai através do pipe P0-P1
    if (P1 == 0) // Filho 1
    {
        close(pipeP0P1[1]);

        char *mensagemPai = NULL;
        read(pipeP0P1[0], &mensagemPai, sizeof(mensagemPai));
        printf("\nP1 na voz: %s\n", mensagemPai);
    }
}
