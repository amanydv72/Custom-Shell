#include <stdio.h>
#include <stdlib.h>

long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main(int argc, char *argv[]) {
    int num;

    // Check if the number is provided as a command-line argument
    if (argc == 2) {
        num = atoi(argv[1]); // Convert string to integer
    } else {
        // If no argument is provided, read from stdin
        if (scanf("%d", &num) != 1) {
            fprintf(stderr, "Usage: ./fact <number>\n");
            return 1;
        }
    }

    // Validate the input
    if (num < 0) {
        fprintf(stderr, "Factorial of negative numbers is undefined.\n");
        return 1;
    }

    // Calculate and print the factorial
    printf("Factorial of %d is %lld\n", num, factorial(num));
    return 0;
}
