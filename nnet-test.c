#include <check.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <float.h>

extern float *dismember(FILE *f);
extern float weighted_sum(float *inputs, float *weights, int count);
extern float evaluate(float *inputs, float weights[][1<<16]);
extern float **disinter(char *dirname);

#define IDX(s) ((s[0]) * 256 + (s[1]))

void bigram_string(char *s, int len, int n, ...) {
    char template[256] = "/tmp/tfile.XXXXXX";
    char *filename = mktemp(template);
    FILE *f = fopen(filename, "w+");
    int i;
    float *bigrams;
    float expected_bigrams[256 * 256];
    va_list ap;

    write(fileno(f), s, len);
    fflush(f);
    fseek(f, 0, SEEK_SET);

    bigrams = dismember(f);

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

START_TEST (test_weighted_sum)
{
    float inputs[2] = { 0.1, 0.1 };
    float weights[2] = { -1.0, 1.0 };

    float result = weighted_sum(inputs, weights, 2);
    fail_unless(result == 0.0);
}
END_TEST

START_TEST (test_evaluate)
{
    char template[256] = "/tmp/tfile.XXXXXX";
    char *filename = mktemp(template);
    FILE *f = fopen(filename, "w+");
    float *bigrams;
    float x;
    float weights[7][1<<16];
    int i, j;

    write(fileno(f), "aa", 2);
    fflush(f);

    /* set all weights to zero */
    for (i = 0; i < 7; ++i) {
        for (j = 0; j < 1<<16; ++j) {
            weights[i][j] = 0;
        }
    }
    bigrams = dismember(f);

    /* evaluates to 0.5 */
    x = evaluate(bigrams, weights);
    fail_unless(x == 0.5);
    /* stashes the derivative in weights[6][82] ! */
    fail_unless(weights[6][82] == 0.25);

    /* force it to zero */
    weights[0][IDX("aa")] = -7;
    weights[6][0] = -100000;
    x = evaluate(bigrams, weights);
    fail_unless(x == 0.0);
    fail_unless(weights[6][82] == 0.0);
    weights[0][IDX("aa")] = 0;
    weights[6][0] = 0;

    /* force it to one */
    weights[1][IDX("aa")] = 7;
    weights[6][1] = 100000;
    x = evaluate(bigrams, weights);
    fail_unless(x == 1.0);
    fail_unless(weights[6][82] == 0.0);
    weights[1][IDX("aa")] = 0;
    weights[6][1] = 0;

    fclose(f);
    unlink(filename);
}
END_TEST

START_TEST (test_getfiles)
{
    float **result;
    char template[256] = "/tmp/tdir.XXXXXX";
    char *dirname, dirname_slash[1024];
    int i, j;

    /* return an empty list for nonexistent dirs */
    result = disinter("some-nonexistent-directory");
    fail_if(result == NULL);
    fail_unless(result[0] == NULL);

    /* return an empty list for empty dirs */
    dirname = mkdtemp(template);
    snprintf(dirname_slash, sizeof(dirname_slash), "%s/", dirname);

    result = disinter(dirname);
    fail_if(result == NULL);
    fail_unless(result[0] == NULL);

    result = disinter(dirname_slash);
    fail_if(result == NULL);
    fail_unless(result[0] == NULL);

    /* return longer lists for dirs with more files... */
    for (i = 0; i < 9; ++i) {
        FILE *f;
        char filename[1024];
        snprintf(filename, sizeof(filename), "%s%d", dirname_slash, i);
        f = fopen(filename, "w+");
        fclose(f);

        result = disinter(dirname_slash);
        fail_if(result == NULL);
        for (j = 0; j <= i; ++j) {
            fail_unless(result[j] != NULL);
        }
        fail_unless(result[j] == NULL);

        /* ... but doesn't work without the trailing slash specified */
        result = disinter(dirname);
        fail_if(result == NULL);
        fail_unless(result[0] == NULL);
    }

    for (i = 0; i < 9; ++i) {
        char filename[1024];
        snprintf(filename, sizeof(filename), "%s%d", dirname_slash, i);
        unlink(filename);
    }
    rmdir(dirname);
}
END_TEST

int main(void) {
    TCase *tc;
    Suite *suite;
    SRunner *sr;
    int n_failed;
    
    tc = tcase_create("nnet");
    tcase_add_test(tc, test_bigrammer);
    tcase_add_test(tc, test_weighted_sum);
    tcase_add_test(tc, test_evaluate);
    tcase_add_test(tc, test_getfiles);

    suite = suite_create("nnet");
    suite_add_tcase(suite, tc);

    sr = srunner_create(suite);

    srunner_run_all(sr, CK_NORMAL);
    n_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return n_failed ? 1 : 0;
}
