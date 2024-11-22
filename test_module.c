#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include "shared/file.c"
#include "shared/string.c"

#ifdef test_success
    #define code 0
    #define output "succesful compilation output"
    #define config "--TRUE .TRUE"
    #define name "DEBUG_OK"
#else
    #define code 1
    #define output "testing failure behavior"
    #define config "--FALSE .FALSE"
    #define name "DEBUG_NO"
#endif

int main (int argc, char **argv) {
    if (argc == 3) {
        file_write(argv[2], output, sizeof(output));
        return code;
    }
    else if (argc == 2) {
        if (string_compare(argv[1], "--version", 16)) { printf(name ", version 0.0.0\n"); exit(0); }
        if (string_compare(argv[1], "--contributors", 16)) { printf(name ": John Alex, Modula.dev\n"); exit(0); }
        if (string_compare(argv[1], "--mconfig", 16)) { printf(name "\n", config); exit(0); }
        if (string_compare(argv[1], "--license", 16)) { printf(name ": GNU GPLv3\n"); exit(0); }
    }
    else { printf("usage: %s input output\n"); exit(1); }
}