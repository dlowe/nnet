#include <check.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <float.h>

extern float bigrams[256 * 256];
extern void bigrammer(FILE *f);
extern float logistic(float x);

#define IDX(s) ((s[0]) * 256 + (s[1]))

void bigram_string(char *s, int len, int n, ...) {
    char template[256] = "/tmp/tfile.XXXXXX";
    char *filename = mktemp(template);
    FILE *f = fopen(filename, "w+");
    int i;
    float expected_bigrams[256 * 256];
    va_list ap;

    write(fileno(f), s, len);
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
        unsigned char *bigram = va_arg(ap, unsigned char *);
        double x = va_arg(ap, double);

        expected_bigrams[IDX(bigram)] = x;
    }
    va_end(ap);

    /* assert expectations */
    for (i = 0; i < (256 * 256); ++i) {
        fail_unless((bigrams[i] == expected_bigrams[i]),
            "bigramming '%s': bigram '%c%c' (%d.%d) expected %f, actual %f",
            s, i / 256, i % 256, i / 256, i % 256, expected_bigrams[i], bigrams[i]);
    }
    return;
}

START_TEST (test_bigrammer)
{
    /* boundary: need at least 2 bytes to get a bigram :) */
    bigram_string("", 0, 0);
    bigram_string("a", 1, 0);

    /* basic maths */
    bigram_string("AA",   2, 1, "AA", 1.0F);
    bigram_string("asdf", 4, 3, "as", 1.0F / 3.0F, "sd", 1.0F / 3.0F, "df", 1.0F / 3.0F);
    bigram_string("aaab", 4, 2, "aa", 2.0F / 3.0F, "ab", 1.0F / 3.0F);
    bigram_string("aaaa", 4, 1, "aa", 1.0F);
    bigram_string("aabb", 4, 3, "aa", 1.0F / 3.0F, "ab", 1.0F / 3.0F, "bb", 1.0F / 3.0F);

    /* byte boundaries... */
    bigram_string("\0\0", 2, 1, "\0\0", 1.0F);
    bigram_string("\xFF\xFF", 2, 1, "\xFF\xFF", 1.0F);
}
END_TEST

START_TEST (test_logistic)
{
    float y;
    y = logistic(0.0);
    fail_unless(y == 0.5);

    y = logistic(-1 * FLT_MAX);
    fail_unless(y == 0.0);

    y = logistic(FLT_MAX);
    fail_unless(y == 1.0);
}
END_TEST

int main(void) {
    TCase *tc;
    Suite *suite;
    SRunner *sr;
    int n_failed;
    
    tc = tcase_create("nnet");
    tcase_add_test(tc, test_bigrammer);
    tcase_add_test(tc, test_logistic);

    suite = suite_create("nnet");
    suite_add_tcase(suite, tc);

    sr = srunner_create(suite);

    srunner_run_all(sr, CK_NORMAL);
    n_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return n_failed ? 1 : 0;
}
