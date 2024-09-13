#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Inicializa os dois mutexes que representam os recursos compartilhados
pthread_mutex_t recurso1; 
pthread_mutex_t recurso2;

void* processoA(void* arg) {
    // Processo A sempre adquire Recurso 1 primeiro
    printf("O processo A está tentando adquirir o recurso 1...\n");
    pthread_mutex_lock(&recurso1);
    printf("Processo A adquiriu o recurso 1. \n");

    // Simula algum processamento
    sleep(1);

    // Processo A agora tenta adquirir o Recurso 2
    printf("O processo A está tentando adquirir o recurso 2...\n");
    pthread_mutex_lock(&recurso2);
    printf("Processo A adquiriu o recurso 2.\n");

    // Libera os recursos
    pthread_mutex_unlock(&recurso2);
    pthread_mutex_unlock(&recurso1);

    return NULL;
}

void* processoB(void* arg) {
    // Processo B também adquire Recurso 1 primeiro para evitar impasse
    printf("O processo B está tentando adquirir o recurso 1...\n");
    pthread_mutex_lock(&recurso1);
    printf("Processo B adquiriu o recurso 1. \n");

    // Simula algum processamento
    sleep(1);

    // Processo B agora tenta adquirir o Recurso 2
    printf("O processo B está tentando adquirir o recurso 2...\n");
    pthread_mutex_lock(&recurso2);
    printf("Processo B adquiriu o recurso 2.\n");

    // Libera os recursos
    pthread_mutex_unlock(&recurso2);
    pthread_mutex_unlock(&recurso1);

    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Inicializa os mutexes
    pthread_mutex_init(&recurso1, NULL);
    pthread_mutex_init(&recurso2, NULL);

    // Cria-se dois processos
    pthread_create(&t1, NULL, processoA, NULL);
    pthread_create(&t2, NULL, processoB, NULL);

    // Espera ambos os processos terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Elimina os mutexes
    pthread_mutex_destroy(&recurso1);
    pthread_mutex_destroy(&recurso2);

    return 0;
}
