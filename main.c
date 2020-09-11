#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    pid_t average;
    pid_t id;
} averages;

void asterisks(int, int);

int main(int argc, char const *argv[])
{
    int children = 0;
    if(argc > 1)
    {
        children = atoi(argv[1]);
    } else
    {
        // No number of child processes
        printf("Porfavor pon el numero de procesos hijos.\n");
        return -1;
    }

    pid_t pid;
    averages *pidAverages = (averages *)malloc(children * sizeof(averages));
    pid_t parentPID = getpid();

    int i = 0;
    averages *aux = pidAverages;

    while (i < children)
    {
        pid = fork();

        if (pid == 0)
        {
            //Assigning Child process pid to its corresponding position
            sleep(1);
            printf("Proceso hijo con PID = %d y  promedio es = %d\n", getpid(), (getpid() + parentPID) / 2);
            exit(0);
        }
        else if (pid == -1)
        {
            // Assigning -1 to failing child process position
            aux->id = 0;
            printf("Hubo un error creando proces hijo. Numero de procesos hijos creados hasta ahora: %d\n", i + 1);
            break;
        }
        else
        {
            // Storing pids and averages
            aux->id = pid;
            aux->average = rand() % 10;
        }
        
        i++;
        aux++;
    }

    averages *final = pidAverages + children;
    
    pid_t biggest = 0;

    // Waiting for children to finish
    for (aux = pidAverages; aux < final; ++aux)
    {
        waitpid(aux->id, NULL, 0);
        if (aux->average > biggest)
        {
            biggest = aux->average;
        }
        
    }

    // Histograma
    printf("PID Hijo\t\tPromedio\t\tHistograma\n");

    for (aux = pidAverages; aux < final; ++aux)
    {
        printf("%d\t\t\t%d\t\t\t", aux->id, aux->average);
        asterisks(aux->average, biggest);
    }

    // freeing memory
    free(pidAverages);

    return 0;
}

void asterisks(int average, int biggestAverage)
{
    // Calculating as percentage of biggestAverage
    float size = (average * 10) /biggestAverage;

    for (int i = 0; i < size; ++i)
    {
        printf("*");
    }
    printf("\n");
}