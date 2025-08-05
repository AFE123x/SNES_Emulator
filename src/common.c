#include "../include/common.h"
#include <stdio.h>
#include <stdlib.h>
#include<stdio.h>
#include<stdarg.h>

/**
 * A helper function to print out parts not implemented yet
 * @param file - name of the file called.
 * @param line - line number where todo is placed.
 */
void todo_func(char* file, int line, char* quote){
    fprintf(stderr,"Not Implemented yet: %s, line %d\n%s\n",file,line,quote);
    exit(1);
}

/**
 * A custom assertion function to include a message, similar to Rust's assert!() macro
 * @param condition - condition assertion check. if it's false, assert prints error, and quits program
 * @param file - file where assertion is located
 * @param line - line number where assertion is located
 * @param message - custom message to define what failure means
 */
void assert_func(bool condition, const char* file, int line, const char* format, ...) {
    if (!condition) {
        fprintf(stderr, "assertion %s, line %d FAILED: ", file, line);

        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);  // Use vfprintf to handle format + args
        va_end(args);

        fprintf(stderr, "\n");
        exit(1);
    }
}