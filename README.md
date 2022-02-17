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

## Writing tests

If the function is not added in the `load_library` create a global variable containing the function pointer inside of it and you the macro `LOAD_SYM()` to load the function inside it

Then write the assert function for this call

    -> Please make it pretty
    -> Do not forget to increment the success variable or failure variable

Feel free to write multiple asserts version of a function
Try to follow the subject order to make the code "as clean as possible"

Write a function to put all the tests inside of it called `funcname_tests` where of course `funcname` is the name of the function

Add the `RUN_TEST_SUITE()` macro to the call your function with a pretty output inside `run_tests` function

Add your  function inside the `chose_specific_test` function (and if you can do it make it cleaner because at the current state it is a big if else if statememnt of at least +8 conditional branching)

## Author

- Mattis DALLEAU
