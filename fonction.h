/*--------------*
 |	BENSIDHOUM  |
 |	Nicolas		|
 |	EISE TPA	|
 *--------------* 
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "snakeAPI.h"
#include "clientAPI.h"


/* Définition du type Snake */
typedef struct t_node{
    int coordonnee_x;
    int coordonnee_y;
    struct t_node* next; 
}
Node; 
typedef Node* Snake_head; 


/* Définition du type Cell */
typedef struct T_cell{
	int north_wall;
	int east_wall;
	int south_wall; 
	int west_wall; 

	int ifSnake; 
}
Cell; 


/* Définition du type Arena */
typedef struct T_Arena{
	int length; 
	int width; 
	Cell** grid; 	
}
Arena; 


/* Définition du type MoveTree */
typedef struct t_movetree{
	t_move move_type;
	bool est_bloquant; 	 
	// North: 0
	// East: 1
	// South: 2
	// West: 3
	struct t_movetree* father; 
	struct t_movetree* direction[4]; 
}
MoveTree; 
typedef MoveTree* Root; 


/* 
Entrée:
	- la longueur de l'arène
	- la largeur de l'arène
	- le nombre de murs dans l'arène (bordures extérieures exclues)
	- un tableau contenant les emplacements des murs  

Sortie:
	- renvoie un pointeur vers une arène initialisée: 
		- Les parois externes sont créées
		- les murs internes sont placés grâce au tableau de coordonnées de murs 
		- les cases sont initialisées vides 
*/
Arena* initArena(int nbWalls, int* wall, int sizeX, int sizeY); 


/*
Entrée: 
	- un pointeur vers une arène 
	- son identité: 0 pour nous et 1 pour l'adversaire 

Sortie: 
	- un pointeur vers la tête du serpent  
*/
Snake_head init_snake(Arena* arene, bool id); 


/*
Entrée: 
	- un pointeur vers une arène  

Sortie: 
	- rien, on libère la mémoire allouée à l'arène donnée en argument 
*/
void free_arena(Arena* arene); 


/*
Entrée:
	- un pointeur vers la tête du serpent 

Sortie: 
	- rien, on supprime le dernier noeud de la liste composant le serpent 
*/
void delete_tail(Snake_head head); 


/*
Entrée: 
	- un pointeur vers un serpent. 

Sortie: 
	- rien, on supprime le serpent en mémoire  
*/
void delete_snake(Snake_head head); 


/*
Entrée: 
	- le type de mouvement à effectuer 
	- un pointeur vers la tête du serpent 
	- le numéro du tour actuel 

Sortie: 
	- mise à jour des coordonnées du serpent
	- mise à jour de la taille du serpent  
*/
void snake_move(t_move m, Snake_head head, int turn); 


/*
Entrée: 
	- un pointeur vers une arène 
	- un pointeur vers la tête du serpent du joueur
	- un pointeur vers la tête du serpent du bot 

Sortie: 
	- rien, met à jour l'état des cases de l'arène en fonction de l'emplacement des serpents 
*/
void update_arena(Arena* arene, Snake_head player, Snake_head bot); 


/*
Entrée: 
	- un pointeur vers une arène 
	- un pointeur vers la tête du serpent du player  
	- un entier pour limiter le nombre d'itération de notre algorithme

Sortie: 
	- en créant un arbre quaternaire des déplacements que le serpent pourra effectuer et en le manipulant, détermine le meilleur mouvement pour que le serpent puisse survivre le plus longtemps possible  
*/
t_move send_move(Arena* arene, Snake_head player); 


/*
Entrée: 
	- un pointeur vers une arène 

Sortie: 
	- rien, affiche la liste des coordonnées des murs 
*/
void print_walls(Arena* arene); 


/* 
Entrée: 
	- un type de mouvement 

Sortie: 
	- initialise un noeud possédant l'information du mouvement 
*/
MoveTree* initMoveNode(t_move m); 


/*
Entrée: 
	- un pointeur vers le prédecesseur du noeud à ajouter 
	- un pointeur vers le noeud à ajouter 
	- l'indice du père du noeud à ajouter  

Sortie: 
	- rien, on ajoute le nouveau noeud au bon endroit 
*/
void addMoveNode(Root r, MoveTree* new_node, int father); 


/*
Entrée: 
	- un pointeur vers la racine de l'arbre à supprimer

Sortie: 
	- rien, on libère la mémoire allouée à l'arbre de déplacement
*/
void deleteTree(Root r); 


/*
Entrée: 
	- 4 entiers positifs ou nuls

Sortie: 
	- renvoie le plus grand entiers
*/
int max(int a, int b, int c, int d); 


/*
Entrée: 
	- un pointeur vers la racine d'un arbre 

Sortie: 
	- renvoie la profondeur de cet arbre
*/
int profondeur(Root r); 


/*
Entrée: 
	- un pointeur vers la racine d'un arbre

Sortie: 
	- regarde quelle est la branche de l'arbre la plus longue, et renvoie en conséquence le mouvement, fils de la racine, menant à ce chemin 
*/
t_move bestPrediction(Root r); 