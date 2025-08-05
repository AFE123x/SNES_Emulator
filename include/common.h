#ifndef COMMON_H
#define COMMON_H
#include<stdbool.h>
void assert_func(bool condition, const char* file, int line, const char* format, ...);
void todo_func(char* file, int line, char* quote);

/* Helper macro for exiting program on parts to do. */
#define TODO(quote) todo_func(__FILE__,__LINE__,quote);

#define ASSERT(condition, ...) assert_func(condition, __FILE__, __LINE__, __VA_ARGS__);


#endif