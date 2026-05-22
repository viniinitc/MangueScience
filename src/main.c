#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "resource_dir.h"   
#include "types.h"
#include <math.h>

GameScreen currentScreen = SCREEN_MENU;

extern bool DrawMenu(void);
extern int UpdateSongSelect(int totalSongs, int* selectedSong);

void getdirectionofball(int* val){

    int dir = GetRandomValue(1,4);
    *val = dir;

}

void createnextball(balls** head,balls** tail, int type,Texture2D sprite){

    if(*head == NULL) {


        *head = (balls*)malloc(sizeof(balls));
        if (*head == NULL) return;
        (*head)->next = NULL;
        (*head)->type = type;

        int posx;
        int posy;

        getdirectionofball(&(*head)->dir);
        
        if((*head)->dir == 1){
            posx = GetScreenWidth()/2;
            posy = 0;
        }
        if((*head)->dir == 2){
            posx = GetScreenWidth()/2;
            posy = GetScreenHeight();
        }
        if((*head)->dir == 3){
            posx = GetScreenWidth();
            posy = GetScreenHeight()/2;
        }
        if((*head)->dir == 4){
            posx = 0;
            posy = GetScreenHeight()/2;
        }
        
        (*head)->rect.x = posx;
        (*head)->rect.y = posy;
        (*head)->rect.height = sprite.height;
        (*head)->rect.width = sprite.width;
        (*head)->outsiderect.height = sprite.height + 20;
        (*head)->outsiderect.width = sprite.width + 20;
        (*head)->outsiderect.x = posx;
        (*head)->outsiderect.y = posy;
        (*head)->sprite = sprite;
        (*head)->vect.x = posx;
        (*head)->vect.y = posy;
		(*head)->check = 0;
        *tail = *head;
        (*head)->prev = NULL;
        return;
    }

    balls* n = *tail;
    n->next = (balls*)malloc(sizeof(balls));
    if(n->next == NULL) return;
    n->next->type = type;
    
    getdirectionofball(&(n->next->dir));
    
    int posx;
    int posy;

    if(n->next->dir == 1){
            posx = GetScreenWidth()/2;
            posy = 0;
        }
        if(n->next->dir == 2){
            posx = GetScreenWidth()/2;
            posy = GetScreenHeight();
        }
        if(n->next->dir == 3){
            posx = GetScreenWidth();
            posy = GetScreenHeight()/2;
        }
        if(n->next->dir == 4){
            posx = 0;
            posy = GetScreenHeight()/2;
        }

    n->next->rect.x = posx;
    n->next->rect.y = posy;
    n->next->rect.height = sprite.height;
    n->next->rect.width = sprite.width;
    n->next->outsiderect.height = sprite.height + 20;
    n->next->outsiderect.width = sprite.width + 20;
    n->next->outsiderect.x = posx;
    n->next->outsiderect.y = posy;
    n->next->sprite = sprite;
    n->next->vect.x = posx;
    n->next->vect.y = posy;
    n->next->next = NULL;
	n->next->check = 0;
    n->next->prev = n;
    *tail = n->next;

}

void deleteeverything(balls** head, balls** tail){

    balls* n = *head;

    while(n != NULL){

        //UnloadTexture(n->sprite);
        balls* aux = n;
        n = n->next;
        free(aux);

    }
    *head = NULL;
    *tail = NULL;
}

//movimento das notas
void moveballs(balls** head){

    if((*head)->dir == 1){
            (*head)->rect.y+=3;
            (*head)->vect.y+=3;

    }
    if((*head)->dir == 2){
            (*head)->rect.y-=3;
            (*head)->vect.y-=3;
    }
    if((*head)->dir == 3){
            (*head)->rect.x-=3;
            (*head)->vect.x-=3;
    }
    if((*head)->dir == 4){
            (*head)->rect.x+=3;
            (*head)->vect.x+=3;
    }

}

int countlines(const char* path) {
    int c;
    int count = 0;

    FILE *read = fopen(path, "r");

    while((c = fgetc(read)) != EOF){
        if(c == '\n') count ++;
    }

    fclose(read);
    return count;
}


int main (){
    
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    SetRandomSeed(10);
    
    
    InitWindow(1240, 800, "Hello Raylib");
    InitAudioDevice();


    int test[2];

    //tamanho da tela
    int screenheight = GetScreenHeight();
    int screenwidth = GetScreenWidth();

    SearchAndSetResourceDir("resources");

    songs playlist[3];

    playlist[0].musica = LoadMusicStream("praiera.mp3");
    playlist[0].qntbeats = 452;
    playlist[0].title = "Praiera - Chico Science & Nation Zumbi";

    playlist[1].musica = LoadMusicStream("maracatu_atomico.mp3");
    playlist[1].qntbeats = 380;
    playlist[1].title = "Maracatu Atomico";

    playlist[2].musica = LoadMusicStream("da_lama_ao_caos.mp3");
    playlist[2].qntbeats = 510;
    playlist[2].title = "Da Lama ao Caos";

    const char* beatmaps[] = {
        "../tools/praiera.beatmap",
        "../tools/maracatu_atomico.beatmap",
        "../tools/da_lama_ao_caos.beatmap"
    };

    int selectedSong = 0;
    int totalSongs = 3;

	Sound hit = LoadSound("hit.mp3");

    balls* head = NULL;
    balls* tail = NULL;
    balls* aux = NULL;
    balls* n = NULL;

    Texture2D ballTexture = LoadTexture("balltest.png");
    //textura do jogador
    Texture wabbit = LoadTexture("wabbit_alpha.png");

    //posicao do jogador
    float pposx = screenwidth/2;
    float pposy = screenheight/2;

    Rectangle playerrect;

    playerrect.height = wabbit.height;
    playerrect.width = wabbit.width;
    playerrect.x = pposx;
    playerrect.y = pposy;

    //variaveis de onde o jogador vai apontar
    int up = 0;
    int down = 0;
    int right = 0;
    int left = 0;

    bool musicStarted = false;

    aux = head;
    n = head;

    bool menuButtonClicked = false;
	Rectangle playertablet;


    int qtd = 0;
    float* beatmap_music = NULL;
    float dummy1, dummy2;
    int dummy3;
    int next_note = 0;

    float max_distance = fmaxf(screenwidth / 2.0f, screenheight / 2.0f); //pega a maior distância entre a borda e o centro da tela
    float ball_speed = 3.0f * 60.0f; // velocidade da bola em pixels por segundo
    float lead_time = max_distance / ball_speed; // tempo em segundos que a bola leva para percorrer do spawn até o centro

    // game loop
    while (!WindowShouldClose()){

        if (currentScreen == SCREEN_MENU) {

            if (IsKeyPressed(KEY_ENTER) || menuButtonClicked) {
                currentScreen = SCREEN_SONG_SELECT;
                menuButtonClicked = false;
            }

        } else if(currentScreen == SCREEN_SONG_SELECT){

            int result = UpdateSongSelect(totalSongs, &selectedSong);
            if(result >= 0) {
                // carrega beatmap e muda de tela
                deleteeverything(&head, &tail);
                    
                if(beatmap_music != NULL) free(beatmap_music);

                    qtd = countlines(beatmaps[selectedSong]);
                    beatmap_music = malloc(qtd * sizeof(float));

                    FILE *f = fopen(beatmaps[selectedSong], "r");
                    for(int i = 0; i < qtd; i++) {
                        fscanf(f, "%f %f %f %d", &beatmap_music[i], &dummy1, &dummy2, &dummy3);
                    }
                    fclose(f);

                    next_note = 0;
                    musicStarted = false;
                    aux = head;
                    n = head;
                    currentScreen = SCREEN_GAMEPLAY;
                }
                   
        }
        else if (currentScreen == SCREEN_GAMEPLAY) {

            
            if (!musicStarted) {
                PlayMusicStream(playlist[selectedSong].musica);
                SetMusicVolume(playlist[selectedSong].musica, 1.0);
                musicStarted = true;            
            }

            float spawn_time = fmaxf(0.0f, beatmap_music[next_note] - lead_time); // calcula o tempo em q a bola deve aparecer na tela

            if(GetMusicTimePlayed(playlist[selectedSong].musica) >= spawn_time && next_note < qtd){
                createnextball(&head, &tail, 0, ballTexture);
                if(n == NULL) n = head;
                if(aux == NULL) aux = head;
                next_note++;
            }

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


            if(IsKeyReleased(KEY_UP)) {
                down = 0;
                right = 0;
                left = 0;
                up = 0;
            }

            if(IsKeyReleased(KEY_DOWN)) {
                down = 0;
                right = 0;
                left = 0;
                up = 0;
            }

            if(IsKeyReleased(KEY_RIGHT)) {
                down = 0;
                right = 0;
                left = 0;
                up = 0;
            }

            if(IsKeyReleased(KEY_LEFT)) {
                down = 0;
                right = 0;
                left = 0;
                up = 0;
            }

            
            playertablet.height = 30;
            playertablet.width = 30;

            if(up){

                    playertablet.x = pposx;
                    playertablet.y = pposy - 60;

            }else if(down){
                    playertablet.x = pposx;
                    playertablet.y = pposy + 80;
            }else if(right){
                    playertablet.x = pposx + 80;
                    playertablet.y = pposy - 10;
            }else{
                    playertablet.x = pposx -60;
                    playertablet.y = pposy - 10;
            }


            if(IsKeyPressed(KEY_SPACE) && aux != NULL && aux->next != NULL) aux = aux->next;

            if(IsKeyPressed(KEY_BACKSPACE) && aux != NULL && aux->prev != NULL) aux = aux->prev;

            // Mecânica de pausar a música integrada dinamicamente com a playlist
            if(IsMusicStreamPlaying(playlist[selectedSong].musica)) {
                if(IsKeyPressed(KEY_ONE)) PauseMusicStream(playlist[selectedSong].musica);
            } else {
                if(IsKeyPressed(KEY_ONE)) ResumeMusicStream(playlist[selectedSong].musica);
            }

            if(n != NULL && n->prev != NULL && CheckCollisionRecs(n->rect,n->prev->rect)){
                
                
            }


			if(n != NULL && CheckCollisionRecs(n->rect, playerrect) && n->check != 0){
                
                n = n->next;
            }

			if(n != NULL && CheckCollisionRecs(n->rect, playertablet) && n->check==0){

				PlaySound(hit);
				n->check++;
			}

			if(n != NULL && CheckCollisionRecs(n->rect, playerrect) && n->check == 0){
				n->check++;
				n = n->next;
					
            } 

			
            


            // nota se mexendo e sendo destruida com segurança quando chega perto/colide com o jogador
            


            float musicDuration = GetMusicTimeLength(playlist[selectedSong].musica);
            float musicPlayed  = GetMusicTimePlayed(playlist[selectedSong].musica);

            if (musicPlayed >= musicDuration - 0.1f) {
                StopMusicStream(playlist[selectedSong].musica);
                currentScreen = SCREEN_SCORE;
            } else {
                UpdateMusicStream(playlist[selectedSong].musica);
            }
            
        } 


        BeginDrawing();

            if (currentScreen == SCREEN_MENU) {
                menuButtonClicked = DrawMenu();
            } 
            else if(currentScreen == SCREEN_SONG_SELECT){

                ClearBackground(BLACK);
                
                DrawText("SELECIONE SUA MUSICA", GetScreenWidth()/2 - MeasureText("SELECIONE SUA MUSICA", 30)/2, 100, 30, RAYWHITE);

                for (int i = 0; i < totalSongs; i++) {
                    int posY = 250 + (i * 60);
                    if (i == selectedSong) {
                        DrawText(TextFormat("> %s <", playlist[i].title), 200, posY, 24, GOLD);                    
                    } else {
                        DrawText(playlist[i].title, 220, posY, 24, LIGHTGRAY);
                    }
                }
                        
                DrawText("Use as SETAS para navegar e ENTER para confirmar", 200, 650, 20, GRAY);

            } 
            else if (currentScreen == SCREEN_GAMEPLAY){
                
                ClearBackground(BLACK);

                DrawText("Hello Raylib", 200,200,20,WHITE);

                if (IsMusicStreamPlaying(playlist[selectedSong].musica)) {
                    DrawText(TextFormat("Tocando: %s", playlist[selectedSong].title), 300, 300, 20, GREEN);
                }


                // Círculos concêntricos e alvos redondos no meio da tela
                DrawCircle(pposx, pposy, 100, YELLOW);
                DrawCircle(pposx, pposy, 50, GREEN);
                DrawCircle(pposx, pposy, 25, BLUE);

                DrawTexture(wabbit, pposx, pposy, WHITE);


				if(n != NULL){
                    moveballs(&n);
                    n->outsiderect.height--;
                    n->outsiderect.width--;	
                    n->outsiderect.x = n->rect.x;
                    n->outsiderect.y = n->rect.y;
                }
                	

                if(aux != NULL) {
                    if(aux->dir == 1) DrawText("cima", 400, 400, 20, WHITE);
                    if(aux->dir == 2) DrawText("baixo", 400, 400, 20, WHITE);
                    if(aux->dir == 3) DrawText("direita", 400, 400, 20, WHITE);
                    if(aux->dir == 4) DrawText("esquerda", 400, 400, 20, WHITE);
                }

				if((n != NULL && n->vect.x < screenwidth - 100 && n->vect.x > 0 + 100) && (n->vect.y > 0 + 100 && n->vect.y < screenheight-100) && n->check == 0){
                    
                    //tirei para ver se as batidas estao certas com a musica - nicole
					//DrawTextureRec(n->sprite, n->rect, n->vect, WHITE);
                    
                    
					DrawRectangleRec(n->rect, BLUE);
					
                    //tirei para ver se as batidas estao certas com a musica - nicole
                    //DrawRectangleLinesEx(n->outsiderect, 30, WHITE);

				}                

                // posicao onde o jogador vai pegar as notas (alvos vermelhos)
                if(up)DrawRectangleRec(playertablet, RED);
                if(down)DrawRectangleRec(playertablet, RED);
                if(right)DrawRectangleRec(playertablet, RED);
                if(left)DrawRectangleRec(playertablet, RED);

                


                // Debug posicional do mouse na tela
                DrawCircleV(GetMousePosition(), 4, DARKGRAY);
                DrawText(TextFormat("X: %i  Y: %i",GetMouseX(),GetMouseY()),GetMousePosition().x, GetMousePosition().y, 20, RED);
            }
            else if(currentScreen == SCREEN_SCORE){

                ClearBackground(BLACK);
                DrawText("Tela de pontuação", 200,200,20,WHITE);

            }


        EndDrawing();
    }

    UnloadTexture(wabbit);
    UnloadTexture(ballTexture);
    deleteeverything(&head, &tail);

    for(int i = 0; i < totalSongs; i++) {
        StopMusicStream(playlist[i].musica);
        UnloadMusicStream(playlist[i].musica);
    }

    if(beatmap_music != NULL) free(beatmap_music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}