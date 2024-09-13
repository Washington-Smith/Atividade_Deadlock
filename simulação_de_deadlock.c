#include <stdio.h>
#include <pthread.h>

//Inicializa os dois mutexes que representam os recursos compartilhados
pthread_mutex_t recurso1; 
pthread_mutex_t recurso2;

void* processoA(void* arg){
    
    // Processo A está tentando adquirir o Recurso 1 primeiramente...
    printf("O processo A está tentando adquirir o recurso 1...\n");
    pthread_mutex_lock(&recurso1);
    printf("Processo A adquiriu o recurso 1. \n");

    // Simula algum processamento
    sleep(1);
  
    // Processo A está tentando adquirir o Recurso 2.
    printf("O processo A está tentando adquirir o recurso 2...\n");
    pthread_mutex_lock(&recurso2);
    printf("Processo A adquiriu o recurso2\n");

    // Libera os recursos
    pthread_mutex_unlock(&recurso2);
    pthread_mutex_unlock(&recurso1);
}

void* processoB(void* arg){

    //O processo B está tentando adquirir o recurso 2 primeiro....
    printf("Processo B está tentando adquirir o recurso2... \n");
    pthread_mutex_lock(&recurso2);
    printf("Processo B adquiriu o recurso2 \n");

    // Simula algum processamento
    sleep(1); 

    //O processo B está tentando adquirir o recurso 1.
    printf("O processo B está tentando adquirir o recurso1...\n");
    pthread_mutex_lock(&recurso1);
    printf("Processo B adquiriu o recurso1 \n");

    // Libera os recursos
    pthread_mutex_unlock(&recurso1);
    pthread_mutex_unlock(&recurso2);
}

int main(){
    pthread_t t1, t2;

    //Inicializa os mutexes
    pthread_mutex_init(&recurso1, NULL);
    pthread_mutex_init(&recurso2, NULL);

    //Cria-se dois processos
    pthread_create(&t1, NULL, processoA, NULL);
    pthread_create(&t2, NULL, processoB, NULL);

    //Espera ambos os processos terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    //Elimina os mutexes
    pthread_mutex_destroy(&recurso1);
    pthread_mutex_destroy(&recurso2);
    return 0;
}
