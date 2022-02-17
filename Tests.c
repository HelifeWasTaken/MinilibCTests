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
size_t (*my_memcpy)(void *, const void *, size_t) = NULL;
int (*my_strcmp)(const char *s1, const char *s2) = NULL;

void unload_library(void)
{
    printf("Unloading library\n");
    dlclose(handler);
}

void load_library(void)
{
    TEST_HEADER;
    printf("Loading library ./libasm.so");
    handler = dlopen("./libasm.so", RTLD_LAZY);
    NOT_NULL(handler);
    LOAD_SYM(my_strlen, "strlen");
    LOAD_SYM(my_memcpy, "memcpy");
    LOAD_SYM(my_strcmp, "strcmp");
}

void assert_strlen(char const *test)
{
    printf("=============\n");
    printf("\tTesting:  [%s]\n", test);
    printf("\tGot:      [%lu]\n", strlen(test));
    printf("\tExpected: [%lu]\n", my_strlen(test));
    printf("=============\n\n");
}

void assert_memcpy(void *right, size_t size, size_t size_to_test)
{
    char buf1[BUFSIZ] = {0};
    char buf2[BUFSIZ] = {0};

    memcpy(buf1, right, size);
    my_memcpy(buf2, right, size);
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

/* tests */

void tests_strcmp(void)
{
    assert_strcmp("hello world", "hello world");
    assert_strcmp("hello worl", "hello world");
    assert_strcmp("hello world", "hello worl");
    assert_strcmp("hello world", "hello");
}

void tests_strlen(void)
{
    assert_strlen("hello");
    assert_strlen("he");
    assert_strlen("h");
    assert_strlen("");
}

void tests_memcpy(void)
{
    assert_memcpy("qigqoiegeqogb", 10, 10);
    assert_memcpy("qigqoiegeqogb", 1, 10);
    assert_memcpy("qigqoiegeqogb", 0, 10);
    assert_memcpy("qi", 1, 1);
}

void run_tests()
{
    RUN_TEST_SUITE(tests_strlen, "strlen");
    RUN_TEST_SUITE(tests_memcpy, "memcpy");
    RUN_TEST_SUITE(tests_strcmp, "strcmp");
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    load_library();
    run_tests();
    unload_library();
    return 0;
}
