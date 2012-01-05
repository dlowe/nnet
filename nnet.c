#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

float *bigrammer(FILE *f) {
    float *bigrams = malloc(sizeof(float) * 1<<17);
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

    return bigrams;
}

float logistic(float x) {
    /* 1/(1+exp(-x)) */
    return powf(1 + expf(-x), -1);
}

float weighted_sum(float *inputs, float *weights, int count) {
    /* return weighted sum of inputs */
    float s = 0;
    for (--count; count >= 0; --count) {
        s += inputs[count] * weights[count];
    }
    return s;
}

float activate(float *inputs, float *weights, int count) {
    /* given N inputs and weights, activate a neuron */
    return logistic(weighted_sum(inputs, weights, count));
}

float dx_activate(float *inputs, float *weights, int count) {
    /* given N inputs and weights, compute the derivative of the activation function */
    float l = logistic(weighted_sum(inputs, weights, count));
    return l * (1.0 - l);
}

#define N_HIDDEN 6
float evaluate(float *inputs, float weights[][1<<16]) {
    int i;

    for (i = 0; i < N_HIDDEN; ++i) {
        inputs[(1<<16) + i] = activate(inputs, weights[i], 1<<16);
        /* printf("hidden node %d: %f\n", i, inputs[(1<<16) + i]); */
    }
    return activate(inputs + (1<<16), weights[N_HIDDEN], N_HIDDEN);
}

#define RATE 0.3
float backpropagate(float *inputs, float weights[][1<<16], float target) {
    float out = evaluate(inputs, weights);
    float error, output_delta;
    int j;

    error        = target - out;
    output_delta = dx_activate(inputs + (1<<16), weights[N_HIDDEN], N_HIDDEN) * error;

    for (j = 0; j < N_HIDDEN; ++j) {
        int k;
        float hidden_delta, change;

        error        = output_delta * weights[N_HIDDEN][j];
        hidden_delta = dx_activate(inputs, weights[j], 1<<16) * error;

        /* update input weights for this hidden node */
        for (k = 0; k < 1<<16; ++k) {
            change = hidden_delta * inputs[k];
            weights[j][k] += RATE * change;
        }

        /* update the output weight for this hidden node */
        change = output_delta * inputs[(1<<16) + j];
        weights[N_HIDDEN][j] += RATE * change;
    }

    return powf(target - out, 2);
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
                inputs[i] = bigrammer(stdin);
                ++i;
            }
        }
    }

    /* null-terminate */
    inputs = realloc(inputs, sizeof(FILE *) * (i + 1));
    inputs[i] = NULL;

    return inputs;
}

int main(int argc, char **argv) {
    float weights[N_HIDDEN+1][1<<16];
    int i, j;

    /* randomize weights */
    srand(time(NULL));
    for (i = 0; i < N_HIDDEN+1; ++i) {
        for (j = 0; j < 1<<16; ++j) {
            weights[i][j] = (float)rand() / (float)RAND_MAX - 0.5;
        }
    }

    fread(weights, sizeof(weights), 1, stdin);

    if (*argv[1] == '-') {
        int n;
        float ***training;

        argc -= 2;

        training = malloc(sizeof(float **) * argc);
        for (i = 0; i < argc; ++i) {
            training[i] = getfiles(argv[i + 2]);
        }

        for (n = 0; n < atoi(&(argv[1][1])); ++n) {
            float error = 0.0;
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
                fprintf(stderr, "%s %f\n", argv[i], evaluate(bigrammer(stdin), weights));
            }
        }
    }
    return 0;
}
