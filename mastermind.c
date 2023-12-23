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
        if(!prop_edited[i]){
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

int score(char* solution, char* prop){
    char tmp[PIN+1];
    compute_result(solution, prop, tmp);
    return compute_score(tmp);
}

int comb_to_i(char* comb){
    int n = 1;
    int i = 0;
    for (int j = PIN-1; j >= 0; j--){
        i += n*(comb[j]-48);
        n *= 6;
    }
    return i;
}

void i_to_comb(int i, char* comb) {
    for (int j = PIN-1; j >= 0; j--) {
        comb[j] = i % 6 + 48; 
        i /= 6;
    }
    comb[PIN] = '\0'; 
}

int first_index_1(int* t){
    int i = 0;
    while(t[i] != 1){
        i++;
    }
    return i;
}

void machine_eliminate(char* solution){
    int CAND[1296];
    for (int i = 0; i < 1296; i++){
        CAND[i]=1;
    }
    char comb[PIN+1];
    int i_comb = rand()%1296;
    i_to_comb(i_comb, comb);
    char res[PIN+1];
    compute_result(solution, comb, res);
    int s = compute_score(res);
    int essai = 0;
    printf("Chances restantes : %d\n", CHANCES - essai);
    printf("%s\n", comb);
    printf("%s\n", res);
    essai++;
    while((s != 40) && (essai < CHANCES)){
        for (int i = 0; i < 1296; i++){
            if(CAND[i] != 0){
                char tmp[PIN+1];
                i_to_comb(i, tmp);
                if (score(comb, tmp) != s)
                    CAND[i] = 0; 
            }  
        }
        CAND[i_comb] = 0;
        printf("Chances restantes : %d\n", CHANCES - essai);
        i_comb = first_index_1(CAND);
        i_to_comb(i_comb, comb);
        printf("%s\n", comb);
        compute_result(solution,comb,res);
        printf("%s\n", res);
        s = compute_score(res);
        essai++;
    }
}

int nb_candidat(int* CAND){
    int res = 0;
    for(int i = 0; i < 1296; i++){
        res += CAND[i];
    }
    return res;
}

int max_tab(int* tab){
    int max = tab[0];
    for(int i = 0; i < 41; i++){
        if (tab[i] > max){
            max = tab[i];
        }
    }
    return max;
}

int index_min_tab(int* tab){
    int min = tab[0];
    int index_min = 0;
    for(int i = 0; i < 41; i++){
        if (tab[i] < min){
            min = tab[i];
            index_min = i;
        }
    }
    return index_min;
}

void meilleure_prop(int* CAND, char* P){
    if(nb_candidat(CAND) == 1){
        int index_P = first_index_1(CAND);
        i_to_comb(index_P, P);
        exit(0);
    }

    // * Création TAB
    int** TAB = malloc(1296*sizeof(int*));
    for (int i = 0; i < 1296; i++) {
        TAB[i] = (int *)malloc(41 * sizeof(int));
        for (int j = 0; j < 41; j++){
            TAB[i][j] = 0;
        }
    }

    int i = 0;
    for (int j = 0; j < 1296; j++){
        for (int k = 0; j < 1296; k++){
            if (CAND[k] == 1){
                char P[PIN+1];
                i_to_comb(j, P);
                char C[PIN+1];
                i_to_comb(k, C);
                int s = score(P, C);
                TAB[i][s]++;
            }
            i++;
        }
    }

    int MAX[1296];
    for(int i = 0; i < 1296; i++){
        MAX[i] = max_tab(TAB[i]);
    }
    int index_P = index_min_tab(MAX);
    i_to_comb(index_P, P);
}

void machine_optimal(char* solution){
int CAND[1296];
    for (int i = 0; i < 1296; i++){
        CAND[i]=1;
    }
    char P[PIN+1];
    meilleure_prop(CAND, P);
    char res[PIN+1];
    compute_result(solution, P, res);
    int s = compute_score(res);
    int essai = 0;
    printf("Chances restantes : %d\n", CHANCES - essai);
    printf("%s\n", P);
    printf("%s\n", res);
    essai++;
    while((s != 40) && (essai < CHANCES)){
        for (int i = 0; i < 1296; i++){
            if(CAND[i] != 0){
                char tmp[PIN+1];
                i_to_comb(i, tmp);
                if (score(P, tmp) != s)
                    CAND[i] = 0; 
            }  
        }
        // CAND[i_comb] = 0;
        printf("Chances restantes : %d\n", CHANCES - essai);
        meilleure_prop(CAND, P);
        printf("%s\n", P);
        compute_result(solution,P,res);
        printf("%s\n", res);
        s = compute_score(res);
        essai++;
    }
}

int main(int argc, char* argv[]) {
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
        while(mode != 0 && mode != 1 && mode != 2){
            printf("Version aléatoire (0), par élimination (1) ou optimale (2) ? ");
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

        if (mode == 1){
            machine_eliminate(solution);
        }

        if(mode == 2){
            machine_optimal(solution);
        }
    }

    
    // printf("%s\n", solution);
    return 0;
}
