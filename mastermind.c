#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

//Can be modified
#define VALUE    6    
#define PIN      4     
#define CHANCES  12    


int verif_proposition(char* prop){
    int n = strlen(prop);
    if (n == PIN){
        for (int i = 0; i < n; i++){
            if (prop[i] < 48 || prop[i] >53)
                return 0;
        }
        return 1;
    }
    return 0;
}

void compute_result(char* solution, char* prop, char* res){  
    int sol_edited[PIN] = {0};
    int prop_edited[PIN] = {0};

    int nb_X = 0;
    int nb_O = 0;

    // Bonne valeur bien placée
    for (int i = 0; i < PIN; i++){
        if (solution[i] == prop[i]){
            nb_X++;
            sol_edited[i] = 1;
            prop_edited[i] = 1;
        }
    }
    // Bonne valeur mal placée
    for(int i = 0; i < PIN; i++){ // i -> proposition
        if(!prop_edited[i])
            for (int j = 0; j < PIN; j++){ // j -> solution
                if (!sol_edited[j]){
                    if (solution[j] == prop[i]){
                        nb_O++;
                        sol_edited[j] = 1;
                        prop_edited[i] = 1;
                        break;
                    }
                }
            }
    }

    for (int i = 0; i < nb_X; i++){
        res[i] = 'X';
    }
    for (int i = nb_X; i < nb_X+nb_O; i++){
        res[i] = 'O';
    }
    for (int i = nb_X + nb_O; i < PIN; i++){
        res[i] = '_';
    }
    res[PIN] = '\0';
}

void random_machine(char* prop){
    for(int i = 0; i < PIN; i++){
        prop[i] = rand() % 5 + 48;
    }
    prop[PIN] = '\0';
}

int compute_score(char* res){
    int s = 0;
    for (int i = 0; i < PIN; i++){
        if (res[i] == 'X') 
            s += 10;
        else if (res[i] == 'O')
            s += 1;
    }
    return s;
}

int main(int argc, char* argv[]) {

    // ? Zone de test
    // ?    


    srand(time(NULL));
    // * Game mode 
    int game_mode = -1; // Random : 0, From file : 1, Player entry : 2
    while((game_mode != 0) && (game_mode != 1) && (game_mode != 2)){
        printf("Mode de jeu (0 : Aléatoire, 1 : Depuis un fichier, 2 : Contre l'ordinateur) : ");
        scanf("%d", &game_mode);
    }

    // * Define a solution
    char solution[PIN+1];
    if (game_mode == 0){
        for(int i = 0; i < PIN; i++){
            solution[i] = rand() % 5 + 48;
        }
        solution[PIN] = '\0';
        printf("%s\n", solution); // TODO : Remove 

        for(int i = 0; i < CHANCES; i++){
            printf("Chances restantes : %d\n", CHANCES - i);

            int OK = 0;
            char prop[PIN+1];

            while(!OK){
                printf("Proposition : ");
                scanf("%s", prop);
                if(verif_proposition(prop))
                    OK = 1;
                else   
                    printf("Vous devez choisir 4 chiffres entre 0 et 5 ! \n\n");
            }
            char res[PIN+1];
            compute_result(solution, prop, res);
            printf("%s\n", res);
            if(strcmp(prop, solution) == 0) {
                printf("Gagné !\n");
                break;
            }
        }   
    }

    if(game_mode == 1){
        FILE* input = stdin; //read on standard input (keyboard)
        
        if (argc > 1) { //If filename specified
            input = fopen(argv[1],"r");
            if (!input) {
                printf("File incorrect %s\n", argv[1]);
                return -1;
            }
        }
        fscanf(input, "%s", solution);
        solution[PIN] = '\0';
        printf("%s", solution);

        for(int i = 0; i < CHANCES; i++){
            printf("Chances restantes : %d\n", CHANCES - i);

            int OK = 0;
            char prop[PIN+1];

            while(!OK){
                printf("Proposition : ");
                fscanf(input, "%s", prop);
                prop[PIN] = '\0';
                printf("%s\n", prop);
                if(verif_proposition(prop))
                    OK = 1;
                else   
                    printf("Vous devez choisir 4 chiffres entre 0 et 5 ! \n\n");
            }
            fscanf(input, "%*s");
            char res[PIN+1];
            compute_result(solution, prop, res);
            printf("%s\n", res);
            if(strcmp(prop, solution) == 0) {
                printf("Gagné !\n");
                break;
            }
        } 

        if (argc>1) {
            fclose(input);
        }
    }

    if (game_mode == 2){
        int OK = 0;

        while (OK != 1){
            OK = 1;
            printf("Choisir la solution (4 chiffres entre 0 et 5) : ");
            char c[PIN];
            scanf("%s", c);
            for(int i = 0; i < 4; i++){
                if (c[i] < 48 || c[i] > 53){
                    printf("Mauvaise valeur de solution !\n\n");
                    OK = 0;
                }
                solution[i] = c[i];
            }
        }

        int mode = -1; // 0 -> random, 1 -> optimisé
        while(mode != 0 && mode != 1){
            printf("Version aléatoire (0) ou optimisée (1) ? ");
            scanf("%d", &mode);
        }

        // * Random mode

        if (mode == 0){
            printf("Solution : %s\n", solution);
            for(int i = 0; i < CHANCES; i++){
                printf("Chances restantes : %d\n", CHANCES - i);
                char prop[PIN+1];
                random_machine(prop);
                printf("%s\n", prop);
                char res[PIN+1];
                compute_result(solution, prop, res);
                printf("%s\n", res);
                if(strcmp(prop, solution) == 0) {
                    printf("Gagné !\n");
                    break;
            }
            }
        }
    }

    
    // printf("%s\n", solution);
    return 0;
}
