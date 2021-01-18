#ifndef nml_TEST_H
#define nml_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lib/nml_util.h"

#define NML_TEST_HEADER(num_cases, input, testfile) \
    printf("Found %d test cases in file: " CYAN "'%s'" RESET "\n", num_cases, input); \
    printf("Running: " CYAN "%s(test_data = " YELLOW "%s" RESET CYAN ")" RESET "\n", testfile, input); \
    printf("Test cases: ");

#define TEST

#endif

#ifdef __cplusplus
}
#endif