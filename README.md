# MinilibC - Tests

Tests for the Epitech Project MinilibC - B-ASM-400

## Compiling:

gcc Tests.c -ldl -o tests

## Usage:

```bash
./tests
```

Run all tests in the project

```bash
./tests strcmp strncmp
```
Run all the tests specifically for strncmp and strcmp

## Example Output
```
~~~~~~~~
        TEST ASM B-400
~~~~~~~~

--> Setting up tests...
--> Setting up printf automatic flush...
--> Setting up random seed to 42...
--> Setting up signal handler...


--> Loading library [./libasm.so]
---- Loading library symbol: [strlen]
---- Loading library symbol: [strchr]
---- Loading library symbol: [strrchr]
---- Loading library symbol: [memcpy]
---- Loading library symbol: [memset]
---- Loading library symbol: [strcmp]
---- Loading library symbol: [memmove]
---- Loading library symbol: [strncmp]
---- Loading library symbol: [strcasecmp]
---- Loading library symbol: [strstr]
---- Loading library symbol: [strpbrk]
---- Loading library symbol: [strcspn]

------------| Running test suite for: [strlen] |------------|

=============
        Testing:  [(hello)]
=============

=============
        Testing:  [(he)]
=============

=============
        Testing:  [(h)]
=============

=============
        Testing:  [()]
=============

|------------| Running test suite for: [memmove] |------------|

=============
        Testing:  [(0x2362c80), (10), (0), (0)]
        Got:      [0]
           ->:    [abcdeabcde]
        Expected: [0]
           ->:    [abcdeabcde]
=============
=============
         Testing: [(0x2362cc0), (10), (0), (0)]
=============

=============
        Testing:  [(0x2362d00), (10), (0), (1)]
        Got:      [-1]
           ->:    [abcdeabcdea]
        Expected: [0]
           ->:    [bcdeabcdeaa]
=============
=============
         Testing: [(0x2362d40), (10), (0), (1)]
        Got:      [-2]
           ->:    [abcdeabcdea]
        Expected: [0]
           ->:    [cdeabcdeaaa]
=============
...
~~~~~~~~
        TEST ASM B-400
~~~~~~~~

Success: [84]
Failure: [39]
Rate: [68.292686%]
```

If the test fails it shows information about `Got` and `Expected` fields

It also shows you your result at the end

## Writing tests

Notes: tests for strpbrk and strcspn are still to easy if someone wants to write some

If the function is not added in the `load_library` create a global variable containing the function pointer inside of it and you the macro `LOAD_SYM()` to load the function inside it

Then write the assert function for this call

    -> Please make it pretty
    -> Do not forget to increment the success variable or failure variable

Feel free to write multiple asserts version of a function.
Try to follow the subject order to make the code "as clean as possible".

Write a function to put all the tests inside of it called `funcname_tests` where of course `funcname` is the name of the function.

Add the `RUN_TEST_SUITE()` macro to the call your function with a pretty output inside `run_tests` function.

Add your  function inside the `chose_specific_test` function table.

## Author

- Mattis DALLEAU
