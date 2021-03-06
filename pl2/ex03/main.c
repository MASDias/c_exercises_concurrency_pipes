#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    pid_t pid;
    int status;
    int fd[2];
    char hello[20] = "Hello world";
    char goodbye[20] = "Goodbye";

    if (pipe(fd) == -1) //teste para verificar sucesso pipe
    {
        perror("FAIL PIPE");
        return EXIT_FAILURE;
    }
    pid = fork();  //criação de um processo
    if (pid == -1) //verificar erro na criação do processo
    {
        perror("Erro criação de processo");
        return EXIT_FAILURE;
    }
    if (pid > 0) //processo pai
    {
        close(fd[0]);  // fechar pipe de leitura para fazer escrita 
        if(write(fd[1], (void *)&hello, 20) == -1){  //fazer a escrita "Hello World" para o pipe
            perror("Erro Escrita");
            return EXIT_FAILURE;
        }
        if(write(fd[1], (void *)&goodbye, 20) == -1) //fazer a escrita "Goodbye" para o pipe
        {
            perror("Erro Escrita");
            return EXIT_FAILURE;
        }
        close(fd[1]);                                    //fechar o pipe para escrita
        wait(&status);
        if(WIFEXITED(status))
        {
            printf("---Parent---\nPID:%d\nExit value:%d\n",pid,WEXITSTATUS(status));
        }
    }
    if (pid == 0)
    {
        char readHello [20];
        char readGoodbye [20];
        close(fd[1]); // fecha pipe de escrita pra fazer leitura
        if(read(fd[0], (void *)&readHello, 20 ) == -1){  //fazer a leitura "Hello World" para o pipe
            perror("Erro leitura Hello world");
            return EXIT_FAILURE;
        }
        if(read(fd[0], (void *)&readGoodbye, 20 ) == -1){  //fazer a leitura "Goodbye" para o pipe
            perror("Erro leitura Goodbye");
            return EXIT_FAILURE;
        }
        printf("---Child---\n%s\n%s\n",readHello,readGoodbye);
        close(fd[0]);                 //fechar o pipe apos leitura
        exit(2);
    }
    return 0;
}