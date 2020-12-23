
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Variáveis de condição das threads
pthread_cond_t pthread_condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t pthread_condB = PTHREAD_COND_INITIALIZER;
pthread_cond_t pthread_condC = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Variáveis utilizadas para controlar as linhas do programa que serão exibidas para o usuário.
// *qntLinhasAtual* para saber quantas vezes o código já printou a linha HHSOOO
// *qntLinhasDesejada* para saber quantas vezes o código deverá printar a linha HHSOOO
int qntLinhasAtual = 0;
int qntLinhasDesejada = 0;

// Variáveis para saber quantas vezes as letras foram printadas
// *qntH* para saber quantas vezes o "H" foi printado
// *qntO* para saber quantas vezes o "O" foi printado
int qntH = 0;
int qntO = 0;

// Enum para poder controlar qual thread irá poder realizar a ação, os nomes são respectivos.
enum
{
  threadA,
  threadB,
  threadC
} threadAtual = threadA;

// Função para printar a letra "H"
// Sua lógica se consiste em um for infinito, onde dentro dele há a verificação se a thread que deverá realizar a ação é a thread do Hidrogênio, ou seja, a threadA.
// Caso não for, o código trava o mutex, coloca a threadA em espera e destrava o mutex.
// Caso for, o código valida se a letra "H" não foi printada 2 vezes já.

// Caso não, printa "H" e incrementa a variável "qntH".
// Caso sim, a variável de contagem do "H" é zerada, a thread que deverá realizar a ação vira a thread do Enxofre, ou seja, a threadB,
// o mutex é travado, a threadB é ativada e depois o mutex é destravado.
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
      pthread_cond_wait(&pthread_condA, &mutex);
      pthread_mutex_unlock(&mutex);
    }
  }
  return 0;
}

// Função para printar a letra "S"
// Sua lógica se consiste em um for infinito, onde dentro dele há a verificação se a thread que deverá realizar a ação é a thread do Enxofre, ou seja, a threadB.
// Caso não for, o código trava o mutex, coloca a threadB em espera e destrava o mutex.
// Caso for, o código printa "S", a thread que deverá realizar a ação vira a thread do Oxigênio, ou seja, a threadC,
// o mutex é travado, a threadC é ativada e depois o mutex é destravado.
void *thr_Enxofre()
{
  for (;;)
  {
    if (threadAtual == threadB)
    {

      printf("S");

      threadAtual = threadC;
      pthread_mutex_lock(&mutex);
      pthread_cond_signal(&pthread_condC);
      pthread_mutex_unlock(&mutex);
    }
    else
    {
      pthread_mutex_lock(&mutex);
      pthread_cond_wait(&pthread_condB, &mutex);
      pthread_mutex_unlock(&mutex);
    }
  }
  return 0;
}

// Função para printar a letra "O"
// Sua lógica se consiste em um for infinito, onde dentro dele há a verificação se a thread que deverá realizar a ação é a thread do Oxigênio, ou seja, a threadC.
// Caso não for, o código trava o mutex, coloca a threadC em espera e destrava o mutex.
// Caso for, o código valida se a letra "O" não foi printada 4 vezes já.

// caso não, printa "O" e incrementa a variável "qntO".
// Caso sim, a variável de contagem do "O" é zerada, a thread que deverá realizar a ação vira a thread do Hidrogênio, ou seja, a threadA,
// o mutex é travado, a threadA é ativada e depois o mutex é destravado.

void *thr_Oxigenio()
{
  for (;;)
  {
    if (threadAtual == threadC)
    {
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
        pthread_cond_signal(&pthread_condA);
        pthread_mutex_unlock(&mutex);
      }
    }
    else
    {
      pthread_mutex_lock(&mutex);
      pthread_cond_wait(&pthread_condC, &mutex);
      pthread_mutex_unlock(&mutex);
    }
  }
}

// Na main é recebido o número de linhas desejadas, vindo da linha de comando do prompt. A string é convertida em inteiro, depois é verificado se o número NÃO é NULL, ou seja, vazio, e maior que 0.
// Caso for, entra na condição
// Caso não, uma mensagem é exibida para o usuário com as devidas orientações.

int main(int argc, char *argv[])
{

  if (argv[1] != NULL && atoi(argv[1]) > 0)
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
    printf("Por favor, preencha o argumento de quantidade de linhas com um numero maior que 0.\n");
  }
}