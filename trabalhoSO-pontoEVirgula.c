
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t pthread_condB = PTHREAD_COND_INITIALIZER;
pthread_cond_t condC = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int qntLinhasAtual = 0;
int qntLinhasDesejada = 0;
enum
{
  threadA,
  threadB,
  threadC
} threadAtual = threadA;

int qntH = 0;
int qntO = 0;
void *thr_Hidrogenio()
{
  for (;;)
  {
    if (threadAtual == threadA)
    {
      if (qntH != 2)
      {
        printf("H");
        qntH++;
      }
      else
      {
        qntH = 0;
        threadAtual = threadB;
        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&pthread_condB);
        pthread_mutex_unlock(&mutex);
      }
    }
    else
    {
      pthread_mutex_lock(&mutex);
      pthread_cond_wait(&condA, &mutex);
      pthread_mutex_unlock(&mutex);
    }

    //do stuff
  }
  return 0;
}

void *thr_Enxofre()
{
  for (;;)
  {
    if (threadAtual == threadB)
    {

      printf("S");

      threadAtual = threadC;
      pthread_mutex_lock(&mutex);
      pthread_cond_signal(&condC);
      pthread_mutex_unlock(&mutex);
    }
  }
  return 0;
}
void *thr_Oxigenio()
{
  for (;;)
  {
    if (threadAtual == threadC)
    {

      //do stuff
      if (qntO != 4)
      {
        printf("O");
        qntO++;
      }
      else
      {
        qntLinhasAtual++;
        printf("\n");
        if (qntLinhasAtual == qntLinhasDesejada)
        {
          exit(0);
        }
        qntO = 0;
        threadAtual = threadA;
        pthread_mutex_lock(&mutex);
        pthread_cond_signal(&condA);
        pthread_mutex_unlock(&mutex);
      }
    }
    else
    {
      pthread_mutex_lock(&mutex);
      pthread_cond_wait(&condC, &mutex);
      pthread_mutex_unlock(&mutex);
    }
  }
}
int main(int argc, char *argv[])
{
  if (argv[1] != NULL)
  {
    qntLinhasDesejada = atoi(argv[1]);

    pthread_t threadHidrogenio, threadEnxofre, threadOxigenio;

    pthread_create(&threadHidrogenio, NULL, thr_Hidrogenio, NULL);
    pthread_create(&threadEnxofre, NULL, thr_Enxofre, NULL);
    pthread_create(&threadOxigenio, NULL, thr_Oxigenio, NULL);

    pthread_join(threadHidrogenio, NULL);
    pthread_join(threadEnxofre, NULL);
    pthread_join(threadOxigenio, NULL);
  }
  else
  {
    printf("Por favor, preencha o argumento de quantidade de linhas.\n");
  }
}