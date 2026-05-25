#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "resource_dir.h"   
#include "types.h"
#include <math.h>

GameScreen currentScreen = SCREEN_MENU;
GameState gs;


void getdirectionofball(int* val){

    int dir = GetRandomValue(1,4);
    *val = dir;

}

void createnextball(balls** head, balls** tail, int type, Texture2D sprite, int dir){

    if(*head == NULL) {


        *head = (balls*)malloc(sizeof(balls));
        if (*head == NULL) return;
        (*head)->next = NULL;
        (*head)->type = type;

        int posx;
        int posy;

        (*head)->dir = dir;
        
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
    
    n->next->dir = dir;
    
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

void removeball(balls** head, balls** tail, balls* alvo) {
    if(alvo == NULL) return;

    if(alvo->prev != NULL)
        alvo->prev->next = alvo->next;
    else
        *head = alvo->next;

    if(alvo->next != NULL)
        alvo->next->prev = alvo->prev;
    else
        *tail = alvo->prev;

    free(alvo);
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

    float speed_h = (GetScreenWidth() / (float)GetScreenHeight()) * 3.0f;
    float speed_v = 3.0f;

    if((*head)->dir == 1){ 
        (*head)->rect.y += speed_v; 
        (*head)->vect.y += speed_v; 
    }
    if((*head)->dir == 2){
        (*head)->rect.y -= speed_v;
        (*head)->vect.y -= speed_v;
    }
    if((*head)->dir == 3){
        (*head)->rect.x -= speed_h;
        (*head)->vect.x -= speed_h;
    }
    if((*head)->dir == 4){
        (*head)->rect.x += speed_h;
        (*head)->vect.x += speed_h;
    }
}

int counthitballs(balls* head){

    int count = 0;
    balls* current = head;

    while(current != NULL){

        if(current->check == 1){
            count++;
        }

        current = current->next;
    }

    return count;
}

int countmissedballs(balls* head){

    int count = 0;
    balls* current = head;

    while(current != NULL){

        bool saiu = (
            current->vect.x < -100 ||
            current->vect.x > GetScreenWidth() + 100 ||
            current->vect.y < -100 ||
            current->vect.y > GetScreenHeight() + 100
        );

        if(saiu && current->check == 0){
            count++;
        }

        current = current->next;
    }

    return count;
}

int countlines(const char* path) {
    int c;
    int count = 0;

    FILE *read = fopen(path, "r");
    if(read == NULL) {
        TraceLog(LOG_ERROR, "Beatmap nao encontrado: %s", path);
        return 0;
    }

    while((c = fgetc(read)) != EOF){
        if(c == '\n') count++;
    }

    fclose(read);
    return count;
}


int main (){
    
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    SetRandomSeed(10);
    
    
    InitWindow(1240, 800, "Hello Raylib");
    ToggleFullscreen();
    InitAudioDevice();

    int test[2];

    //tamanho da tela
    int screenheight = GetScreenHeight();
    int screenwidth = GetScreenWidth();

    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    RenderTexture2D target = LoadRenderTexture(1240, 800);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    SetWindowSize(monitorWidth, monitorHeight);
    if (!IsWindowFullscreen()) {
        ToggleFullscreen();
    }

    SearchAndSetResourceDir("resources");

    songs playlist[5];

    playlist[0].musica = LoadMusicStream("music/praieira.mp3");
    playlist[0].qntbeats = 452;
    playlist[0].title = "Praieira - Chico Science & Nation Zumbi";
    playlist[0].offset = 0.0f;

    playlist[1].musica = LoadMusicStream("music/maracatu_atomico.mp3");
    playlist[1].qntbeats = 380;
    playlist[1].title = "Maracatu Atômico - Chico Science & Nation Zumbi";

    playlist[2].musica = LoadMusicStream("music/da_lama_ao_caos.mp3");
    playlist[2].qntbeats = 510;
    playlist[2].title = "Da Lama ao Caos - Chico Science & Nation Zumbi";

    playlist[3].musica = LoadMusicStream("music/manguetown.mp3");
    playlist[3].qntbeats = 452;
    playlist[3].title = "Manguetown - Chico Science & Nation Zumbi";
    playlist[3].offset = 0.0f;

    playlist[4].musica = LoadMusicStream("music/rios_pontes_e_overdrives.mp3");
    playlist[4].qntbeats = 380;
    playlist[4].title = "Rios, Pontes e Overdrives - Chico Science & Nation Zumbi";


    const char* beatmaps[] = {
        "../tools/praieira.beatmap",
        "../tools/maracatu_atomico.beatmap",
        "../tools/da_lama_ao_caos.beatmap",
        "../tools/manguetown.beatmap",
        "../tools/rios_pontes_e_overdrives.beatmap"
    };

    int selectedSong = 0;
    int pontuacaoAtual = 0;
    int totalSongs = 5;

	Sound hit = LoadSound("hit.mp3");

    balls* head = NULL;
    balls* tail = NULL;
    balls* aux = NULL;
    balls* n = NULL;



    Texture2D noteTextures[3];
    noteTextures[0] = LoadTexture("notes/notamusical1.png");
    noteTextures[1] = LoadTexture("notes/notamusical2.png");
    noteTextures[2] = LoadTexture("notes/notamusical3.png");

    gs.skins[0] = LoadTexture("characters/skin1.png");
    gs.skins[1] = LoadTexture("characters/skin2.png");
    gs.skins[2] = LoadTexture("characters/skin3.png");

    // seleção
    gs.skinsSelect[0] = LoadTexture("characters/skin1_v2.png");
    gs.skinsSelect[1] = LoadTexture("characters/skin2.png");
    gs.skinsSelect[2] = LoadTexture("characters/skin3.png");
    gs.selectedSkin = 0;

    gs.backgrounds[SCREEN_MENU] = LoadTexture("backgrounds/bg_menu.png");
    gs.backgrounds[SCREEN_CHARACTER_SELECT] = LoadTexture("backgrounds/bg_character_select.png"); 
    gs.backgrounds[SCREEN_GAMEPLAY] = LoadTexture("backgrounds/bg_gameplay.png");
    gs.backgrounds[SCREEN_SCORE] = LoadTexture("backgrounds/bg_score.png");

    gs.fonte = LoadFontEx("fonts/PressStart2P-Regular.ttf", 64, NULL, 0);

    //posicao do jogador
    float pposx = screenwidth/2;
    float pposy = screenheight/2;

    Rectangle playerrect;

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


    int qtd_notes = 0;
    float* beatmap_music = NULL;
    int* note_dirs = NULL;
    float dummy1, dummy2;
    int dummy3;
    int next_note = 0;
    int comboAtual = 0;
    int maxCombo = 0;
    int notasAcertadas = 0;
    int notasPassadas = 0;

    float dist_horizontal = GetScreenWidth() / 2.0f;
    float dist_vertical = GetScreenHeight() / 2.0f;
    float ball_speed = 3.0f * 60.0f;

    // game loop

    LoadRanking();

    while (!WindowShouldClose()){

        if (currentScreen == SCREEN_MENU) {

            if (IsKeyPressed(KEY_ENTER) || menuButtonClicked) {
                currentScreen = SCREEN_CHARACTER_SELECT;;
                menuButtonClicked = false;
            }

        } 
        else if(currentScreen == SCREEN_CHARACTER_SELECT){

            int result = UpdateCharacterSelect();

            if(result == 1){
                currentScreen = SCREEN_SONG_SELECT;
            }

        }
        else if(currentScreen == SCREEN_SONG_SELECT){

            int result = UpdateSongSelect(totalSongs, &selectedSong);

            if(result >= 0) {
                // carrega beatmap e muda de tela
                deleteeverything(&head, &tail);
                    
                if(beatmap_music != NULL) free(beatmap_music);

                    qtd_notes = countlines(beatmaps[selectedSong]);
                    beatmap_music = malloc(qtd_notes * sizeof(float));

                    FILE *f = fopen(beatmaps[selectedSong], "r");
                    if(f == NULL) {
                        TraceLog(LOG_ERROR, "Erro ao abrir beatmap: %s", beatmaps[selectedSong]);
                        currentScreen = SCREEN_SONG_SELECT;
                    } else {
                        for(int i = 0; i < qtd_notes; i++) {
                            fscanf(f, "%f %f %f %d", &beatmap_music[i], &dummy1, &dummy2, &dummy3);
                        }
                        fclose(f);

                        if(note_dirs != NULL) free(note_dirs);
                        note_dirs = malloc(qtd_notes * sizeof(int));
                        for(int i = 0; i < qtd_notes; i++){
                            note_dirs[i] = GetRandomValue(1, 4);
                        }

                        next_note = 0;
                        musicStarted = false;
                        aux = head;
                        n = head;
                        pontuacaoAtual = 0;
                        comboAtual = 0;
                        maxCombo = 0;
                        notasAcertadas = 0;
                        notasPassadas = 0;
                        currentScreen = SCREEN_GAMEPLAY;
                    }
            }
                   
        }
        else if (currentScreen == SCREEN_GAMEPLAY) {

            
            if (!musicStarted) {
                PlayMusicStream(playlist[selectedSong].musica);
                SetMusicVolume(playlist[selectedSong].musica, 1.0);
                musicStarted = true;            
            }

            if(next_note < qtd_notes){

                int dir = note_dirs[next_note];
                float dist = (dir == 1 || dir == 2) ? dist_vertical : dist_horizontal;
                float lead_time = dist / ball_speed;
                float spawn_time = fmaxf(0.0f, beatmap_music[next_note] - lead_time);

                if(GetMusicTimePlayed(playlist[selectedSong].musica) >= spawn_time){
                    createnextball(&head, &tail, 0, noteTextures[next_note % 3], dir);
                    notasPassadas++;
                    if(n == NULL) n = head;
                    if(aux == NULL) aux = head;
                    next_note++;
                }

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

            float raio = 60.0f;

            if(up){
                playertablet.x = pposx - 15;
                playertablet.y = pposy - raio - 15;
            }else if(down){
                playertablet.x = pposx - 15;
                playertablet.y = pposy + raio - 15;
            }else if(right){
                playertablet.x = pposx + raio - 15;
                playertablet.y = pposy - 15;
            }else{
                playertablet.x = pposx - raio - 15;
                playertablet.y = pposy - 15;
            }


            if(IsKeyPressed(KEY_SPACE) && aux != NULL && aux->next != NULL) aux = aux->next;

            if(IsKeyPressed(KEY_BACKSPACE) && aux != NULL && aux->prev != NULL) aux = aux->prev;

            // Mecânica de pausar a música integrada dinamicamente com a playlist
            if(IsMusicStreamPlaying(playlist[selectedSong].musica)) {
                if(IsKeyPressed(KEY_ONE)) PauseMusicStream(playlist[selectedSong].musica);
            } else {
                if(IsKeyPressed(KEY_ONE)) ResumeMusicStream(playlist[selectedSong].musica);
            }


			if(n != NULL && CheckCollisionRecs(n->rect, playerrect) && n->check != 0){
                
                if(n->next != NULL) n = n->next;
            }

			if(n != NULL && (up || down || right || left) && CheckCollisionRecs(n->rect, playertablet) && n->check == 0){
                PlaySound(hit);
                n->check++;
                pontuacaoAtual += 100;

                notasAcertadas++;
                comboAtual++;
                if(comboAtual > maxCombo) maxCombo = comboAtual;
            }

			if(n != NULL && n->check == 0) {

                float dx = n->vect.x - pposx;
                float dy = n->vect.y - pposy;
                float distancia = sqrtf(dx*dx + dy*dy);

                if(distancia <= 60.0f) {
                    n->check++;
                    if(n->next != NULL) n = n->next;
                }
            }		

            if(n != NULL) {

                bool saiu = (n->vect.x < -100 || n->vect.x > GetScreenWidth() + 100 || n->vect.y < -100 || n->vect.y > GetScreenHeight() + 100);

                if(saiu) {

                    if (n->check == 0) { 
                        comboAtual = 0;
                    }

                    balls* remover = n;

                    if(n->next != NULL)
                        n = n->next;
                    else
                        n = NULL;

                    removeball(&head, &tail, remover);
                }
            }

            // se n ficou preso na mesma nota que já saiu, tenta avançar
            if(n != NULL && n->check != 0 && n->next != NULL) {
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
        else if(currentScreen == SCREEN_SCORE){

            int result = UpdateScoreSystem(pontuacaoAtual, playlist[selectedSong].title);
            if(result == 1) currentScreen = SCREEN_MENU;

        }


        BeginDrawing();

            if (currentScreen == SCREEN_MENU) {
                menuButtonClicked = DrawMenu();
            } 
            else if(currentScreen == SCREEN_CHARACTER_SELECT){

                DrawCharacterSelect();

            }
            else if(currentScreen == SCREEN_SONG_SELECT){

                DrawSongSelect(playlist, totalSongs, selectedSong);

            } 
            else if (currentScreen == SCREEN_GAMEPLAY){
                
                ClearBackground(WHITE);

                DrawText("Hello Raylib", 200,200,20,WHITE);

                if (IsMusicStreamPlaying(playlist[selectedSong].musica)) {
                    DrawText(TextFormat("Tocando: %s", playlist[selectedSong].title), 300, 300, 20, GREEN);
                }


                // Círculos concêntricos e alvos redondos no meio da tela
                DrawCircle(pposx, pposy, 150, YELLOW);
                DrawCircle(pposx, pposy, 100, GREEN);
                DrawCircle(pposx, pposy, 60, BLUE);

                gs.frameCounter++;
                if(gs.frameCounter >= (60/4)){
                    gs.frameCounter = 0;
                    gs.currentFrame++;
                    if(gs.currentFrame >= 16) gs.currentFrame = 0;
                }

                int frameWidth = gs.skins[gs.selectedSkin].width / 16;

                Rectangle frameRec = { gs.currentFrame * frameWidth, 
                    0, frameWidth, 
                    (float)gs.skins[gs.selectedSkin].height };

                Rectangle destRec = { pposx - (frameWidth * 2.0f)/2, 
                    pposy - (gs.skins[gs.selectedSkin].height * 2.0f)/2, 
                    frameWidth * 2.0f, 
                    gs.skins[gs.selectedSkin].height * 2.0f };

                DrawTexturePro(gs.skins[gs.selectedSkin], frameRec, destRec, (Vector2){0,0}, 0.0f, WHITE);


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

				if(n != NULL && n->check == 0 && n->vect.x > 100 && n->vect.x < GetScreenWidth() - 100 && n->vect.y > 100 && n->vect.y < GetScreenHeight() - 100){
                    float tamanho = 64.0f;
                    DrawTexturePro(
                        n->sprite,
                        (Rectangle){ 0, 0, n->sprite.width, n->sprite.height },
                        (Rectangle){ n->vect.x, n->vect.y, tamanho, tamanho },
                        (Vector2){ tamanho/2, tamanho/2 },
                        0.0f,
                        WHITE
                    );
                }           

                if(up)DrawRectangleRec(playertablet, RED);
                if(down)DrawRectangleRec(playertablet, RED);
                if(right)DrawRectangleRec(playertablet, RED);
                if(left)DrawRectangleRec(playertablet, RED);

                
                DrawCircleV(GetMousePosition(), 4, DARKGRAY);
                DrawText(TextFormat("X: %i  Y: %i",GetMouseX(),GetMouseY()),GetMousePosition().x, GetMousePosition().y, 20, RED);
            }
            else if(currentScreen == SCREEN_SCORE){

                float accuracyReal = 0.0f;
                if (notasPassadas > 0) {
                    accuracyReal = ((float)notasAcertadas / notasPassadas) * 100.0f;
                }

                DrawScoreSystem(
                    pontuacaoAtual, 
                    accuracyReal,   
                    maxCombo        
                );

            }


        EndDrawing();
    }

    UnloadFont(gs.fonte);
    if(note_dirs != NULL) free(note_dirs);

    for(int i = 0; i < 3; i++){
        UnloadTexture(gs.skins[i]);
    }

    for(int i = 0; i < 3; i++){
        UnloadTexture(gs.skinsSelect[i]);
    }

    for(int i = 0; i < 3; i++){
        UnloadTexture(noteTextures[i]);
    }

    for(int i = 0; i < 5; i++){
        UnloadTexture(gs.backgrounds[i]);
    }

    for(int i = 0; i < totalSongs; i++) {
        StopMusicStream(playlist[i].musica);
        UnloadMusicStream(playlist[i].musica);
    }

    deleteeverything(&head, &tail);

    if(beatmap_music != NULL) free(beatmap_music);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}