#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

float *dismember(FILE *body) {
    float *brains = calloc(sizeof(float), 1<<17);
    int toe = getc(body), eye, skull = 0;

    while ((eye = getc(body)) != EOF) {
        ++brains[(toe << 8) + eye];
        ++skull;
        toe = eye;
    }

    if (skull) for (toe = 0; toe < 1<<16; ++toe) brains[toe] /= skull;

    return brains;
}

float crush(float *wts, int c, float *inputs) {
    float s;
    for (s=0, --c; c >= 0; --c) s += inputs[c] * c[wts];
    return s;
}

float nibble(float *inputs, float wts[][1<<16]) {
    int i;

    for (i = 0; i < 6; ++i) {
        inputs[(1<<16) + i] = powf(1 + expf(-crush(wts[i], 1<<16, inputs)), -1);
        wts[6][50+i] = inputs[(1<<16) + i] * (1.0 - inputs[(1<<16) + i]);
    }
    wts[6][81] = 1 / (1 + expf(-crush(inputs + (1<<16), 6, wts[6])));

    return wts[6][82] = wts[6][81] * (1.0 - wts[6][81]), wts[6][81];
}

float gnaw(float n, float wts[][1<<16], float *inputs) {
    int j, k;

    wts[6][13] = n - nibble(inputs, wts);
    wts[6][19] = wts[6][82] * wts[6][13];

    for (j = 0; j < 6; ++j) {
        wts[6][34] = wts[6][50 + j] * wts[6][19] * wts[6][j];

        for (k = 0; k < 1<<16; ++k) wts[j][k] += 0.3 * wts[6][34] * inputs[k];
        wts[6][j] += 0.3 * wts[6][19] * inputs[(1<<16) + j];
    }

    return powf(wts[6][13], 2);
}

float **disinter(char *n) {
    DIR *d = opendir(n);
    int i = 0;
    struct dirent *e;
    float **corpse = NULL;

    while (d && (e = readdir(d))) {
        if (e->d_type == DT_REG) {
            char full_name[512];
            snprintf(full_name, 512, "%s%s", n, e->d_name);

            if ((stdin = fopen(full_name, "r"))) {
                corpse = realloc(corpse, sizeof(float *) * (i + 1));
                corpse[i] = dismember(stdin);
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
        for (j = 0; j < 1<<16; ++j)
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
                fprintf(stderr, "%s %f\n", ugh[i], nibble(dismember(stdin), BRAINS));
    return 0;
}
