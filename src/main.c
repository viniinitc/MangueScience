#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "resource_dir.h"   
#include "types.h"
#include <math.h>

GameScreen currentScreen = SCREEN_MENU;
GameState gs;


int selectedPauseOption = 0;

const char *pauseOptions[3] = {
    "CONTINUAR",
    "ESCOLHER MUSICA",
    "MENU PRINCIPAL"
};


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

        balls* aux = n;
        n = n->next;
        free(aux);

    }
    *head = NULL;
    *tail = NULL;
}


void moveballs(balls* ball, float speed){

    Vector2 centro = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

    Vector2 dir = {
        centro.x - ball->vect.x,
        centro.y - ball->vect.y
    };

    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);

    if(len > 0.0f){
        dir.x /= len;
        dir.y /= len;
    }

    ball->vect.x += dir.x * speed;
    ball->vect.y += dir.y * speed;

    ball->rect.x = ball->vect.x - 32.0f;
    ball->rect.y = ball->vect.y - 32.0f;
    ball->rect.width = 64.0f;
    ball->rect.height = 64.0f;
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

int countActiveNotes(balls* head) {
    int count = 0;
    balls* current = head;
    while(current != NULL) {
        if(current->check == 0) {
            count++;
        }
        current = current->next;
    }
    return count;
}


int main (){
    
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    SetRandomSeed(10);
    
    
    InitWindow(1240, 800, "MANGUE SCIENCE");
    // ToggleFullscreen();
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

    // SetWindowSize(monitorWidth, monitorHeight);
    // if (!IsWindowFullscreen()) {
    //     ToggleFullscreen();
    // }

    SearchAndSetResourceDir("resources");

    songs playlist[5];

    playlist[0].musica = LoadMusicStream("music/praieira.mp3");
    playlist[0].qntbeats = 452;
    playlist[0].title = "Praieira";
    playlist[0].offset = 0.0f;

    playlist[1].musica = LoadMusicStream("music/maracatu_atomico.mp3");
    playlist[1].qntbeats = 380;
    playlist[1].title = "Maracatu Atomico";

    playlist[2].musica = LoadMusicStream("music/da_lama_ao_caos.mp3");
    playlist[2].qntbeats = 510;
    playlist[2].title = "Da Lama ao Caos";

    playlist[3].musica = LoadMusicStream("music/manguetown.mp3");
    playlist[3].qntbeats = 452;
    playlist[3].title = "Manguetown";
    playlist[3].offset = 0.0f;

    playlist[4].musica = LoadMusicStream("music/rios_pontes_e_overdrives.mp3");
    playlist[4].qntbeats = 380;
    playlist[4].title = "Rios, Pontes e Overdrives";


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

	Sound somAcerto = LoadSound("music/som_acerto.mp3");
    Sound somErro   = LoadSound("music/som_erro.mp3");

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

    gs.skinsSelect[0] = LoadTexture("characters/skin1_v2.png");
    gs.skinsSelect[1] = LoadTexture("characters/skin2.png");
    gs.skinsSelect[2] = LoadTexture("characters/skin3.png");
    gs.selectedSkin = 0;

    gs.backgrounds[SCREEN_MENU] = LoadTexture("backgrounds/bg_menu.png");
    gs.backgrounds[SCREEN_CHARACTER_SELECT] = LoadTexture("backgrounds/bg_character_select.png");
    gs.backgrounds[SCREEN_SONG_SELECT] = LoadTexture("backgrounds/bg_song_select.png");
    gs.backgrounds[BG_SCORE] = LoadTexture("backgrounds/bg_score.png");

    gs.backgrounds[BG_SKIN_0] = LoadTexture("backgrounds/bg_carangueijo.png");
    gs.backgrounds[BG_SKIN_1] = LoadTexture("backgrounds/bg_crocoscience.png");
    gs.backgrounds[BG_SKIN_2] = LoadTexture("backgrounds/bg_camarao.png");

    Texture2D pauseRadio = LoadTexture("backgrounds/radio.png");

    Texture2D setaCima = LoadTexture("setas/seta_cima.png");
    Texture2D setaBaixo = LoadTexture("setas/seta_baixo.png");
    Texture2D setaDireita = LoadTexture("setas/seta_dir.png");
    Texture2D setaEsquerda = LoadTexture("setas/seta_esq.png");

    gs.fonte = LoadFontEx("fonts/PressStart2P-Regular.ttf", 64, NULL, 0);

    float pposx = screenwidth/2;
    float pposy = screenheight/2;

    Rectangle playerrect;

    playerrect.x = pposx;
    playerrect.y = pposy;

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
    float ultimo_spawn = -1.0f;
    int comboAtual = 0;
    int maxCombo = 0;
    float comboTimer = 0.0f;
    int ultimaDezenaMostrada = 0;
    int notasAcertadas = 0;
    int notasPassadas = 0;

    float dist_horizontal = GetScreenWidth() / 2.0f;
    float dist_vertical = GetScreenHeight() / 2.0f;
    float ball_speed_base = 7.0f * 60.0f;
    float ball_speed = ball_speed_base;


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

                deleteeverything(&head, &tail);
                    
                if(beatmap_music != NULL) free(beatmap_music);

                    qtd_notes = countlines(beatmaps[selectedSong]);
                    beatmap_music = malloc(qtd_notes * sizeof(float));

                    FILE *f = fopen(beatmaps[selectedSong], "r");
                    if(f == NULL) {
                        TraceLog(LOG_ERROR, "Erro ao abrir beatmap: %s", beatmaps[selectedSong]);
                        currentScreen = SCREEN_SONG_SELECT;
                    } else {
                        int real_notes = 0;

                        for(int i = 0; i < qtd_notes; i++) {

                            float tempo;
                            float d1;
                            float d2;
                            int d3;

                            fscanf(f, "%f %f %f %d",
                                &tempo,
                                &d1,
                                &d2,
                                &d3);

                            if(i % 2 == 0){

                                beatmap_music[real_notes] = tempo;

                                real_notes++;
                            }
                        }

                        qtd_notes = real_notes;
                        fclose(f);

                        if(note_dirs != NULL) free(note_dirs);
                        note_dirs = malloc(qtd_notes * sizeof(int));

                        for(int i = 0; i < qtd_notes; i++){
                            int nova_dir;
                            do {
                                nova_dir = GetRandomValue(1, 4);
                            } while (i >= 2 && nova_dir == note_dirs[i - 1] && nova_dir == note_dirs[i - 2]); 

                            note_dirs[i] = nova_dir;
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
                        ball_speed = ball_speed_base;
                        currentScreen = SCREEN_GAMEPLAY;
                    }
            }
                   
        }
        else if (currentScreen == SCREEN_GAMEPLAY) {

            if (IsKeyPressed(KEY_SPACE)) {
                PauseMusicStream(playlist[selectedSong].musica);
                currentScreen = SCREEN_PAUSED;
            }

            
            if (!musicStarted) {
                PlayMusicStream(playlist[selectedSong].musica);
                SetMusicVolume(playlist[selectedSong].musica, 1.0);
                musicStarted = true;            
            }

            
            if(next_note < qtd_notes){
                int dir = note_dirs[next_note];
                float distance = 0.0f;

                if(dir == 1 || dir == 2){ 
                    distance = (GetScreenHeight() / 2.0f) - 80.0f;
                } else { 
                    distance = (GetScreenWidth() / 2.0f) - 80.0f;
                }

                float lead_time = distance / ball_speed;
                float sync_offset = 0.2f;

                float spawn_time = fmaxf(0.0f, beatmap_music[next_note] - lead_time + sync_offset);

                if(GetMusicTimePlayed(playlist[selectedSong].musica) >= spawn_time){
                    
                    if(countActiveNotes(head) < 2 && (ultimo_spawn < 0.0f || (beatmap_music[next_note] - ultimo_spawn) >= 0.3f)){
                        createnextball(&head, &tail, 0, noteTextures[next_note % 3], dir);
                        ultimo_spawn = beatmap_music[next_note]; 
                        if(n == NULL) n = head;
                        if(aux == NULL) aux = head;
                    }
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

            
            playertablet.height = 60;
            playertablet.width = 60;

            float raio = 80.0f;

            if(up){
                playertablet.x = pposx - 30;
                playertablet.y = pposy - raio - 30;
            }else if(down){
                playertablet.x = pposx - 30;
                playertablet.y = pposy + raio - 30;
            }else if(right){
                playertablet.x = pposx + raio - 30;
                playertablet.y = pposy - 30;
            }else{
                playertablet.x = pposx - raio - 30;
                playertablet.y = pposy - 30;
            }


            if(IsKeyPressed(KEY_SPACE) && aux != NULL && aux->next != NULL) aux = aux->next;

            if(IsKeyPressed(KEY_BACKSPACE) && aux != NULL && aux->prev != NULL) aux = aux->prev;

            if(IsMusicStreamPlaying(playlist[selectedSong].musica)) {
                if(IsKeyPressed(KEY_ONE)) PauseMusicStream(playlist[selectedSong].musica);
            } else {
                if(IsKeyPressed(KEY_ONE)) ResumeMusicStream(playlist[selectedSong].musica);
            }


			if(n != NULL && CheckCollisionRecs(n->rect, playerrect) && n->check != 0){
                
                if(n->next != NULL) n = n->next;
            }

            balls* tempHit = head;

            while(tempHit != NULL){

                if((up || down || right || left) &&
                CheckCollisionRecs(tempHit->rect, playertablet) &&
                tempHit->check == 0){

                    PlaySound(somAcerto);

                    tempHit->check = 2;

                    pontuacaoAtual += 100;

                    notasAcertadas++;

                    notasPassadas++;

                    comboAtual++;

                    if(comboAtual > maxCombo)
                        maxCombo = comboAtual;

                    ball_speed = ball_speed_base;

                    float tempoAtual = GetMusicTimePlayed(playlist[selectedSong].musica);
                    float tempoIdeal = beatmap_music[next_note];
                    float diff = tempoAtual - tempoIdeal;
                    TraceLog(LOG_INFO, "Sync: diff=%.3fs (+ atrasado, - adiantado)", diff);

                    TraceLog(LOG_INFO, "COMBO: %d", comboAtual);
                }

                tempHit = tempHit->next;
            }

            balls* tempMiss = head;
            while(tempMiss != NULL){
                if(tempMiss->check == 0){

                    float dx = tempMiss->vect.x - (GetScreenWidth() / 2.0f);
                    float dy = tempMiss->vect.y - (GetScreenHeight() / 2.0f);
                    float dist_ao_centro = sqrtf(dx*dx + dy*dy);


                    if(dist_ao_centro <= 60.0f){
                        comboAtual = 0;
                        PlaySound(somErro);
                        tempMiss->check = 1; 
                        notasPassadas++; 
                        ball_speed = ball_speed_base;
                    }
                }
                tempMiss = tempMiss->next;
            }		

            if(n != NULL) {

                bool saiu = (n->vect.x < -100 || n->vect.x > GetScreenWidth() + 100 || n->vect.y < -100 || n->vect.y > GetScreenHeight() + 100);

                if(saiu) {

                    if (n->check == 0) { 
                        comboAtual = 0;
                        notasPassadas++; 
                        TraceLog(LOG_INFO, "RESET por saiu");
                        ball_speed = ball_speed_base;
                    }

                    balls* remover = n;

                    if(n->next != NULL)
                        n = n->next;
                    else
                        n = NULL;

                    removeball(&head, &tail, remover);
                }
            }

            if(n != NULL && n->check != 0 && n->next != NULL) {
                n = n->next;
            }

            float musicDuration = GetMusicTimeLength(playlist[selectedSong].musica);
            float musicPlayed  = GetMusicTimePlayed(playlist[selectedSong].musica);

            if (musicPlayed >= musicDuration - 0.1f) {
                StopMusicStream(playlist[selectedSong].musica);
                currentScreen = SCREEN_SCORE;
            } else {
                UpdateMusicStream(playlist[selectedSong].musica);
            }

            TraceLog(LOG_INFO, "FIM: acertadas=%d, passadas=%d, accuracy=%.1f%%, maxCombo=%d",
            notasAcertadas,
            notasPassadas,
            notasPassadas > 0 ? ((float)notasAcertadas / notasPassadas) * 100.0f : 0.0f,
            maxCombo);
            
        } 

        else if (currentScreen == SCREEN_PAUSED) {


            if (IsKeyPressed(KEY_DOWN)){
                selectedPauseOption++;

                if (selectedPauseOption > 2)
                    selectedPauseOption = 0;
            }

            if (IsKeyPressed(KEY_UP)){
                selectedPauseOption--;

                if (selectedPauseOption < 0)
                    selectedPauseOption = 2;
            }


            if (IsKeyPressed(KEY_ENTER)){
                switch(selectedPauseOption){
                    case 0:

                        ResumeMusicStream(playlist[selectedSong].musica);
                        currentScreen = SCREEN_GAMEPLAY;
                        break;

                    case 1:

                        StopMusicStream(playlist[selectedSong].musica);
                        musicStarted = false;
                        deleteeverything(&head, &tail);
                        next_note = 0;
                        currentScreen = SCREEN_SONG_SELECT;
                        break;

                    case 2:

                        currentScreen = SCREEN_MENU;
                        break;
                }
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

                int bgIndex = BG_SKIN_0 + gs.selectedSkin; 

                ClearBackground(BLACK);

                DrawTexturePro(
                    gs.backgrounds[bgIndex],
                    (Rectangle){ 0, 0, gs.backgrounds[bgIndex].width, gs.backgrounds[bgIndex].height },
                    (Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() },
                    (Vector2){ 0, 0 },
                    0.0f,
                    WHITE
                );

                float receptorRaio = 20.0f;
                float distancia = 80.0f;

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


                int dezenaAtual = (comboAtual / 10) * 10;

                if(dezenaAtual > 0 && dezenaAtual != ultimaDezenaMostrada) {
                    ultimaDezenaMostrada = dezenaAtual;
                    comboTimer = 4.0f; 
                }

                if(comboTimer > 0.0f) {
                    comboTimer -= GetFrameTime();

                    float pulse = 1.0f + 0.15f * sinf(comboTimer * 8.0f);
                    int fontSize = (int)(40 * pulse);

                    const char* texto = TextFormat("%dx COMBO!", ultimaDezenaMostrada);
                    int textW = MeasureText(texto, fontSize);

                    float t = comboTimer / 4.0f;
                    Color cor = (Color){
                        255,
                        (unsigned char)(165 * t + 80 * (1 - t)),
                        0,
                        255
                    };

                    DrawText(texto, GetScreenWidth()/2 - textW/2, 60, fontSize, cor);
                }

                balls* tempMove = head;

                while(tempMove != NULL){

                    moveballs(tempMove, ball_speed * GetFrameTime());

                    tempMove->outsiderect.height--;
                    tempMove->outsiderect.width--;

                    tempMove->outsiderect.x = tempMove->rect.x;
                    tempMove->outsiderect.y = tempMove->rect.y;

                    tempMove = tempMove->next;
                }

                balls* tempDraw = head;

                while(tempDraw != NULL){

                    if(tempDraw->check == 0){

                        float tamanho = 64.0f;

                        DrawTexturePro(
                            tempDraw->sprite,
                            (Rectangle){ 0, 0, tempDraw->sprite.width, tempDraw->sprite.height },
                            (Rectangle){ tempDraw->vect.x, tempDraw->vect.y, tamanho, tamanho },
                            (Vector2){ tamanho/2, tamanho/2 },
                            0.0f,
                            WHITE
                        );
                    }

                    tempDraw = tempDraw->next;
                }

                Texture2D* setaAtiva = NULL;
                if(up)    setaAtiva = &setaCima;
                if(down)  setaAtiva = &setaBaixo;
                if(right) setaAtiva = &setaDireita;
                if(left)  setaAtiva = &setaEsquerda;

                if(setaAtiva != NULL) {
                    DrawTexturePro(
                        *setaAtiva,
                        (Rectangle){ 0, 0, setaAtiva->width, setaAtiva->height },
                        (Rectangle){ playertablet.x, playertablet.y, playertablet.width, playertablet.height },
                        (Vector2){ 0, 0 },
                        0.0f,
                        WHITE
                    );
                }
                
            }
            
            else if (currentScreen == SCREEN_PAUSED) {

                int bgIndex = 5 + gs.selectedSkin;
                DrawTexturePro(
                    gs.backgrounds[bgIndex],
                    (Rectangle){ 0, 0, gs.backgrounds[bgIndex].width, gs.backgrounds[bgIndex].height },
                    (Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() },
                    (Vector2){ 0, 0 },
                    0.0f,
                    WHITE
                );


                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

                float scale = 0.55f;
                int radioW = (int)(pauseRadio.width  * scale);
                int radioH = (int)(pauseRadio.height * scale);
                int radioX = GetScreenWidth()/2  - radioW/2;
                int radioY = GetScreenHeight()/2 - radioH/2;

                DrawTextureEx(pauseRadio, (Vector2){radioX, radioY}, 0.0f, scale, WHITE);


                Color optionColor;
                switch(selectedPauseOption) {
                    case 0: optionColor = GREEN;  break;
                    case 1: optionColor = YELLOW; break;
                    case 2: optionColor = RED;    break;
                    default: optionColor = WHITE; break;
                }

                const char *currentText = pauseOptions[selectedPauseOption];
                int fontSize = 28;

                float telaCentroX = radioX + (0.50f * radioW);
                float telaCentroY = radioY + (0.54f * radioH);

                int textWidth = MeasureText(currentText, fontSize);
                int textX = (int)telaCentroX - textWidth/2;
                int textY = (int)telaCentroY - fontSize/2;

                for(int i = 0; i < 3; i++){
                    Color cor = (i == selectedPauseOption) ? optionColor : (Color){80, 80, 80, 255};
                    int itemY = textY - 30 + (i * 30);
                    int itemW = MeasureText(pauseOptions[i], 20);
                    DrawText(pauseOptions[i], GetScreenWidth()/2 - itemW/2, itemY, 20, cor);
                }

            }

            else if(currentScreen == SCREEN_SCORE){

                float accuracyReal = 0.0f;
                if (notasPassadas > 0) {
                    accuracyReal = ((float)notasAcertadas / notasPassadas) * 100.0f;
                }

                DrawScoreSystem(pontuacaoAtual, accuracyReal, maxCombo);

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

    for(int i = 0; i < 8; i++){
        UnloadTexture(gs.backgrounds[i]);
    }

    for(int i = 0; i < totalSongs; i++) {
        StopMusicStream(playlist[i].musica);
        UnloadMusicStream(playlist[i].musica);
    }

    UnloadTexture(setaCima);
    UnloadTexture(setaBaixo);
    UnloadTexture(setaDireita);
    UnloadTexture(setaEsquerda);

    UnloadSound(somAcerto);
    UnloadSound(somErro);

    deleteeverything(&head, &tail);

    if(beatmap_music != NULL) free(beatmap_music);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
