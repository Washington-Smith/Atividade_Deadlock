#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Inicializa os dois mutexes que representam os recursos compartilhados
pthread_mutex_t recurso1; 
pthread_mutex_t recurso2;
int deadlock_detected = 0; // Flag para detectar o deadlock

void* processoA(void* arg) {
    // Processo A está tentando adquirir o Recurso 1 primeiramente...
    printf("O processo A está tentando adquirir o recurso 1...\n");
    pthread_mutex_lock(&recurso1);
    printf("Processo A adquiriu o recurso 1. \n");

    // Simula algum processamento
    sleep(1);
  
    // Processo A está tentando adquirir o Recurso 2.
    printf("O processo A está tentando adquirir o recurso 2...\n");
    if (pthread_mutex_trylock(&recurso2) != 0) {
        printf("Processo A detectou deadlock ao tentar adquirir o recurso 2.\n");
        deadlock_detected = 1;
        // Libera os recursos e termina o processo A
        pthread_mutex_unlock(&recurso1);
        return NULL;
    }
    printf("Processo A adquiriu o recurso 2\n");

    // Libera os recursos
    pthread_mutex_unlock(&recurso2);
    pthread_mutex_unlock(&recurso1);

    return NULL;
}

void* processoB(void* arg) {
    // O processo B está tentando adquirir o Recurso 2 primeiramente...
    printf("O processo B está tentando adquirir o recurso 2...\n");
    pthread_mutex_lock(&recurso2);
    printf("Processo B adquiriu o recurso 2. \n");

    // Simula algum processamento
    sleep(1);
  
    // O processo B está tentando adquirir o Recurso 1.
    printf("O processo B está tentando adquirir o recurso 1...\n");
    if (pthread_mutex_trylock(&recurso1) != 0) {
        printf("Processo B detectou deadlock ao tentar adquirir o recurso 1.\n");
        deadlock_detected = 1;
        // Libera os recursos e termina o processo B
        pthread_mutex_unlock(&recurso2);
        return NULL;
    }
    printf("Processo B adquiriu o recurso 1\n");

    // Libera os recursos
    pthread_mutex_unlock(&recurso1);
    pthread_mutex_unlock(&recurso2);

    return NULL;
}

void* monitor(void* arg) {
    while (1) {
        sleep(2); // Verifica o estado de deadlock a cada 2 segundos
        if (deadlock_detected) {
            printf("Deadlock detectado! Liberando os recursos...\n");
            // Tenta liberar ambos os recursos
            pthread_mutex_unlock(&recurso1);
            pthread_mutex_unlock(&recurso2);
            break; // Sai do loop de monitoramento
        }
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t_monitor;

    // Inicializa os mutexes
    pthread_mutex_init(&recurso1, NULL);
    pthread_mutex_init(&recurso2, NULL);

    // Cria-se dois processos
    pthread_create(&t1, NULL, processoA, NULL);
    pthread_create(&t2, NULL, processoB, NULL);

    // Cria o thread de monitoramento para detectar deadlock
    pthread_create(&t_monitor, NULL, monitor, NULL);

    // Espera ambos os processos terminarem
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t_monitor, NULL);

    // Elimina os mutexes
    pthread_mutex_destroy(&recurso1);
    pthread_mutex_destroy(&recurso2);

    return 0;
}
