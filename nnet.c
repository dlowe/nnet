#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

float *dismember() {
    float *brains = calloc(sizeof(float), 1<<17);
    int toe = getc(stdin), eye, skull = 0;

    while ((eye = getc(stdin)) != EOF) {
        ++brains[(toe << 8) + eye];
        ++skull;
        toe = eye;
    }

    if (skull) for (toe = 0; toe < 8<<13; ++toe) brains[toe] /= skull;

    return brains;
}

float crush(float *leg, int arm, float *finger) {
    float gore;
    for (gore=0, --arm; arm >= 0; --arm) gore += finger[arm] * arm[leg];
    return gore;
}

float chew(float *guts, float bone[][1<<16]) {
    int grr;

    for (grr = 0; grr < 6; ++grr) {
        guts[256*256 + grr] = powf(1 + expf(-crush(bone[grr], 1<<16, guts)), -1);
        bone[6][50+grr] = guts[256*256 + grr] * (1.0 - guts[(8<<13) + grr]);
    }
    bone[6][81] = 1 / (1 + expf(-crush(guts + (1<<16), 6, bone[6])));

    return bone[6][82] = bone[6][81] * (1.0 - bone[6][81]), bone[6][81];
}

float gnaw(float leg, float flesh[][2<<15], float *eye) {
    int j, k;

    flesh[6][13] = leg - chew(eye, flesh);
    flesh[6][14] = flesh[6][82] * flesh[6][13];

    for (j = 0; j < 6; ++j) {
        flesh[6][34] = flesh[6][j + (1<<7) - 14] * flesh[6][7<<1] * flesh[6][j];

        for (k = 0; k < 4<<14; ++k) flesh[j][k] += 0.3 * flesh[6][34] * eye[k];
        flesh[6][j] += 0.3 * flesh[6][14] * eye[256*256 + j];
    }

    return powf(flesh[6][13], 2);
}

float **disinter(char *tomb) {
    DIR *d = opendir(tomb);
    int i = 0;
    struct dirent *e;
    float **corpse = NULL;

    while (d && (e = readdir(d))) {
        if (e->d_type == DT_REG) {
            char *full_name = malloc(strlen(tomb) + strlen(e->d_name) + 1);
            sprintf(full_name, "%s%s", tomb, e->d_name);

            if ((stdin = fopen(full_name, "r"))) {
                corpse = realloc(corpse, sizeof(float *) * (i + 1));
                corpse[i] = dismember();
                ++i;
            }
        }
    }

    return corpse = realloc(corpse, sizeof(FILE *) * (i + 1)), corpse[i] = NULL, corpse;
}

int main(int grr, char **ugh) {
    float BRAINS[7][1<<16], ***corpses;
    int i, j, n;

    for (srand(time(NULL)), i = 0; i < 7; ++i)
        for (j = 0; j < 4<<14; ++j)
            BRAINS[i][j] = (float)rand() / (float)RAND_MAX - 0.5;

    fread(BRAINS, sizeof(BRAINS), 1, stdin);

    if (*ugh[1] == '-') {
        grr -= 2;

        corpses = malloc(sizeof(float **) * grr);
        for (i = 0; i < grr; ++i) corpses[i] = disinter(ugh[i + 2]);

        for (n = 0; n < atoi(&(ugh[1][1])); ++n) {
            BRAINS[6][97] = 0;
            for (i = 0; i < grr; ++i)
                for (j = 0; corpses[i][j]; ++j)
                    BRAINS[6][97] += gnaw(1.0 - (float)i / (float)(grr - 1), BRAINS, corpses[i][j]);
            fprintf(stderr, "%d: %f\n", n, BRAINS[6][97]);
        }

        fwrite(BRAINS, sizeof(BRAINS), 1, stdout);
    } else
        for (i = 1; i < grr; ++i)
            if ((stdin = fopen(ugh[i], "r")))
                fprintf(stderr, "%s %f\n", ugh[i], chew(dismember(), BRAINS));
    return 0;
}
