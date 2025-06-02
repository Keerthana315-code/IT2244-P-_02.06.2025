#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

// Function to compute factorial
long long factorial(int num) {
    long long fact = 1;
    for (int i = 1; i <= num; i++) {
        fact *= i;
    }
    return fact;
}

int main() {
    size_t size = 3 * sizeof(long long); // Shared memory size
    long long *shared_mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if (shared_mem == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    int n, r;
    printf("Parent: Enter n: ");
    scanf("%d", &n);
    printf("Parent: Enter r: ");
    scanf("%d", &r);

    pid_t pid = fork();
    
    if (pid == 0) { // Child process
        printf("Child: Computing factorials...\n");
        shared_mem[0] = factorial(n);       // n!
        shared_mem[1] = factorial(r);       // r!
        shared_mem[2] = factorial(n - r);   // (n-r)!
        exit(0);
    } else if (pid > 0) { // Parent process
        wait(NULL); // Wait for child to finish
        long long fact_n = shared_mem[0];
        long long fact_r = shared_mem[1];
        long long fact_nr = shared_mem[2];

        long long nCr = fact_n / (fact_r * fact_nr);
        long long nPr = fact_n / fact_nr;

        printf("Parent: nCr (C(n, r)) = %lld\n", nCr);
        printf("Parent: nPr (P(n, r)) = %lld\n", nPr);

        // Cleanup
        munmap(shared_mem, size);
    } else {
        perror("fork failed");
        exit(1);
    }

    return 0;
}
