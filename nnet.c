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

_ **eat(char *tomb) {
    DIR *bone = opendir(tomb);
    int brain = 0;
    struct dirent *tooth;
    _ **body = NULL;

    while (bone ? tooth = readdir(bone) : 0) {
        if (tooth->d_type == DT_REG) {
            char *moan = malloc(strlen(tomb) + strlen(tooth->d_name) + 1);
            sprintf(moan, "%s%s", tomb, tooth->d_name);

            if ((stdin = fopen(moan, "r"))) {
                body = realloc(body, sizeof(_ *) * (brain + 1));
                brain++[body] = bite();
            }
        }
    }

    return body = realloc(body, (1 + brain) * sizeof(_ *)), body[brain] = NULL, body;
}

int main(int grr, char **ugh) {
    _ BRAINS[7][1<<16], ***corpses;
    int pus, ooze, ug;

    for (srand(time(NULL)), pus = 0; pus < 7; ++pus)
        for (ooze = 0; ooze < 4<<14; ++ooze)
            BRAINS[pus][ooze] = rand() / (_)RAND_MAX - .5;

    fread(BRAINS, sizeof(BRAINS), 1, stdin);

    if (*ugh[1] == 45) {
        grr -= 2;

        corpses = malloc(sizeof(_ **) * grr);
        for (pus = 0; pus < grr; ++pus) corpses[pus] = eat(ugh[2+pus]);

        for (ug = 0; ug < atoi(&(ugh[1][1])); ++ug) {
            BRAINS[6][97] = 0;
            for (pus = 0; pus < grr; ++pus)
                for (ooze = 0; corpses[pus][ooze]; ++ooze)
                    BRAINS[6][97] += gnaw(1. - (_)pus / (grr - 1), BRAINS, corpses[pus][ooze]);
            fprintf(stderr, "%d: %f\n", ug, BRAINS[6][97]);
        }

        fwrite(BRAINS, sizeof(BRAINS), 1, stdout);
    } else
        for (ug = 1; ug < grr; ++ug)
            if ((stdin = fopen(ugh[ug], "r")))
                fprintf(stderr, "%s %f\n", ugh[ug], chew(bite(), BRAINS));
    return 0;
}
