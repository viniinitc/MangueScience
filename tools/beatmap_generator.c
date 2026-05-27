#include <aubio/aubio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){


    const int qtd_musicas = 5;

    const char* musicas[] = {
        "../resources/music/praieira.mp3",
        "../resources/music/maracatu_atomico.mp3", 
        "../resources/music/da_lama_ao_caos.mp3",
        "../resources/music/manguetown.mp3",
        "../resources/music/rios_pontes_e_overdrives.mp3"
    };

    const char* beatmaps[] = {
        "praieira.beatmap",
        "maracatu_atomico.beatmap",
        "da_lama_ao_caos.beatmap",
        "manguetown.beatmap",
        "rios_pontes_e_overdrives.beatmap"
    };
    

    fvec_t *buffer = new_fvec(256);
    fvec_t *velocity = new_fvec(3);

    for (int i = 0; i < qtd_musicas; i++){

        aubio_source_t *music = new_aubio_source(musicas[i], 44100, 256);
        aubio_notes_t *notes = new_aubio_notes("default", 1024, 256, 44100);

        uint_t frames_read = 0;
        uint_t total_frames = 0;

        FILE *file_music = fopen(beatmaps[i], "w"); 

        do{

            aubio_source_do(music, buffer, &frames_read);
            aubio_notes_do(notes, buffer, velocity);

            total_frames += frames_read;

            if(velocity->data[0] > 0){
                smpl_t pitch = velocity->data[0];
                smpl_t volume = velocity->data[1];

                float timestamp = total_frames / 44100.0f;

                int lane = (int)(pitch / 32.0f);

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

