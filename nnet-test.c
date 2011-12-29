#include <check.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

extern float bigrams[256 * 256];
extern void bigrammer(FILE *f);

#define IDX(s) ((s[0]) * 256 + (s[1]))

void bigram_string(char *s, int n, ...) {
    char template[256] = "/tmp/tfile.XXXXXX";
    char *filename = mktemp(template);
    FILE *f = fopen(filename, "w+");
    int i;
    float expected_bigrams[256 * 256];
    va_list ap;

    fprintf(f, "%s", s);
    fflush(f);
    fseek(f, 0, SEEK_SET);

    bigrammer(f);

    fclose(f);
    unlink(filename);

    /* build expectations */
    for (i = 0; i < (256 * 256); ++i) {
        expected_bigrams[i] = 0;
    }
    va_start(ap, n);
    for (i = 0; i < n; ++i) {
        char *bigram = va_arg(ap, char *);
        double x = va_arg(ap, double);
        expected_bigrams[IDX(bigram)] = x;
    }
    va_end(ap);

    /* assert expectations */
    for (i = 0; i < (256 * 256); ++i) {
        fail_unless((bigrams[i] == expected_bigrams[i]),
            "bigramming '%s': bigram '%c%c' expected %f, actual %f",
            s, i / 256, i % 256, expected_bigrams[i], bigrams[i]);
    }
    return;
}

START_TEST (test_bigrammer)
{
    /* boundary: need at least 2 bytes to get a bigram :) */
    bigram_string("", 0);
    bigram_string("a", 0);

    /* basic maths */
    bigram_string("AA", 1, "AA", 1.0F);
    bigram_string("asdf", 3, "as", 1.0F / 3.0F, "sd", 1.0F / 3.0F, "df", 1.0F / 3.0F);
    bigram_string("aaab", 2, "aa", 2.0F / 3.0F, "ab", 1.0F / 3.0F);
    bigram_string("aaaa", 1, "aa", 1.0F);
    bigram_string("aabb", 3, "aa", 1.0F / 3.0F, "ab", 1.0F / 3.0F, "bb", 1.0F / 3.0F);
}
END_TEST

int main(void) {
    TCase *tc;
    Suite *suite;
    SRunner *sr;
    int n_failed;
    
    tc = tcase_create("nnet");
    tcase_add_test(tc, test_bigrammer);

    suite = suite_create("nnet");
    suite_add_tcase(suite, tc);

    sr = srunner_create(suite);

    srunner_run_all(sr, CK_NORMAL);
    n_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return n_failed ? 1 : 0;
}
