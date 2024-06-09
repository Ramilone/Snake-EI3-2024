#include "fonction.h"


int main(){

    connectToServer("localhost", 1234, "Ramilone"); 

    /* Paramètre de l'arène */
    char GameName[50]; 
    int* sizeX = (int*)malloc(sizeof(int));
    int* sizeY = (int*)malloc(sizeof(int)); 
    int nbWalls; 

    /* Création de la partie */
    waitForSnakeGame("TRAINING RANDOM_PLAYER seed=2002", GameName, sizeX, sizeY, &nbWalls); 
    
    /* Récupération des murs de l'arène et le joueur qui commence */
    int* walls = (int*)malloc(sizeof(int)*4*nbWalls); 
    int priority = getSnakeArena(walls); 
    /* Affichage de celle-ci */
    printArena(); 

    /* Une fois l'arène et les murs obtenus, on peut en créer une copie pour notre IA */
    Arena* arene = initArena(nbWalls, walls, *sizeX, *sizeY); 
    /* On crée également les copies des serpents en faisant attention à qui commence pour savoir qui placer où */
    Snake_head player; 
    Snake_head bot; 
    if(!priority){
        player = init_snake(arene, true); 
        bot = init_snake(arene, false);        
    }
    else{
        player = init_snake(arene, false); 
        bot = init_snake(arene, true); 
    } 
    /* On initialise l'arène avec la position initiale des serpents */
    update_arena(arene, player, bot); 
    print_walls(arene); 

    /* Boucle de jeu: tant que les deux joueurs font des coups normaux, la partie continue */
    int turn = 0; 
    t_move player_move;
    t_return_code player_coup; 
    t_move bot_move; 
    t_return_code bot_coup; 
    
    do{
        if(!priority){
            /* Envoie du coup du joueur */
            player_move = send_move(arene, player); 
            player_coup = sendMove(player_move); 
            snake_move(player_move, player, turn); 
            /* On met à jour l'arène après ce coup */
            update_arena(arene, player, bot); 
        }
        else if(priority){
            /* Récupération du coup du bot */
            bot_coup = getMove(&bot_move); 
            snake_move(bot_move, bot, turn); 
            /* On met à jour l'arène après ce coup */
            update_arena(arene, player, bot); 
        }
        /* Affichage de la map après un tour */
        printArena(); 
        /* On passe au joueur suivant */
        priority = !priority; 
        turn = turn + 1; 
    }
    while((player_coup != -1) || (bot_coup != -1)); 

    /* Fin de la partie, on se déconnecte et on libère la mémoire utilisée */
    closeConnection(); 
    delete_snake(player);
    delete_snake(bot); 
    free_arena(arene); 
    free(sizeX);
    free(sizeY);
    free(walls); 


    return 0; 

}