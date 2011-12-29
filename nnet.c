#include <stdlib.h>
#include <stdio.h>

float bigrams[256 * 256];

void bigrammer(FILE *f) {
    int pc = -1, c, n;

    for (n = 256 * 256; n > 0; --n) bigrams[n-1] = 0;

    while ((c = getc(f)) != EOF) {
        if (pc != -1) {
            ++bigrams[256 * pc + c];
            ++n;
        }
        pc = c;
    }

    if (n) {
        for (pc = 0; pc < (256 * 256); ++pc) bigrams[pc] /= n;
    }
}

int main(int argc, char **argv) {
    exit(0);
}
