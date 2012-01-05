#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

float *dismember(FILE *f) {
    float *bigrams = malloc(sizeof(float) * 1<<17);
    int p = getc(f), c, n;

    for (n = 1<<16; n > 0; --n) bigrams[n-1] = 0;

    while ((c = getc(f)) != EOF) {
        ++bigrams[256 * p + c];
        ++n;
        p = c;
    }

    if (n) for (p = 0; p < 1<<16; ++p) bigrams[p] /= n;

    return bigrams;
}

float logistic(float x) {
    return powf(1 + expf(-x), -1);
}

float weighted_sum(float *inputs, float *wts, int c) {
    float s = 0;
    for (--c; c >= 0; --c) {
        s += inputs[c] * wts[c];
    }
    return s;
}

float evaluate(float *inputs, float wts[][1<<16]) {
    int i;

    for (i = 0; i < 6; ++i) {
        inputs[(1<<16) + i] = logistic(weighted_sum(inputs, wts[i], 1<<16));
        wts[6][50+i] = inputs[(1<<16) + i] * (1.0 - inputs[(1<<16) + i]);
    }
    wts[6][81] = logistic(weighted_sum(inputs + (1<<16), wts[6], 6));
    wts[6][82] = wts[6][81] * (1.0 - wts[6][81]);

    return wts[6][81];
}

float gnaw(float *inputs, float wts[][1<<16], float n) {
    int j, k;

    wts[6][13] = n - evaluate(inputs, wts);
    wts[6][19] = wts[6][82] * wts[6][13];

    for (j = 0; j < 6; ++j) {
        wts[6][34] = wts[6][50 + j] * wts[6][19] * wts[6][j];

        for (k = 0; k < 1<<16; ++k) {
            wts[j][k] += 0.3 * wts[6][34] * inputs[k];
        }

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

    corpse = realloc(corpse, sizeof(FILE *) * (i + 1));
    corpse[i] = NULL;

    return corpse;
}

int main(int grr, char **ugh) {
    float BRAINS[7][1<<16], ***corpses;
    int i, j, n;

    srand(time(NULL));
    for (i = 0; i < 7; ++i) {
        for (j = 0; j < 1<<16; ++j) {
            BRAINS[i][j] = (float)rand() / (float)RAND_MAX - 0.5;
        }
    }

    fread(BRAINS, sizeof(BRAINS), 1, stdin);

    if (*ugh[1] == '-') {
        grr -= 2;

        corpses = malloc(sizeof(float **) * grr);
        for (i = 0; i < grr; ++i) {
            corpses[i] = disinter(ugh[i + 2]);
        }

        for (n = 0; n < atoi(&(ugh[1][1])); ++n) {
            BRAINS[6][97] = 0;
            for (i = 0; i < grr; ++i) {
                for (j = 0; corpses[i][j]; ++j) {
                    BRAINS[6][97] += gnaw(corpses[i][j], BRAINS, 1.0 - (float)i / (float)(grr - 1));
                }
            }
            fprintf(stderr, "%d: %f\n", n, BRAINS[6][97]);
        }

        fwrite(BRAINS, sizeof(BRAINS), 1, stdout);
    } else {
        for (i = 1; i < grr; ++i) {
            if ((stdin = fopen(ugh[i], "r"))) {
                fprintf(stderr, "%s %f\n", ugh[i], evaluate(dismember(stdin), BRAINS));
            }
        }
    }
    return 0;
}
