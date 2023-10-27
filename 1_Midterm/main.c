#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h> 

#define SHM_KEY 12345
#define SEM_KEY 54321

union semun {
    unsigned short* array;
    struct semid_ds* buf;
    int val;
  
    
};

void P(int sem_id) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    semop(sem_id, &sb, 1);
}

void V(int sem_id) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = 0;
    semop(sem_id, &sb, 1);
}

int main() {
    double *shared_memory;
    int shmid, semid;
    pid_t child1, child2;

    // Creating shared memory segment
    if ((shmid = shmget(SHM_KEY, sizeof(double), IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(1);
    }

    // Allocating memory segment
    if ((shared_memory = shmat(shmid, NULL, 0)) == (double *)-1) {
        perror("shmat");
        exit(1);
    }

    // Creating semaphore
    if ((semid = semget(SEM_KEY, 1, IPC_CREAT | 0666)) == -1) {
        perror("semget");
        exit(1);
    }

    // Initialization semaphore value to 0
    union semun semarg;
    semarg.val = 0;
    semctl(semid, 0, SETVAL, semarg);

    // Initialization shared memory with a negative number
    *shared_memory = -5.0;

    // Fork the first child process
    if ((child1 = fork()) == 0) {
       
        P(semid); // Just waiting for the signal
        *shared_memory *= -3.0; // Multiply by a negative number
        V(semid); // Signal the other child process
        exit(0);
    }

    // Fork the second child process
    if ((child2 = fork()) == 0) {
        
        P(semid); 
        *shared_memory += 2.0;
        exit(0);
    }

    // Parent process waits for both child processes to terminate
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    printf("Shared Memory Value: %.2lf\n", *shared_memory);

    // Clean up
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
