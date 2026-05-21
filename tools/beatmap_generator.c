#include <aubio/aubio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    // lista de músicas pra processar
    const int qtd_musicas = 3;

    const char* musicas[] = {
        "../resources/praiera.mp3",
        "../resources/maracatu_atomico.mp3", 
        "../resources/da_lama_ao_caos.mp3"
    };

    const char* beatmaps[] = {
        "praiera.beatmap",
        "maracatu_atomico.beatmap",
        "da_lama_ao_caos.beatmap"
    };
    

    fvec_t *buffer = new_fvec(256);
    fvec_t *velocity = new_fvec(3);

    for (int i = 0; i < qtd_musicas; i++){

        aubio_source_t *music = new_aubio_source(musicas[i], 44100, 256); //abre o audio
        aubio_notes_t *notes = new_aubio_notes("default", 1024, 256, 44100); //detecta notas

        uint_t frames_read = 0;
        uint_t total_frames = 0;

        FILE *file_music = fopen(beatmaps[i], "w"); //arquivo p salvar o beatmap

        //processando o audio frame p frame
        do{

            aubio_source_do(music, buffer, &frames_read);
            aubio_notes_do(notes, buffer, velocity);

            total_frames += frames_read;

            if(velocity->data[0] > 0){
                smpl_t pitch = velocity->data[0]; //a altura musical da nota
                smpl_t volume = velocity->data[1]; // a intensidade da nota

                float timestamp = total_frames / 44100.0f; //diz quando a nota deve ser acertada

                int lane = (int)(pitch / 32.0f); // em qual das 4 colunas a nota vai cair

                fprintf(file_music, "%f %f %f %d\n", timestamp, pitch, volume, lane);
            }
            
        }while (frames_read == 256);

        del_aubio_source(music);
        del_aubio_notes(notes);
        fclose(file_music);

    }
    

    del_fvec(buffer);
    del_fvec(velocity);
    
    return 0;
}

