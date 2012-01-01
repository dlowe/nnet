#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

float bigrams[1<<16];

void bigrammer(FILE *f) {
    int p = getc(f), c, n;

    /* init bigrams to 0 (and init n to 0!) */
    for (n = 1<<16; n > 0; --n) bigrams[n-1] = 0;

    /* walk stream and remember bigrams */
    while ((c = getc(f)) != EOF) {
        ++bigrams[256 * p + c];
        ++n;
        p = c;
    }

    /* normalize */
    if (n) for (p = 0; p < 1<<16; ++p) bigrams[p] /= n;
}

float logistic(float x) {
    /* 1/(1+exp(-x)) */
    return powf(1 + expf(-x), -1);
}

float weighted_sum(float *inputs, float *weights, int count) {
    /* return weighted sum of inputs */
    int i;
    float s = 0;
    for (i = 0; i < count; ++i) {
        s += inputs[i] * weights[i];
    }
    return s;
}

float activate(float *inputs, float *weights, int count) {
    /* given N inputs and weights, activate a neuron */
    return logistic(weighted_sum(inputs, weights, count));
}

float dx_activate(float *inputs, float *weights, int count) {
    /* given N inputs and weights, compute the derivative of the activation function */
    float i = weighted_sum(inputs, weights, count);
    return logistic(i) * logistic(-i);
}

#define N_HIDDEN 6
float evaluate(FILE *f, float weights[][1<<16]) {
    int i;

    fseek(f, 0, SEEK_SET);
    bigrammer(f);
    for (i = 0; i < N_HIDDEN; ++i) {
        bigrams[(1<<16) + i] = activate(bigrams, weights[i], 1<<16);
        /* printf("hidden node %d: %f\n", i, bigrams[(1<<16) + i]); */
    }
    return activate(bigrams + (1<<16), weights[N_HIDDEN], N_HIDDEN);
}

FILE **getfiles(char *dirname) {
    DIR *d = opendir(dirname);
    FILE **files = NULL;
    int i = 0;
    struct dirent *de;

    while (d && (de = readdir(d))) {
        if (de->d_type == DT_REG) {
            char full_name[512];
            snprintf(full_name, 512, "%s%s", dirname, de->d_name);

            files = realloc(files, sizeof(FILE *) * (i + 1));
            files[i] = fopen(full_name, "r");
            if (files[i]) {
                ++i;
            }
        }
    }

    /* null-terminate */
    files = realloc(files, sizeof(FILE *) * (i + 1));
    files[i] = NULL;

    return files;
}

float weights[N_HIDDEN+1][1<<16];
int main(int argc, char **argv) {
    if (argc == 3) {
        FILE **spam, **ham;
        int i;

        spam = getfiles(argv[1]);
        ham  = getfiles(argv[2]);

        /* randomize weights */
        srand(time(NULL));
        for (i = 0; i < N_HIDDEN+1; ++i) {
            int j;
            for (j = 0; j < 1<<16; ++j) {
                weights[i][j] = (float)rand() / (float)RAND_MAX - 0.5;
            }
        }

        /* evaluate spam */
        for (i = 0; spam[i]; ++i) {
            printf("%f\n", evaluate(spam[i], weights));
        }
    }
    return 0;
}
