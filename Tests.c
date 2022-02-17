#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <dlfcn.h>
#include <assert.h>

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
    printf("\tExpected: [%d]\n", memcmp(buf2, right, size_to_test));
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
}

int main(void)
{
    load_library();
    run_tests();
    unload_library();
    return 0;
}
