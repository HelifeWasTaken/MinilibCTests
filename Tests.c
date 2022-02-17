/**
 * Tests.c
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <dlfcn.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int success = 0;
int failure = 0;

/*
Setups
*/

// Catch SIGSEGV and SIGBUS etc...
void signal_handler(int signum)
{
    printf("Got: (%d) -> a Fatal Error occured\n", signum);
    printf("If you see this message and think this is not due to your code, "
           "please report it to [mattis.dalleau@epitech.eu] "
           "or a maintainer of the repository\n");
    exit(signum);
}

#define NOT_NULL(v) (assert((v) != NULL))
#define LOAD_SYM(sym, symname) \
    { \
        printf("---- Loading library symbol: [%s]\n", symname); \
        NOT_NULL(sym = dlsym(handler, symname)); \
    }

#define RUN_TEST_SUITE(f, suite_name) \
    do { \
        printf("|------------|" \
               " Running test suite for: [%s] " \
               "|------------|\n\n", suite_name); \
        f(); \
    } while (0)

#define TEST_HEADER \
    { \
        printf("\n~~~~~~~~\n"); \
        printf("\tTEST ASM B-400\n"); \
        printf("~~~~~~~~\n\n"); \
    } \

void *handler = NULL;
size_t (*my_strlen)(char const *) = NULL;
char *(*my_strchr)(char const *, int) = NULL;
char *(*my_strrchr)(char const *, int) = NULL;
void *(*my_memset)(void *, int, size_t) = NULL;
size_t (*my_memcpy)(void *, const void *, size_t) = NULL;
int (*my_strcmp)(const char *, const char *) = NULL;
void *(*my_memmove)(void *, const void *, size_t) = NULL;
int (*my_strncmp)(const char *, const char *, size_t) = NULL;
int (*my_strcasecmp)(const char *, const char *) = NULL;
char *(*my_strstr)(const char *, const char *) = NULL;
char *(*my_strpbrk)(char const *, char const *) = NULL;
size_t (*my_strcspn)(char const *, char const *) = NULL;

void setup()
{
    TEST_HEADER;
    printf("--> Setting up tests...\n");
    printf("--> Setting up printf automatic flush...\n");
    assert(setvbuf(stdout, NULL, _IONBF, 0) == 0);
    printf("--> Setting up random seed to 42...\n");
    srand(42);
    printf("--> Setting up signal handler...\n");
    for (int i = 0; i < 32; i++)
        if (i != SIGINT)
            signal(i, signal_handler);
}

void unload_library(void)
{
    printf("\n\n--> Unloading library\n");
    dlclose(handler);
}

void load_library(void)
{
    printf("\n\n--> Loading library [./libasm.so]\n");
    NOT_NULL(handler = dlopen("./libasm.so", RTLD_LAZY));
    LOAD_SYM(my_strlen, "strlen");
    LOAD_SYM(my_strchr, "strchr");
    LOAD_SYM(my_strrchr, "strrchr");
    LOAD_SYM(my_memcpy, "memcpy");
    LOAD_SYM(my_memset, "memset");
    LOAD_SYM(my_strcmp, "strcmp");
    LOAD_SYM(my_memmove, "memmove");
    LOAD_SYM(my_strncmp, "strncmp");
    LOAD_SYM(my_strcasecmp, "strcasecmp");
    LOAD_SYM(my_strstr, "strstr");
    LOAD_SYM(my_strpbrk, "strpbrk");
    LOAD_SYM(my_strcspn, "strcspn");
    puts("");
}

void show_score()
{
    TEST_HEADER;
    printf("Success: [%d]\n", success);
    printf("Failure: [%d]\n", failure);
    printf("Rate: [%f%%]\n", ((float)success / (float)(success + failure)) * 100);
}

/*

Asserts functions to test each functions
feel free to add more functions to test
and use them in the tests section

*/

void assert_strlen(char const *test)
{
    printf("=============\n");
    printf("\tTesting:  [(%s)]\n", test);
    int res1 = my_strlen(test);
    int res2 = strlen(test);
    if (res2 != res1) {
        printf("\tGot:      [%lu]\n", res1);
        printf("\tExpected: [%lu]\n", res2);
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_strchr(const char *s, int c)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%c)]\n", s, c);
    char *res1 = my_strchr(s, c);
    char *res2 = strchr(s, c);
    if (res1 != res2) {
        printf("\tGot:      [%s]\n", res1);
        printf("\tExpected: [%s]\n", res2);
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_strrchr(const char *s, int c)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%c)]\n", s, c);
    char *res1 = my_strrchr(s, c);
    char *res2 = my_strrchr(s, c);
    if (res1 != res2) {
        printf("\tGot:      [%s]\n", my_strrchr(s, c));
        printf("\tExpected: [%s]\n", strrchr(s, c));
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_memset(size_t size_to_test)
{
    char buf1[BUFSIZ] = {0};
    char buf2[BUFSIZ] = {0};

    for (size_t i = 0; i < BUFSIZ; i++) {
        int c = rand() % 256;
        buf1[i] = c;
        buf2[i] = c;
    }
    char visible[] = {'a', 'b', 'c', 'd', 'e'};
    int c = visible[sizeof(visible) - 1];
    my_memset(buf1, c, size_to_test);
    memset(buf2, c, size_to_test);
    printf("=============\n");
    printf("\tTesting:  [(%p), (%lu)]\n", buf1, size_to_test);
    int res1 = memcmp(buf1, buf2, size_to_test);
    if (res1 != 0) {
        printf("\tGot:      [%d]\n", memcmp(buf1, buf2, BUFSIZ));
        printf("\t   ->:    ["); write(1, buf1, size_to_test); printf("]\n");
        printf("\tExpected: [%d]\n", 0);
        printf("\t   ->:    ["); write(1, buf2, size_to_test); printf("]\n");
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_memcpy(void *right, size_t size, size_t size_to_test)
{
    char buf1[BUFSIZ] = {0};
    char buf2[BUFSIZ] = {0};

    my_memcpy(buf1, right, size);
    memcpy(buf2, right, size);
    printf("=============\n");
    printf("\tTesting:  [(%p), (%lu), (%lu)]\n", right, size, size_to_test);
    int res1 = memcmp(buf1, buf2, size_to_test);
    if (res1 != 0) {
        printf("\tGot:      [%d]\n", memcmp(buf1, buf2, BUFSIZ));
        printf("\t   ->:    ["); write(1, buf1, size_to_test); printf("]\n");
        printf("\tExpected: [%d]\n", 0);
        printf("\t   ->:    ["); write(1, buf2, size_to_test); printf("]\n");
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_strcmp(const char *s1, const char *s2)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s)]\n", s1, s2);
    int res1 = my_strcmp(s1, s2);
    int res2 = strcmp(s1, s2);
    if (res2 != res1) {
        printf("\tGot:      [%d]\n", res1);
        printf("\tExpected: [%d]\n", res2);
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_memmove(size_t size, size_t offset1, size_t offset2)
{
    size_t size_array = size + offset1 + offset2;
    char *buf1 = calloc(size_array, sizeof(char));
    char *buf2 = calloc(size_array, sizeof(char));
    char *buf3 = calloc(size_array, sizeof(char));
    char *buf4 = calloc(size_array, sizeof(char));

    char *mbuf1 = calloc(size_array, sizeof(char));
    char *mbuf2 = calloc(size_array, sizeof(char));
    char *mbuf3 = calloc(size_array, sizeof(char));
    char *mbuf4 = calloc(size_array, sizeof(char));

    for (size_t i = 0; i < size_array; i++) {
        char visible[] = {'a', 'b', 'c', 'd', 'e'};
        int c = visible[i % sizeof(visible)];
        buf1[i] = c;
        buf2[i] = c;
        mbuf1[i] = c;
        mbuf2[i] = c;
        c = visible[(i + offset1) % sizeof(visible)];
        buf3[i] = c;
        buf4[i] = c;
        mbuf3[i] = c;
        mbuf4[i] = c;
    }
    memmove(buf1 + offset1, buf1, size);
    memmove(buf4, buf4 + offset2, size);
    memmove(buf3 + offset1, buf1 + offset2, size);
    memmove(buf2 + offset2, buf4 + offset1, size);
    printf("=============\n");
    printf("\tTesting:  [(%lu), (%lu), (%lu)]\n", size, offset1, offset2);
    my_memmove(mbuf1 + offset1, mbuf1, size);
    my_memmove(mbuf4, mbuf4 + offset2, size);
    my_memmove(mbuf3 + offset1, mbuf1 + offset2, size);
    my_memmove(mbuf2 + offset2, mbuf4 + offset1, size);
    int res1 = memcmp(mbuf1, buf1, size_array);
    int res2 = memcmp(mbuf2, buf2, size_array);
    int res3 = memcmp(mbuf3, buf3, size_array);
    int res4 = memcmp(mbuf4, buf4, size_array);
    if (res1 != 0 || res2 != 0 || res3 != 0 || res4 != 0) {
        printf("\tGot:      [%d, %d, %d, %d]\n", res1, res2, res3, res4);
        printf("\t   ->:    ["); write(1, mbuf1, size_array); printf("]\n");
        printf("\t   ->:    ["); write(1, mbuf2, size_array); printf("]\n");
        printf("\t   ->:    ["); write(1, mbuf3, size_array); printf("]\n");
        printf("\t   ->:    ["); write(1, mbuf4, size_array); printf("]\n");
        printf("\tExpected: [%d, %d, %d, %d]\n", 0, 0, 0, 0);
        printf("\t   ->:    ["); write(1, buf1, size_array); printf("]\n");
        printf("\t   ->:    ["); write(1, buf2, size_array); printf("]\n");
        printf("\t   ->:    ["); write(1, buf3, size_array); printf("]\n");
        printf("\t   ->:    ["); write(1, buf4, size_array); printf("]\n");
        failure++;
    } else {
        success++;
    }
    free(buf1);
    free(buf2);
    free(buf3);
    free(buf4);
}

void assert_strncmp(const char *s1, const char *s2, size_t n)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s), (%lu)]\n", s1, s2, n);
    int res1 = my_strncmp(s1, s2, n);
    int res2 = strncmp(s1, s2, n);

    if (res2 != res1) {
        printf("\tGot:      [%d]\n", res1);
        printf("\tExpected: [%d]\n", res2);
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_strcasecmp(const char *s1, const char *s2)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s)]\n", s1, s2);
    int res1 = my_strcasecmp(s1, s2);
    int res2 = strcasecmp(s1, s2);
    if (res2 != res1) {
        printf("\tGot:      [%d]\n", res1);
        printf("\tExpected: [%d]\n", res2);
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_strstr(const char *s1, const char *s2)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s)]\n", s1, s2);
    char *res1 = my_strstr(s1, s2);
    char *res2 = strstr(s1, s2);
    if (res2 != res1) {
        printf("\tGot:      [%s]\n", res1);
        printf("\tExpected: [%s]\n", res2);
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_strpbrk(const char *s1, const char *s2)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s)]\n", s1, s2);
    char *res1 = my_strpbrk(s1, s2);
    char *res2 = strpbrk(s1, s2);
    if (res2 != res1) {
        printf("\tGot:      [%s]\n", res1);
        printf("\tExpected: [%s]\n", res2);
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

void assert_strcspn(const char *s1, const char *s2)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s)]\n", s1, s2);
    size_t res1 = my_strcspn(s1, s2);
    size_t res2 = strcspn(s1, s2);
    if (res2 != res1) {
        printf("\tGot:      [%lu]\n", res1);
        printf("\tExpected: [%lu]\n", res2);
        failure++;
    } else {
        success++;
    }
    printf("=============\n\n");
}

/*

tests
Please call the asserts functions to test your functions.

*/


void tests_strlen(void)
{
    assert_strlen("hello");
    assert_strlen("he");
    assert_strlen("h");
    assert_strlen("");
}

void tests_strchr(void)
{
    assert_strchr("hello", 'h');
    assert_strchr("hello", 'o');
    assert_strchr("hello", 'l');
    assert_strchr("h", 'h');
    assert_strchr("", 'h');
    assert_strchr("hhh", 'h');
    assert_strchr("hhh", '\0');
    assert_strchr("", '\0');
}

void tests_strrchr(void)
{
    assert_strrchr("hello", 'h');
    assert_strrchr("hello", 'o');
    assert_strrchr("hello", 'l');
    assert_strrchr("h", 'h');
    assert_strrchr("", 'h');
    assert_strrchr("hhh", 'h');
    assert_strrchr("hhh", '\0');
    assert_strrchr("", '\0');
}

void tests_memset(void)
{
    assert_memset(0);
    assert_memset(1);
    assert_memset(2);
    assert_memset(3);
    assert_memset(4);
    assert_memset(5);
}

void tests_memcpy(void)
{
    assert_memcpy("qigqoiegeqogb", 10, 10);
    assert_memcpy("qigqoiegeqogb", 1, 10);
    assert_memcpy("qigqoiegeqogb", 0, 10);
    assert_memcpy("qi", 1, 1);
}


void tests_strcmp(void)
{
    assert_strcmp("hello world", "hello world");
    assert_strcmp("hello worl", "hello world");
    assert_strcmp("hello world", "hello worl");
    assert_strcmp("hello world", "hello");
    assert_strcmp("", "");
    assert_strcmp("a", "");
    assert_strcmp("a", "a");
    assert_strcmp("", "a");
    assert_strcmp("aa", "a");
    assert_strcmp("aa", "aa");
    assert_strcmp("a", "aa");
}

void tests_memmove(void)
{
    assert_memmove(10, 0, 0);
    assert_memmove(10, 0, 1);
    assert_memmove(10, 0, 2);
    assert_memmove(10, 0, 3);
    assert_memmove(10, 1, 0);
    assert_memmove(10, 1, 1);
    assert_memmove(10, 1, 2);
    assert_memmove(10, 1, 3);
    assert_memmove(10, 2, 0);
    assert_memmove(10, 2, 1);
    assert_memmove(10, 2, 2);
    assert_memmove(10, 2, 3);
    assert_memmove(10, 3, 0);
    assert_memmove(10, 3, 1);
    assert_memmove(10, 3, 2);
    assert_memmove(10, 3, 3);
}

void tests_strncmp(void)
{
    assert_strncmp("hello world", "hello world", 10);
    assert_strncmp("hello worl", "hello world", 10);
    assert_strncmp("hello world", "hello worl", 10);
    assert_strncmp("hello world", "hello", 10);

    assert_strncmp("", "", 10);
    assert_strncmp("a", "", 10);
    assert_strncmp("a", "a", 10);
    assert_strncmp("", "a", 10);
    assert_strncmp("aa", "a", 10);
    assert_strncmp("aa", "aa", 10);
    assert_strncmp("a", "aa", 10);

    assert_strncmp("hello world", "hello world", 0);
    assert_strncmp("world", "hello world", 0);

    assert_strncmp("yo", "yo", 2);
    assert_strncmp("yoaled", "yo", 2);
    assert_strncmp("yo", "yoaled", 2);
}

void tests_strcasecmp(void)
{
    // Base strcmp tests
    assert_strcasecmp("hello world", "hello world");
    assert_strcasecmp("hello worl", "hello world");
    assert_strcasecmp("hello world", "hello worl");
    assert_strcasecmp("hello world", "hello");
    assert_strcasecmp("", "");
    assert_strcasecmp("a", "");
    assert_strcasecmp("a", "a");
    assert_strcasecmp("", "a");
    assert_strcasecmp("aa", "a");
    assert_strcasecmp("aa", "aa");
    assert_strcasecmp("a", "aa");

    // Case tests
    assert_strcasecmp("Hello World", "hello world");
    assert_strcasecmp("hello world", "Hello World");
    assert_strcasecmp("Hello World", "Hello World");
    assert_strcasecmp("hello world", "hello world");
    assert_strcasecmp("Hello World", "hello");
    assert_strcasecmp("hello", "Hello World");
    assert_strcasecmp("Hello World", "hello");
    assert_strcasecmp("hello", "Hello");
    assert_strcasecmp("hello", "hello");
}

void tests_strstr(void)
{
    assert_strstr("hello world", "hello world");
    assert_strstr("hello worl", "hello world");
    assert_strstr("hello world", "hello worl");
    assert_strstr("hello world", "hello");
    assert_strstr("hello world", "world");
    assert_strstr("hello world", "lo");
    assert_strstr("hello world", "worl");
    assert_strstr("hello world", "orld");
    assert_strstr("hello world", "llo");
    assert_strstr("", "");
    assert_strstr("a", "");
    assert_strstr("a", "a");
    assert_strstr("", "a");
    assert_strstr("aa", "a");
}

void tests_strpbrk(void)
{
    /* Easy tests */
    assert_strpbrk("hello world", "helowrd");
    assert_strpbrk("hello world", "elowrd");
    assert_strpbrk("hello world", "lowrd");
    assert_strpbrk("hello world", "owrd");
    assert_strpbrk("hello world", "wrd");
    assert_strpbrk("hello world", "rd");
    assert_strpbrk("hello world", "d");
    assert_strpbrk("ello world", "helowrd");
    assert_strpbrk("llo world", "helowrd");
    assert_strpbrk("llo world", "helowrd");
    assert_strpbrk("lo world", "helowrd");
    assert_strpbrk("o world", "helowrd");
    assert_strpbrk(" world", "helowrd");
    assert_strpbrk("world", "helowrd");
    assert_strpbrk("orld", "helowrd");
    assert_strpbrk("rld", "helowrd");
    assert_strpbrk("ld", "helowrd");
    assert_strpbrk("d", "helowrd");
    assert_strpbrk("d", "h");
    assert_strpbrk("ah", "h");
    assert_strpbrk("h", "h");
    assert_strpbrk("", "h");
    assert_strpbrk("aled", "");
    assert_strpbrk("", "");
    assert_strpbrk("a", "");
    assert_strpbrk("ab", "");
    assert_strpbrk("", "a");
    assert_strpbrk("", "ab");
}

void tests_strcspn(void)
{
    /* Easy tests */
    assert_strcspn("hello world", "helowrd");
    assert_strcspn("hello world", "elowrd");
    assert_strcspn("hello world", "lowrd");
    assert_strcspn("hello world", "owrd");
    assert_strcspn("hello world", "wrd");
    assert_strcspn("hello world", "rd");
    assert_strcspn("hello world", "d");
    assert_strcspn("ello world", "helowrd");
    assert_strcspn("llo world", "helowrd");
    assert_strcspn("llo world", "helowrd");
    assert_strcspn("lo world", "helowrd");
    assert_strcspn("o world", "helowrd");
    assert_strcspn(" world", "helowrd");
    assert_strcspn("world", "helowrd");
    assert_strcspn("orld", "helowrd");
    assert_strcspn("rld", "helowrd");
    assert_strcspn("ld", "helowrd");
    assert_strcspn("d", "helowrd");
    assert_strcspn("d", "h");
    assert_strcspn("ah", "h");
    assert_strcspn("h", "h");
    assert_strcspn("", "h");
    assert_strcspn("aled", "");
    assert_strcspn("", "");
    assert_strcspn("a", "");
    assert_strcspn("ab", "");
    assert_strcspn("", "a");
    assert_strcspn("", "ab");
}

void run_tests()
{
    RUN_TEST_SUITE(tests_strlen, "strlen");
    RUN_TEST_SUITE(tests_strchr, "strchr");
    RUN_TEST_SUITE(tests_strrchr, "strrchr");
    RUN_TEST_SUITE(tests_memset, "memset");
    RUN_TEST_SUITE(tests_memcpy, "memcpy");
    RUN_TEST_SUITE(tests_strcmp, "strcmp");
    RUN_TEST_SUITE(tests_memmove, "memmove");
    RUN_TEST_SUITE(tests_strncmp, "strncmp");
    RUN_TEST_SUITE(tests_strcasecmp, "strcasecmp");
    RUN_TEST_SUITE(tests_strstr, "strstr");
    RUN_TEST_SUITE(tests_strpbrk, "strpbrk");
    RUN_TEST_SUITE(tests_strcspn, "strcspn");
}

void chose_specific_test(char *funcname)
{
    if (strcmp(funcname, "strlen") == 0)
        RUN_TEST_SUITE(tests_strlen, "strlen");
    else if (strcmp(funcname, "strchr") == 0)
        RUN_TEST_SUITE(tests_strchr, "strchr");
    else if (strcmp(funcname, "strrchr") == 0)
        RUN_TEST_SUITE(tests_strrchr, "strrchr");
    else if (strcmp(funcname, "memset") == 0)
        RUN_TEST_SUITE(tests_memset, "memset");
    else if (strcmp(funcname, "memcpy") == 0)
        RUN_TEST_SUITE(tests_memcpy, "memcpy");
    else if (strcmp(funcname, "strcmp") == 0)
        RUN_TEST_SUITE(tests_strcmp, "strcmp");
    else if (strcmp(funcname, "memmove") == 0)
        RUN_TEST_SUITE(tests_memmove, "memmove");
    else if (strcmp(funcname, "strncmp") == 0)
        RUN_TEST_SUITE(tests_strncmp, "strncmp");
    else if (strcmp(funcname, "strcasecmp") == 0)
        RUN_TEST_SUITE(tests_strcasecmp, "strcasecmp");
    else if (strcmp(funcname, "strstr") == 0)
        RUN_TEST_SUITE(tests_strstr, "strstr");
    else if (strcmp(funcname, "strpbrk") == 0)
        RUN_TEST_SUITE(tests_strpbrk, "strpbrk");
    else if (strcmp(funcname, "strcspn") == 0)
        RUN_TEST_SUITE(tests_strcspn, "strcspn");
    else
        printf("No such test: [%s]!\n", funcname);
}

int main(int ac, char **av)
{
    setup();
    load_library();
    if (ac == 1)
        run_tests();
    else
        for (unsigned int i = 1; i < ac; i++)
            chose_specific_test(av[i]);
    unload_library();
    show_score();
    return 0;
}
