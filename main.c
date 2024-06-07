#include <string.h>
#include <stdlib.h>
#include "clientAPI.h"
#include "snakeAPI.h" 
#include "fonction.h"



int main(){

    char GameName[50]; 
    int* sizeX = (int*)malloc(sizeof(int));
    int* sizeY = (int*)malloc(sizeof(int));
    int nbWalls; 

    /* Création de la partie */
    waitForSnakeGame("TRAINING RANDOM_PLAYER", GameName, sizeX, sizeY, &nbWalls);
    int* walls = (int*)malloc(sizeof(int)*4*nbWalls); 
    printf("dimension de la fenêtre; [%d, %d]\n", *sizeX, *sizeY);   

    /* Récupération du tour du premier joueur: 0 si c'est le bot, 1 si c'est moi */
    int turn = getSnakeArena(walls); 
    
    /* Affichage de la map à l'état initial */
    printArena(); 

    /* Boucle de jeu: tant que les deux joueurs font des coups normaux */
    int player_move; 
    t_return_code player_coup;  
    t_move bot_move; 
    t_return_code bot_coup; 
    do{
        if(!turn){
            /* Récupération du tour du joueur */
            printf("Select a direction: 0=NORTH; 1=EAST; 2=SOUTH; 3=WEST\n");
            scanf("%d", &player_move);
            /* Évaluation du tour du joueur: 0 si normal, 1 si gagnant, -1 si perdant*/
            player_coup = sendMove(player_move); 
        }
        else if(turn){ 
            /* Récupération du tour du bot */
            bot_coup = getMove(&bot_move); 
        } 
        /* Affichage de la map après un tour */
        printArena(); 
        /* celui qui n'a pas joué joue */
        turn = !turn; 
    } 
    while((player_coup != -1) || (bot_coup != -1)); 

    closeConnection(); 
    printf("plus de co\n");

    /* Sortie de la boucle lorsque la partie se termine: on n'oublie pas de libérer la mémoire allouée aux pointeurs */
    free(sizeX);
    printf("la libération de sizeX c'est ok\n");
    free(sizeY);
    printf("la libération de sizeY c'est ok\n");
    free(walls);
    printf("la libération de walls c'est ok\n"); 

    return 0; 

}