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

float backpropagate(float *inputs, float wts[][1<<16], float n) {
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

float **getfiles(char *dirname) {
    DIR *d = opendir(dirname);
    int i = 0;
    struct dirent *de;
    float **inputs = NULL;

    while (d && (de = readdir(d))) {
        if (de->d_type == DT_REG) {
            char full_name[512];
            snprintf(full_name, 512, "%s%s", dirname, de->d_name);

            if ((stdin = fopen(full_name, "r"))) {
                inputs = realloc(inputs, sizeof(float *) * (i + 1));
                inputs[i] = dismember(stdin);
                ++i;
            }
        }
    }

    inputs = realloc(inputs, sizeof(FILE *) * (i + 1));
    inputs[i] = NULL;

    return inputs;
}

int main(int argc, char **argv) {
    float wts[7][1<<16], ***corpses;
    int i, j, n;

    srand(time(NULL));
    for (i = 0; i < 7; ++i) {
        for (j = 0; j < 1<<16; ++j) {
            wts[i][j] = (float)rand() / (float)RAND_MAX - 0.5;
        }
    }

    fread(wts, sizeof(wts), 1, stdin);

    if (*argv[1] == '-') {
        argc -= 2;

        corpses = malloc(sizeof(float **) * argc);
        for (i = 0; i < argc; ++i) {
            corpses[i] = getfiles(argv[i + 2]);
        }

        for (n = 0; n < atoi(&(argv[1][1])); ++n) {
            wts[6][97] = 0;
            for (i = 0; i < argc; ++i) {
                for (j = 0; corpses[i][j]; ++j) {
                    wts[6][97] += backpropagate(corpses[i][j], wts, 1.0 - (float)i / (float)(argc - 1));
                }
            }
            fprintf(stderr, "%d: %f\n", n, wts[6][97]);
        }

        fwrite(wts, sizeof(wts), 1, stdout);
    } else {
        for (i = 1; i < argc; ++i) {
            if ((stdin = fopen(argv[i], "r"))) {
                fprintf(stderr, "%s %f\n", argv[i], evaluate(dismember(stdin), wts));
            }
        }
    }
    return 0;
}
