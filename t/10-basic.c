#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "../tap.c"

char outfile[] = "tmp_10-basic_XXXXXX";
char expfile[] = "expected/10-basic.t.out";

void test(void) {
    tap_plan(0);
    tap_plan(1);
    tap_plan(2);

    tap_skip_all(NULL);
    tap_skip_all("foo %s", "bar");

    tap_skip(0, NULL);
    tap_skip(1, "foo %s", "bar");
    tap_skip(2, NULL);

    tap_diag("foo");
    tap_diag("foo %s", "bar");

    tap_bail(NULL);
    tap_bail("foo %s", "bar");

    tap_ok(0, NULL);
    tap_ok(1, NULL);
    tap_ok(0, "%s", "foo");
    tap_ok(1, "%s", "foo");

    tap_is_str("foo", "foo", NULL);
    tap_is_str("foo", "bar", NULL);
    tap_is_str("foo", "foo", "foo %s", "bar");
    tap_is_str("foo", "bar", "foo %s", "bar");
    tap_is_str(NULL, NULL, NULL);
    tap_is_str(NULL, "bar", NULL);

    tap_is_int(1, 1, NULL);
    tap_is_int(1, 0, NULL);
    tap_is_int(1, 1, "foo %s", "bar");
    tap_is_int(1, 0, "foo %s", "bar");

    tap_is_float(1.0, 1.0, 0.1, NULL);
    tap_is_float(1.0, 2.0, 0.1, NULL);
    tap_is_float(1.0, 1.0, 0.1, "foo %s", "bar");
    tap_is_float(1.0, 2.0, 0.1, "foo %s", "bar");

    tap_done_testing();
}

void run(void) {
    test();
    tap_todo("FOO TODO");
    test();
}

int main(int argc, char **argv) {
    int sfd, efd, ofd, ret = 0;
    FILE *efile, *ofile;
    char ebuf[50], obuf[50];
    int line = 0;

    /* redirect output to temp file */
    ofd = mkstemp(outfile);
    sfd = dup(fileno(stdout));
    efd = dup(fileno(stderr));
    dup2(ofd, fileno(stdout));
    dup2(ofd, fileno(stderr));
    close(ofd);

    run();

    /* flush and restore output */
    fflush(stdout);
    fflush(stderr);
    dup2(sfd, fileno(stdout));
    dup2(efd, fileno(stderr));
    close(sfd);
    close(efd);

    efile = fopen(expfile, "r");
    ofile = fopen(outfile, "r");

    puts("1..1");
    while(1) {
        char *eres = fgets(ebuf, 50, efile);
        char *ores = fgets(obuf, 50, ofile);
        ++line;
        if(ores == NULL && eres == NULL) {
            puts("ok 1 - output matches expected");
            break;
        } else if(ores && eres && strcmp(ebuf, obuf) == 0) {
            /* match */
        } else {
            fprintf(stdout, "not ok 1 - mismatch beginning on line %d\n", line);
            fprintf(stderr, "    #      got: '%s'\n", obuf);
            fprintf(stderr, "    # expected: '%s'\n", ebuf);
            ret = 1;
            break;
        }
    }

    fclose(efile);
    fclose(ofile);

    if(getenv("TAP_SAVE")) {
        fprintf(stderr, "%s output saved to %s\n", __FILE__, outfile);
    } else {
        unlink(outfile);
    }

    return ret;
}
