#include <check.h>

extern int thingy(void);

START_TEST (test)
{
    fail_unless(thingy());
}
END_TEST

int main(void) {
    TCase *tc;
    Suite *suite;
    SRunner *sr;
    int n_failed;
    
    tc = tcase_create("nnet");
    tcase_add_test(tc, test);

    suite = suite_create("nnet");
    suite_add_tcase(suite, tc);

    sr = srunner_create(suite);

    srunner_run_all(sr, CK_NORMAL);
    n_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return n_failed ? 1 : 0;
}
