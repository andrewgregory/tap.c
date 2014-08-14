#include <stdio.h>

#include "../tap.c"

/* just verify the library compiles */

int main() {
    tap_plan(1);
    tap_ok(1, "compilation successful");
    return 0;
}
