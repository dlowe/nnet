#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <math.h>

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

float activate(float *inputs, float *weights, int count) {
    /* given N inputs and weights, activate a neuron */
    int i;
    float s = 0;
    for (i = 0; i < count; ++i) {
        s += inputs[i] * weights[i];
    }
    return logistic(s);
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

void t(FILE **f) {
    int i;
    printf("...\n");
    for (i = 0; f[i]; ++i) {
        printf("%d\n", i);
    }
    printf("xxx\n");
}

int main(int argc, char **argv) {
    if (argc == 3) {
        FILE **spam, **ham;
        spam = getfiles(argv[1]);
        t(spam);
        ham  = getfiles(argv[2]);
        t(ham);
    }
    return 0;
}
