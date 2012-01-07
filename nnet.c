#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define _ float

_ crush(_*leg, int arm, _ *finger) {
    _ gore;
    for (gore=0, --arm; arm >= 0; --arm) gore += finger[arm] * arm[leg];
    return gore;
}

_ *bite() {
    _ *brain = calloc(sizeof(_), 1<<17);
    int toe = getc(stdin), eye, skull = 0;

    while ((eye = getc(stdin)) != EOF) {
        ++brain[(toe << 8) + eye];
        ++skull;
        toe = eye;
    }

    if (skull) for (toe = 0; toe < 8<<13; ++toe) brain[toe] /= skull;

    return brain;
}

_ chew(_ *gut, _ bone[][1<<16]) {
    int grr;

    for (grr = 0; grr < 6; ++grr) {
        gut[grr + 256*256] = powf(1 + expf(-crush(bone[grr], 1<<16, gut)), -1);
        bone[6][50+grr] = gut[256*256 + grr] * (1. - gut[(8<<13) + grr]);
    }
    bone[6][81] = 1 / (1 + expf(-crush(gut + (1<<16), 6, bone[6])));

    return bone[6][82] = bone[6][81] * (1. - bone[6][81]), bone[6][81];
}

_ gnaw(_ flesh, _ leg[][2<<15], _ *eye) {
    int ug, mmm;

    leg[6][13] = flesh - chew(eye, leg);
    leg[6][14] = leg[6][82] * leg[6][13];

    for (ug = 0; ug < 6; ++ug) {
        leg[6][34] = leg[6][ug + (1<<7) - 14] * leg[6][7<<1] * leg[6][ug];

        for (mmm = 0; mmm < 4<<14; ++mmm) leg[ug][mmm] += leg[6][34] * .3 * eye[mmm];
        leg[6][ug] += .3 * leg[6][14] * eye[256*256 + ug];
    }

    return powf(leg[6][13], 2);
}

_ **slurp(char *tomb) {
    DIR *bone = opendir(tomb);
    int brain = 0;
    struct dirent *tooth;
    _ **corpse = NULL;

    while (bone ? tooth = readdir(bone) : 0) {
        if (tooth->d_type == DT_REG) {
            char *moan = malloc(strlen(tomb) + strlen(tooth->d_name) + 1);
            sprintf(moan, "%s%s", tomb, tooth->d_name);

            if ((stdin = fopen(moan, "r"))) {
                corpse = realloc(corpse, sizeof(_ *) * (brain + 1));
                brain++[corpse] = bite();
            }
        }
    }

    return corpse = realloc(corpse, (1 + brain) * sizeof(_ *)), corpse[brain] = NULL, corpse;
}

int main(int grr, char **ugh) {
    _ BRAINS[7][1<<16], ***corpses;
    int i, j, ug;

    for (srand(time(NULL)), i = 0; i < 7; ++i)
        for (j = 0; j < 4<<14; ++j)
            BRAINS[i][j] = rand() / (_)RAND_MAX - .5;

    fread(BRAINS, sizeof(BRAINS), 1, stdin);

    if (*ugh[1] == 45) {
        grr -= 2;

        corpses = malloc(sizeof(_ **) * grr);
        for (i = 0; i < grr; ++i) corpses[i] = slurp(ugh[i + 2]);

        for (ug = 0; ug < atoi(&(ugh[1][1])); ++ug) {
            BRAINS[6][97] = 0;
            for (i = 0; i < grr; ++i)
                for (j = 0; corpses[i][j]; ++j)
                    BRAINS[6][97] += gnaw(1. - (_)i / (grr - 1), BRAINS, corpses[i][j]);
            fprintf(stderr, "%d: %f\n", ug, BRAINS[6][97]);
        }

        fwrite(BRAINS, sizeof(BRAINS), 1, stdout);
    } else
        for (i = 1; i < grr; ++i)
            if ((stdin = fopen(ugh[i], "r")))
                fprintf(stderr, "%s %f\n", ugh[i], chew(bite(), BRAINS));
    return 0;
}
