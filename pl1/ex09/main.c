#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROC 10
#define NUM_WRITES 100
int main()
{

    int i;
    int process_num = 0;
    pid_t proc[NUM_PROC];
    int status[NUM_PROC];
    for (i = 0; i < NUM_PROC; i++) // para o NUM_PROC
    {
        process_num = i;
        proc[process_num] = fork();
        status[process_num] = 0;
        if (proc[process_num] == -1)
        {
            perror("Erro na criação de processo");
            return EXIT_FAILURE;
        }
        if (proc[process_num] == 0)
        {
            break;
        }
    }

    if (proc[process_num] == 0)//Se for o processo filho então vamos contar
    {
        int n;
        (process_num == 0) ? (n = 1) : (n = process_num * 100 + 1); //em que número comeºar
        int lim = n + NUM_WRITES;                                   //qual o limite da escrita
        for (; n < lim; ++n)
        {
            printf("Count:%d\n", n);
        }
        exit(process_num);
    }
    else
    {
        for (i = 0; i < NUM_PROC; ++i) //esperar por todos os processos
        {
            waitpid(proc[i], &status[i], 0);
        }
    }

    /*a)
    O ouput não é ordenado.
    Não se consegue garantir que seja ordenado da maneira que é implementado, porque cria-se 10 processos independentes a correrem "ao mesmo tempo".
    Para além do mais, não é possível esperar que 10 processos acabem exatamente ao mesmo tempo.
    Pois cada processo partilha uma time slice do processador.
    */
    return 0;
}