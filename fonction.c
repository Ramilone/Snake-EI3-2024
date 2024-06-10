/*--------------*
 |	BENSIDHOUM  |
 |	Nicolas		|
 |	EISE TPA	|
 *--------------* 
*/


#include "fonction.h"




Arena* initArena(int nbWalls, int* wall, int sizeX, int sizeY){

    /* Allocation de la mémoire pour l'arène */
    Arena* arene = (Arena*)malloc(sizeof(Arena)); 
    arene->length = sizeX; 
    arene->width = sizeY; 
    arene->grid = (Cell**)malloc(sizeX*sizeof(Cell*));
    for(int i=0; i < sizeX; i++){
        arene->grid[i] = (Cell*)malloc(sizeY*sizeof(Cell)); 
    }
    /* Création des bordures extérieures */
    for(int i=0; i<sizeX; i=i+1){
        for(int j=0; j<sizeY; j=j+1){
            /* On traite les murs après */
            arene->grid[i][j].north_wall = 0; 
            arene->grid[i][j].east_wall = 0;
            arene->grid[i][j].south_wall = 0;
            arene->grid[i][j].west_wall = 0;
            /* Au départ, toutes les cases sont vides */
            arene->grid[i][j].ifSnake = 0;
            /* Bordures face nord */
            if(i==0){
                arene->grid[i][j].north_wall = 1; 
            }
            /* Bordures face est */
            if(j==sizeY-1){
                arene->grid[i][j].east_wall = 1;           
            }
            /* Bordures face sud */
            if(i==sizeX-1){
                arene->grid[i][j].south_wall = 1; 
            }
            /* Bordures face ouest */
            if(j==0){
                arene->grid[i][j].west_wall = 1; 
            }
        }
    }

    /* Création des murs internes */
    for(int i=0; i<nbWalls; i=i+1){
        /* Pour un mur horizontal entre deux cases */
        if((wall[4*i]==wall[4*i+2]) && ((wall[4*i+3]==wall[4*i+1]+1) || (wall[4*i+1]==wall[4*i+2]+1))){
            arene->grid[wall[4*i]][wall[4*i+1]].east_wall = 1;
            arene->grid[wall[4*i+2]][wall[4*i+3]].west_wall = 1; 
        }
        /* Pour un mur vertical entre deux cases */
        if((wall[4*i+1]==wall[4*i+3]) && ((wall[4*i+2]==wall[4*i]+1) || (wall[4*i]==wall[4*i+2]+1))){
            arene->grid[wall[4*i]][wall[4*i+1]].south_wall = 1; 
            arene->grid[wall[4*i+2]][wall[4*i+3]].north_wall = 1; 
        }
    }

    return arene; 

}


void free_arena(Arena* arene){
    if(arene == NULL){
        return; 
    }
    else{
        for(int i=0; i<(*arene).length; i++){
            free((*arene).grid[i]); 
        }
        free(arene); 
    }
}



void print_walls(Arena* arene){
    for(int i=0; i<(arene->length); i=i+1){
        for(int j=0; j<(arene->width); j=j+1){
            printf("arene[%d, %d]: [%d, %d, %d, %d]; %d\n", i, j, arene->grid[i][j].north_wall, arene->grid[i][j].east_wall, arene->grid[i][j].south_wall, arene->grid[i][j].west_wall, arene->grid[i][j].ifSnake);
        }
    }
}



Snake_head init_snake(Arena* arene, bool id){
    if(id == true){
        Snake_head p0 = (Node*)malloc(sizeof(Node)); 
        /* Initialisation de la tête du joueur */
        p0->coordonnee_x = 2; 
        p0->coordonnee_y = (arene->width)/2; 
        p0->next = NULL; 
        return p0; 
    }
    else{
        Snake_head p1 = (Node*)malloc(sizeof(Node));  
        /* Initialisation de la tête du bot */
        p1->coordonnee_x = (arene->length) - 3;
        p1->coordonnee_y = (arene->width)/2; 
        p1->next = NULL; 
        return p1; 
    }
}



void delete_tail(Snake_head head){
    if((head == NULL) || (head->next == NULL)){
        return; 
    }
    else{
        Node* to_delete = head; 
        while(to_delete->next != NULL){
            to_delete = to_delete->next;
        }
        free(to_delete); 
    }
}



void delete_snake(Snake_head head){
    // printf("%p %p \n",head,head->next);
    if(head == NULL){
        printf("passs bon\n");
    } 
    else if(head->next == NULL){        
        free(head);
    }
    else{
        delete_snake(head->next); 
    }
}



void snake_move(t_move m, Snake_head head, int turn){
    /* Déclaration de la nouvelle tête */
    Snake_head new_head = (Node*)malloc(sizeof(Node));

    if(m == NORTH){
        new_head->coordonnee_x = head->coordonnee_x - 1;
        new_head->coordonnee_y = head->coordonnee_y;
    }
    else if(m == EAST){
        new_head->coordonnee_x = head->coordonnee_x;
        new_head->coordonnee_y = head->coordonnee_y + 1;
    } 
    else if(m == SOUTH){
        new_head->coordonnee_x = head->coordonnee_x + 1;
        new_head->coordonnee_y = head->coordonnee_y;
    }
    else if(m == WEST){
        new_head->coordonnee_x = head->coordonnee_x;
        new_head->coordonnee_y = head->coordonnee_y - 1; 
    }

    /* On fait pointer head vers la nouvelle tête */
    head = new_head; 

    /* Si le numéro du tour n'est pas un multiple de 10, le serpent ne doit pas grandir */
    if(!(turn%10 == 0)){
        delete_tail(head); 
    }
}



void update_arena(Arena* arene, Snake_head player, Snake_head bot){
    Snake_head travel_player; 
    Snake_head travel_bot; 
    for(int i=0; i<(arene->length); i=i+1){
        for(int j=0; j<(arene->width); j=j+1){
            travel_player = player; 
            travel_bot = bot; 
            while(travel_player->next != NULL){
                if((i == travel_player->coordonnee_x) && (j == travel_player->coordonnee_y)){
                    arene->grid[i][j].ifSnake = 1; 
                }
            }
            if((i == travel_player->coordonnee_x) && (j == travel_player->coordonnee_y)){
                arene->grid[i][j].ifSnake = 1; 
            }
            while(travel_bot->next != NULL){
                if((i == travel_bot->coordonnee_x) && (j == travel_bot->coordonnee_y)){
                    arene->grid[i][j].ifSnake = 1; 
                }
            }
            if((i == travel_bot->coordonnee_x) && (j == travel_bot->coordonnee_y)){
                arene->grid[i][j].ifSnake = 1; 
            }            
        }
    }
}



MoveTree* initMoveNode(t_move m){
    MoveTree* move_node = (MoveTree*)malloc(sizeof(MoveTree)); 
    move_node->move_type = m;
    move_node->est_bloquant = 0; 
    move_node->father = NULL; 
    for(int i=0; i<4; i=i+1){
        move_node->direction[i] = NULL; 
    }
    return move_node; 
}



void addMoveNode(Root r, MoveTree* new_node, int f){
    if((r == NULL) || (new_node == NULL)){
        return; 
    }
    else{
        r->direction[f] = new_node; 
        new_node->father = r->direction[f]; 
    }
}



void deleteTree(Root r){
    if(r == NULL){
        return;
    }
    else{
        if(r->direction == NULL){
            free(r); 
        }
        else{
            for(int i=0; i<4; i=i+1){
                deleteTree(r->direction[i]); 
            }
        }
    }
}



int max(int a, int b, int c, int d){
    if((a>b) && (a>c) && (a>d)){
        return a; 
    }
    else if((b>a) && (b>c) && (b>d)){
        return b; 
    }
    else if((c>a) && (c>b) && (c>d)){
        return c; 
    }
    else if((d>a) && (d>b) && (d>c)){
        return d; 
    }
    else{
        /* a=b=c=d */
        return a; 
    }
}



int profondeur(Root r){
    int p = 0; 
    if(r == NULL){
        p = 0; 
    }
    else if(r->direction == NULL){
        p = 1; 
    }
    else{
        p = 1 + max(profondeur(r->direction[NORTH]), profondeur(r->direction[EAST]), profondeur(r->direction[SOUTH]), profondeur(r->direction[WEST])); 
    }
    return p; 
}



t_move bestPrediction(Root r){
    t_move best_move; 
    int p0 = profondeur(r->direction[NORTH]); 
    int p1 = profondeur(r->direction[EAST]);
    int p2 = profondeur(r->direction[SOUTH]);
    int p3 = profondeur(r->direction[WEST]); 
    if(max(p0, p1, p2, p3) == p0){
        best_move = NORTH; 
    }
    else if(max(p0, p1, p2, p3) == p1){
        best_move = EAST; 
    }
    else if(max(p0, p1, p2, p3) == p2){
        best_move = SOUTH;
    }
    else{
        best_move = WEST; 
    }
    return best_move; 
}



t_move send_move(Arena* arene, Snake_head player){

    if((player->coordonnee_x>0) && (player->coordonnee_x<arene->width) && (player->coordonnee_y>0) && (player->coordonnee_y<arene->length)){
        /* On effectue un mouvement s'il n'y a pas de mur dans la direction adressée ou si la case destination est libre */
        if((arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake==0) && (arene->grid[player->coordonnee_x][player->coordonnee_y].west_wall)){
            return NORTH;
        }
        else if((arene->grid[player->coordonnee_x+1][player->coordonnee_y].ifSnake==0) && (arene->grid[player->coordonnee_x][player->coordonnee_y].south_wall)){
            return EAST;
        }
        else if((arene->grid[player->coordonnee_x][player->coordonnee_y-1].ifSnake==0) && (arene->grid[player->coordonnee_x][player->coordonnee_y].east_wall)){
            return SOUTH; 
        }
        else{
            return WEST; 
        } 
    }
    else{
        /* Il n'y a pas de murs coupant le chemin du contour de l'arène */
        if(player->coordonnee_x == 0){
            if(player->coordonnee_y == 0){
                if(arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake == 0){
                    return NORTH; 
                }
                else{
                    return EAST; 
                }
            }
            else if(player->coordonnee_y == arene->length-1){
                if(arene->grid[player->coordonnee_x][player->coordonnee_y-1].ifSnake == 0){
                    return SOUTH; 
                }
                else{
                    return EAST; 
                }
            }
            else{
                if(arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake == 0){
                    return NORTH;
                }
                else{
                    return SOUTH; 
                }
            }
        }
        else if(player->coordonnee_x == arene->width-1){
            if(player->coordonnee_y == 0){
                if(arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake == 0){
                    return NORTH; 
                }
                else{
                    return WEST; 
                }
            }
            else if(player->coordonnee_y == arene->length-1){
                if(arene->grid[player->coordonnee_x-1][player->coordonnee_y].ifSnake == 0){
                    return WEST; 
                }
                else{
                    return SOUTH; 
                }
            }
            else{
                if(arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake == 0){
                    return NORTH;
                }
                else{
                    return SOUTH; 
                }
            }
        }
        if(player->coordonnee_y == 0){
            if(player->coordonnee_x == 0){
                if(arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake == 0){
                    return NORTH; 
                }
                else{
                    return EAST; 
                }            
            }
            else if(player->coordonnee_x == arene->width-1){
                if(arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake == 0){
                    return NORTH; 
                }
                else{
                    return WEST; 
                }
            }
            else{
                if(arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake == 0){
                    return NORTH;
                }
                else{
                    return SOUTH; 
                }
            }
        }
        else if(player->coordonnee_y == arene->length-1){
            if(player->coordonnee_x == 0){
                if(arene->grid[player->coordonnee_x][player->coordonnee_y-1].ifSnake == 0){
                    return SOUTH; 
                }
                else{
                    return EAST; 
                }
            }
            else if(player->coordonnee_x == arene->width-1){
                if(arene->grid[player->coordonnee_x-1][player->coordonnee_y].ifSnake == 0){
                    return WEST; 
                }
                else{
                    return SOUTH; 
                }
            }
            else{
                if(arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake == 0){
                    return NORTH;
                }
                else{
                    return SOUTH; 
                }
            }
        }
    }

}


// t_move send_move(Arena* arene, Snake_head player, int IQ){

//     int compteur = 0; 

//     /* Pour la racine, pas besoin d'un mouvement, il s'agit seulement d'avoir une racine pour notre arbre */
//     MoveTree* root = initMoveNode(NORTH); 
//     MoveTree* copy = root; 
//     /* Initialisation des fils de la racine à partir des données de l'arène */
//     if((arene->grid[player->coordonnee_x][player->coordonnee_y].north_wall==1) || (arene->grid[player->coordonnee_x][player->coordonnee_y-1].ifSnake==1)){
//         root->direction[NORTH]->est_bloquant = 1; 
//     }
//     if((arene->grid[player->coordonnee_x][player->coordonnee_y].east_wall==1) || (arene->grid[player->coordonnee_x+1][player->coordonnee_y].ifSnake==1)){
//         root->direction[EAST]->est_bloquant = 1; 
//     }   
//     if((arene->grid[player->coordonnee_x][player->coordonnee_y].south_wall==1) || (arene->grid[player->coordonnee_x][player->coordonnee_y+1].ifSnake==1)){
//         root->direction[SOUTH]->est_bloquant = 1; 
//     }   
//     if((arene->grid[player->coordonnee_x][player->coordonnee_y].west_wall==1) || (arene->grid[player->coordonnee_x-1][player->coordonnee_y].ifSnake==1)){
//         root->direction[WEST]->est_bloquant = 1; 
//     }    

//     /* On complète l'arbre suivant les mouvements pouvant être effectués par le serpent pour le tour actuel */
//     while(compteur < IQ){
//         for(int i=0; i<4; i=i+1){
//             if(copy->direction[i]->est_bloquant != 1){

//             }
//         }
//     }

// }


