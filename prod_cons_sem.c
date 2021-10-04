#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define N 3 // espaço no buffer
int buffer[N]; // buffer compartilhado
int c = 0; // posição a ser consumida no buffer
int e = 0; // posição a ser enchida no buffer

sem_t sem_encheu;
sem_t sem_esvaziou;

void produzir(int *x);
void consumir(int *x);

void produtor(){
  while(1){
    sem_wait(&sem_esvaziou);
    e = (e + 1)%N;
    printf("Produtor enchendo o buffer\n");
    produzir(&buffer[e]);
    if( buffer[e] > 1 ) printf("Produtor produziu em um espaço ocupado\n");
    sem_post(&sem_encheu);
    sleep(1);
  }
}


void consumidor(){
  while(1){
    sem_wait(&sem_encheu);
    c = (c + 1)%N;
    printf("Consumidor esvaziando o buffer\n");
    consumir(&buffer[c]);
    if( buffer[c] < 0 ) printf("Consumidor consumiu em espaço vazio\n");
    sem_post(&sem_esvaziou);
    sleep(1);
  }
}

int main() {
  pthread_t thread_prod; 
  pthread_t thread_cons;

  sem_init(&sem_encheu, 0, 0);
  sem_init(&sem_esvaziou, 0, 1);
  
  // Iniciando com o buffer vazio
  for(int n = 0; n < N; n++)
    buffer[n] = 0;
  
  pthread_create(&thread_prod, NULL, produtor,
  NULL);
  pthread_create(&thread_cons, NULL, consumidor,
  NULL);
  
  pthread_join(thread_prod, NULL);
  pthread_join(thread_cons, NULL);

  sem_destroy(&sem_encheu);
  sem_destroy(&sem_esvaziou);

  return 0;
}

void produzir(int *x){
  *x = *x + 1;
}

void consumir(int *x){
  *x = *x - 1;
}
