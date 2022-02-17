#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <dlfcn.h>
#include <assert.h>
#include <unistd.h>

#define NOT_NULL(v) (assert((v) != NULL))
#define LOAD_SYM(sym, symname) \
    { \
        printf("Loading library symbol: [%s]\n", symname); \
        NOT_NULL(sym = dlsym(handler, symname)); \
    }

#define RUN_TEST_SUITE(f, suite_name) \
    { \
        printf("-------------- " \
               "Running test suite for: [%s]\n\n", suite_name); \
        f(); \
        printf("--------------\n"); \
    }

#define TEST_HEADER \
    { \
        printf("\n~~~~~~~~\n"); \
        printf("\tTEST ASM B-400\n"); \
        printf("~~~~~~~~\n"); \
    } \

void *handler = NULL;
size_t (*my_strlen)(char const *) = NULL;
char *(*my_strchr)(char const *s, int c) = NULL;
char *(*my_strrchr)(char const *s, int c) = NULL;
void *(*my_memset)(void *s, int c, size_t n) = NULL;
size_t (*my_memcpy)(void *, const void *, size_t) = NULL;
int (*my_strcmp)(const char *s1, const char *s2) = NULL;
void *(*my_memmove)(void *dest, const void *src, size_t n) = NULL;
int (*my_strncmp)(const char *s1, const char *s2, size_t n) = NULL;
int (*my_strcasecmp)(const char *s1, const char *s2) = NULL;
char *(my_strstr)(const char *s1, const char *s2) = NULL;

void unload_library(void)
{
    printf("Unloading library\n");
    dlclose(handler);
}

void load_library(void)
{
    TEST_HEADER;
    printf("Loading library ./libasm.so");
    NOT_NULL(handler = dlopen("./libasm.so", RTLD_LAZY));
    LOAD_SYM(my_strlen, "strlen");
    LOAD_SYM(my_strchr, "strchr");
    LOAD_SYM(my_strrchr, "strrchr");
    LOAD_SYM(my_memcpy, "memcpy");
    LOAD_SYM(my_memset, "memset");
    LOAD_SYM(my_strcmp, "strcmp");
    LOAD_SYM(my_memove, "memmove");
    LOAD_SYM(my_strncmp, "strncmp");
    LOAD_SYM(my_strcasecmp, "strcasecmp");
    LOAD_SYM(my_strstr, "strstr");
}

void assert_strlen(char const *test)
{
    printf("=============\n");
    printf("\tTesting:  [(%s)]\n", test);
    printf("\tGot:      [%lu]\n", strlen(test));
    printf("\tExpected: [%lu]\n", my_strlen(test));
    printf("=============\n\n");
}

void assert_strchr(const char *s, int c)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%c)]\n", s, c);
    printf("\tGot:      [%s]\n", my_strchr(s, c));
    printf("\tExpected: [%s]\n", strchr(s, c));
    printf("=============\n\n");
}

void assert_strrchr(const char *s, int c)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%c)]\n", s, c);
    printf("\tGot:      [%s]\n", my_strrchr(s, c));
    printf("\tExpected: [%s]\n", strrchr(s, c));
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
    char visible[] = {'a', 'b', 'c', 'd', 'e'}
    int c = visible[sizeof(visible) - 1];
    my_memset(buf1, c, size);
    memset(buf2, c, size);
    printf("=============\n");
    printf("\tTesting:  [(%p), (%lu), (%lu)]\n", right, size, size_to_test);
    printf("\tGot:      [%d]\n", memcmp(buf1, right, size_to_test));
    printf("\t   ->:    ["); write(1, buf1, size_to_test); printf("]\n");
    printf("\tExpected: [%d]\n", memcmp(buf2, right, size_to_test));
    printf("\t   ->:    ["); write(1, buf2, size_to_test); printf("]\n");
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
    printf("\tGot:      [%d]\n", memcmp(buf1, right, size_to_test));
    printf("\t   ->:    ["); write(1, buf1, size_to_test); printf("]\n");
    printf("\tExpected: [%d]\n", memcmp(buf2, right, size_to_test));
    printf("\t   ->:    ["); write(1, buf2, size_to_test); printf("]\n");
    printf("=============\n\n");
}

void assert_strcmp(const char *s1, const char *s2)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s)]\n", s1, s2);
    printf("\tGot:      [%d]\n", my_strcmp(s1, s2));
    printf("\tExpected: [%d]\n", strcmp(s1, s2));
    printf("=============\n\n");
}

void assert_memmove(size_t size, size_t offset1, size_t offset2)
{
    char buf1[BUFSIZ] = {0};
    char buf2[BUFSIZ] = {0};
    char buf3[BUFSIZ] = {0};
    char buf4[BUFSIZ] = {0};

    for (size_t i = 0; i < BUFSIZ; i++) {
        char visible[] = {'a', 'b', 'c', 'd', 'e'}
        int c = visible[i % sizeof(visible)];
        buf1[i] = c;
        buf2[i] = c;
        c = visible[(i + offset1) % sizeof(visible)];
        buf3[i] = c;
        buf4[i] = c;
    }
    my_memmove(buf1, buf1 + offset1, size);
    memmove(buf2, buf2 + offset2, size);

    my_memmove(buf3, buf1 + offset2, size);
    memove(buf4, buf2 + offset2, size);

    printf("=============\n");
    printf("\tTesting:  [(%p), (%lu), (%lu), (%lu)]\n", buf1, size, offset1, offset2);
    printf("\tGot:      [%d]\n", memcmp(buf1, buf2, size));
    printf("\t   ->:    ["); write(1, buf1, size); printf("]\n");
    printf("\tExpected: [%d]\n", 0);
    printf("\t Testing: [(%p), (%lu), (%lu), (%lu)]\n", buf3, size, offset1, offset2);
    printf("Got:     [%d]\n", memcmp(buf3, buf4, size));
    printf("   ->:    ["); write(1, buf3, size); printf("]\n");
    printf("Expected: [%d]\n", 0);
    printf("=============\n\n");
}

void assert_strncmp(const char *s1, const char *s2, size_t n)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s), (%lu)]\n", s1, s2, n);
    printf("\tGot:      [%d]\n", my_strncmp(s1, s2, n));
    printf("\tExpected: [%d]\n", strncmp(s1, s2, n));
    printf("=============\n\n");
}

void assert_strcasecmp(const char *s1, const char *s2)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s)]\n", s1, s2);
    printf("\tGot:      [%d]\n", my_strcasecmp(s1, s2));
    printf("\tExpected: [%d]\n", strcasecmp(s1, s2));
    printf("=============\n\n");
}

void assert_strstr(const char *s1, const char *s2)
{
    printf("=============\n");
    printf("\tTesting:  [(%s), (%s)]\n", s1, s2);
    printf("\tGot:      [%s]\n", my_strstr(s1, s2));
    printf("\tExpected: [%s]\n", strstr(s1, s2));
    printf("=============\n\n");
}

/* tests */


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
    assert_strstr("hello world", "llo")
    assert_strstr("", "");
    assert_strstr("a", "");
    assert_strstr("a", "a");
    assert_strstr("", "a");
    assert_strstr("aa", "a");
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
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    load_library();
    run_tests();
    unload_library();
    return 0;
}
