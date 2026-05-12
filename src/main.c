#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "/home/viky/Desktop/code/gameaed/MangueScience/include/resource_dir.h"	


//notas
typedef struct balls{

	int type;
	//1 - up; 2 - down; 3 - right; 4 - left
	//quando vcs acharam que precisar mudar algo na lista nao esquecam de mudar na funcao de inicializacao
	//ORDEM DE DIRECOES UP DOWN RIGHT LEFT
	int dir;
	float posx;
	float posy;
	Texture2D sprite;
	struct balls* next;
	struct balls* prev;

}balls;

typedef struct songs{

	Music musica;
	int qntbeats;

}songs;
void getdirectionofball(int* val){

	int dir = GetRandomValue(1,4);
	

	*val = dir;



}

void moveball(float *posx, float* posy){

}


void createnextball(balls** head,balls** tail, int type,Texture2D sprite, float posx, float posy){

	if(*head == NULL) {

		*head = (balls*)malloc(sizeof(balls));
		if (*head == NULL) return;
		(*head)->next = NULL;
		(*head)->type = type;

		getdirectionofball(&(*head)->dir);
		
		(*head)->posx = posx;
		(*head)->posy = posy;
		(*head)->sprite = sprite;
		*tail = *head;
		(*head)->prev = NULL;
		return;
	}

	balls* n = *tail;
	n->next = (balls*)malloc(sizeof(balls));
	if(n->next == NULL) return;
	n->next->type = type;
	
	getdirectionofball(&(n->next->dir));
	
	n->next->posx = posx;
	n->next->posy = posy;
	n->next->sprite = sprite;
	n->next->next = NULL;
	n->next->prev = n;
	*tail = n->next;

}

void deleteeverything(balls** head, balls** tail){

	balls* n = *head;

	while(n != NULL){

		UnloadTexture(n->sprite);
		balls* aux = n;
		n = n->next;
		free(aux);

	}
}






int main ()
{
	
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	SetRandomSeed(10);
	
	
	InitWindow(1280, 800, "Hello Raylib");
	InitAudioDevice();


	int test[2];

	//tamanho da tela
	int screenheight = GetScreenHeight();
	int screenwidth = GetScreenWidth();

	SearchAndSetResourceDir("resources");


	//definicao de musicas
	songs praiera;
	praiera.musica = LoadMusicStream("praiera.mp3");
	praiera.qntbeats = 452; //tentar fazer com que isso seja relacionado com a quantidade de tempo da musica GetMusicTimeLenght();
	PlayMusicStream(praiera.musica);
	SetMusicVolume(praiera.musica, 1.0);
	// musica 1 acima

	balls* head = NULL;
	balls* tail = NULL;
	balls* aux = NULL;

	

	for(int i = 0; i < praiera.qntbeats; i++){
		//random da funcao n ta funcionando
		createnextball(&head,&tail, 0,LoadTexture("balltest.png"),0,0);

	}

	

	//textura do jogador
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	//posicao do jogador
	float pposx = screenwidth/2;
	float pposy = screenheight/2;

	//variaveis de onde o jogador vai apontar
	int up = 0;
	int down = 0;
	int right = 0;
	int left = 0;


	aux = head;
	// game loop
	while (!WindowShouldClose())		
	{

		if(IsKeyPressed(KEY_UP)) {
			down = 0;
			right = 0;
			left = 0;
			up = 1;
		}

		if(IsKeyPressed(KEY_DOWN)) {
			down = 1;
			right = 0;
			left = 0;
			up = 0;
		}

		if(IsKeyPressed(KEY_RIGHT)) {
			down = 0;
			right = 1;
			left = 0;
			up = 0;
		}

		if(IsKeyPressed(KEY_LEFT)) {
			down = 0;
			right = 0;
			left = 1;
			up = 0;
		}

		if(IsKeyPressed(KEY_SPACE) && aux->next != NULL ) aux = aux->next;


		BeginDrawing();

		UpdateMusicStream(praiera.musica);

		
		ClearBackground(BLACK);

		DrawText("Hello Raylib", 200,200,20,WHITE);

		if(IsMusicStreamPlaying(praiera.musica))DrawText("aeu",300,300, 20, WHITE );


		DrawTexture(wabbit, pposx, pposy, WHITE);

		

		if(aux->dir == 1)DrawText("cima",400, 400, 20, WHITE);
		if(aux->dir == 2)DrawText("baixo",400, 400, 20, WHITE);
		if(aux->dir == 3)DrawText("direita",400, 400, 20, WHITE);
		if(aux->dir == 4)DrawText("esquerda",400, 400, 20, WHITE);


		// posicao onde o jogador vai pegar as notas
		if(up)DrawRectangle(pposx, pposy - 20, 10, 2, RED);
		if(down)DrawRectangle(pposx, pposy + 60, 10, 2, RED);
		if(right)DrawRectangle(pposx + 50, pposy, 2, 10, RED);
		if(left)DrawRectangle(pposx - 20, pposy, 2, 10, RED);
		
	
		EndDrawing();
	}


	UnloadTexture(wabbit);
	deleteeverything(&head, &tail);
	StopMusicStream(praiera.musica);

	UnloadMusicStream(praiera.musica);

	CloseAudioDevice();
	CloseWindow();
	return 0;
}
