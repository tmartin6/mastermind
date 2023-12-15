#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Can be modified
#define VALUE    6    
#define PIN      4     
#define CHANCES  12    


int main(int argc, char* argv[]) {

    char solution[PIN];
    FILE* input = stdin; //read on standard input (keyboard)
    
    if (argc > 1) { //If filename specified
        input = fopen(argv[1],"r");
        if (!input) {
            printf("File incorrect %s\n", argv[1]);
            return -1;
        }
    }
    fscanf(input, "%s", solution);
    printf("%s\n", solution);


    if (argc>1) {
        fclose(input);
    }
    return 0;
}
