#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "/home/viky/Desktop/code/gameaed/MangueScience/include/resource_dir.h"	


//notas
typedef struct balls{

	int type;
	int* dir;
	float posx;
	float posy;
	Texture2D sprite;
	struct balls* next;

}balls;

typedef struct songs{

	Music musica;
	int qntbeats;

}songs;

void createnextball(balls** head, int type, int* dir,Texture2D sprite, float posx, float posy){

	if(*head == NULL) {

		*head = (balls*)malloc(sizeof(balls));
		if (*head == NULL) return;
		(*head)->type = type;
		(*head)->dir = dir;
		(*head)->posx = posx;
		(*head)->posy = posy;
		(*head)->sprite = sprite;
		return;
	}

	balls* n = *head;

	while(n->next != NULL) n = n->next;
	n->next = (balls*)malloc(sizeof(balls));
	if(n->next == NULL) return;
	n->next->type = type;
	n->next->dir = dir;
	n->next->posx = posx;
	n->next->posy = posy;
	n->next->sprite = sprite;

}




int main ()
{
	
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	
	

	InitWindow(1260, 800, "Hello Raylib");
	InitAudioDevice();

	//tamanho da tela
	int screenheight = GetScreenHeight();
	int screenwidth = GetScreenWidth();

	SearchAndSetResourceDir("resources");

	Music musictest = LoadMusicStream("praiera.mp3");
	
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	PlayMusicStream(musictest);

	SetMusicVolume(musictest, 1.0);


	//posicao do jogador
	float pposx = screenwidth/2;
	float pposy = screenheight/2;

	//variaveis de onde o jogador vai apontar
	int up = 0;
	int down = 0;
	int right = 0;
	int left = 0;

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


		BeginDrawing();

		UpdateMusicStream(musictest);

		
		ClearBackground(BLACK);

		DrawText("Hello Raylib", 200,200,20,WHITE);

		if(IsMusicStreamPlaying(musictest))DrawText("aeu",300,300, 20, WHITE );

		// draw our texture to the screen
		DrawTexture(wabbit, pposx, pposy, WHITE);

		// posicao onde o jogador vai pegar as notas
		if(up)DrawRectangle(pposx, pposy - 20, 10, 2, RED);
		if(down)DrawRectangle(pposx, pposy + 60, 10, 2, RED);
		if(right)DrawRectangle(pposx + 50, pposy, 2, 10, RED);
		if(left)DrawRectangle(pposx - 20, pposy, 2, 10, RED);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	StopMusicStream(musictest);

	UnloadMusicStream(musictest);

	CloseAudioDevice();
	CloseWindow();
	return 0;
}
