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

float weighted_sum(float *inputs, float *weights, int c) {
    float s = 0;
    for (--c; c >= 0; --c) {
        s += inputs[c] * weights[c];
    }
    return s;
}

float evaluate(float *inputs, float weights[][1<<16]) {
    int i;

    for (i = 0; i < 6; ++i) {
        inputs[(1<<16) + i] = logistic(weighted_sum(inputs, weights[i], 1<<16));
        weights[6][50+i] = inputs[(1<<16) + i] * (1.0 - inputs[(1<<16) + i]);
    }
    weights[6][81] = logistic(weighted_sum(inputs + (1<<16), weights[6], 6));
    weights[6][82] = weights[6][81] * (1.0 - weights[6][81]);

    return weights[6][81];
}

float backpropagate(float *inputs, float weights[][1<<16], float n) {
    int j, k;

    weights[6][13] = n - evaluate(inputs, weights);
    weights[6][19] = weights[6][82] * weights[6][13];

    for (j = 0; j < 6; ++j) {
        weights[6][34] = weights[6][50 + j] * weights[6][19] * weights[6][j];

        for (k = 0; k < 1<<16; ++k) {
            weights[j][k] += 0.3 * weights[6][34] * inputs[k];
        }

        weights[6][j] += 0.3 * weights[6][19] * inputs[(1<<16) + j];
    }

    return powf(weights[6][13], 2);
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
    float weights[7][1<<16], ***training;
    int i, j, n;

    srand(time(NULL));
    for (i = 0; i < 7; ++i) {
        for (j = 0; j < 1<<16; ++j) {
            weights[i][j] = (float)rand() / (float)RAND_MAX - 0.5;
        }
    }

    fread(weights, sizeof(weights), 1, stdin);

    if (*argv[1] == '-') {
        argc -= 2;

        training = malloc(sizeof(float **) * argc);
        for (i = 0; i < argc; ++i) {
            training[i] = getfiles(argv[i + 2]);
        }

        for (n = 0; n < atoi(&(argv[1][1])); ++n) {
            float error = 0;
            for (i = 0; i < argc; ++i) {
                for (j = 0; training[i][j]; ++j) {
                    error += backpropagate(training[i][j], weights, 1.0 - (float)i / (float)(argc - 1));
                }
            }
            fprintf(stderr, "%d: %f\n", n, error);
        }

        fwrite(weights, sizeof(weights), 1, stdout);
    } else {
        for (i = 1; i < argc; ++i) {
            if ((stdin = fopen(argv[i], "r"))) {
                fprintf(stderr, "%s %f\n", argv[i], evaluate(dismember(stdin), weights));
            }
        }
    }
    return 0;
}
