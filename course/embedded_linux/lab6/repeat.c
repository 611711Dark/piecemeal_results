#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <character>\n", argv[0]);
        return 1;
    }
    
    char c = argv[1][0];
    
    while (1) {
        printf("%c", c);
        fflush(stdout);
    }
    
    return 0;
}
