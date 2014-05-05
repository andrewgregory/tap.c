/*
 * Copyright 2014 Andrew Gregory andrew.gregory.8@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#ifndef __TAP_C
#define __TAP_C

int _tap_tests_run = 0;
int _tap_tests_failed = 0;

const char *_tap_todo = NULL;

void tap_plan(int test_count)
{
    printf("1..%d\n", test_count);
}

void tap_skip_all(const char *reason, ...)
{
    printf("1..0");
    if(reason) {
        va_list args;
        va_start(args, reason);
        fputc(' ', stdout);
        vprintf(reason, args);
        va_end(args);
    }
    fputc('\n', stdout);
}

void tap_done_testing(void)
{
    tap_plan(_tap_tests_run);
}

/*

int tap_get_tests_run(void)
{
    return _tap_tests_run;
}

int tap_get_tests_failed(void)
{
    return _tap_tests_failed;
}

*/

void tap_todo(const char *reason)
{
    _tap_todo = reason;
}

void tap_skip(int count, const char *reason, ...)
{
    while(count--) {
        printf("ok %d # SKIP", ++_tap_tests_run);
        if(reason) {
            va_list args;
            fputs(" - ", stdout);
            va_start(args, reason);
            vprintf(reason, args);
            va_end(args);
        }
        fputc('\n', stdout);
    }
}

void tap_bail(const char *reason, ...)
{
    va_list args;
    fputs("Bail out! ", stdout);
    va_start(args, reason);
    vprintf(reason, args);
    va_end(args);
    fputc('\n', stdout);
}

void tap_diag(const char *message, ...)
{
    va_list args;
    fputs("    # ", stdout);
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
    fputc('\n', stdout);
}

void _tap_vok(int success, const char *name, va_list args)
{
    const char *result;
    if(success) {
        result = "ok";
        if(_tap_todo) ++_tap_tests_failed;
    } else {
        result = "not ok";
        if(!_tap_todo) ++_tap_tests_failed;
    }

    printf("%s %d", result, ++_tap_tests_run);

    if(name) {
        fputs(" - ", stdout);
        vprintf(name, args);
    }

    if(_tap_todo) {
        fputs(" # TODO", stdout);
        if(*_tap_todo) {
            fputc(' ', stdout);
            fputs(_tap_todo, stdout);
        }
    }

    fputc('\n', stdout);
}

#define _TAP_OK(success, name) do { \
        va_list args; \
        va_start(args, name); \
        _tap_vok(success, name, args); \
        va_end(args); \
    } while(0)

void tap_ok(int success, const char *name, ...)
{
    _TAP_OK(success, name);
}

void tap_is_float(float got, float expected, float delta, const char *name, ...)
{
    float diff = fabs(expected - got);
    int match = diff < delta;
    _TAP_OK(match, name);
    if(!match) {
        tap_diag("expected '%f'", expected);
        tap_diag("got      '%f'", got);
        tap_diag("delta    '%f'", diff);
        tap_diag("allowed  '%f'", delta);
    }
}

void tap_is_int(int got, int expected, const char *name, ...)
{
    int match = got == expected;
    _TAP_OK(match, name);
    if(!match) {
        tap_diag("expected '%d'", expected);
        tap_diag("got      '%d'", got);
    }
}

void tap_is_str(const char *got, const char *expected, const char *name, ...)
{
    int match;
    if(got && expected) {
        match = (strcmp(got, expected) == 0);
    } else {
        match = (got == expected);
    }
    _TAP_OK(match, name);
    if(!match) {
        tap_diag("expected '%s'", expected);
        tap_diag("got      '%s'", got);
    }
}

#endif /* __TAP_C */
