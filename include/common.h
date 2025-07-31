#ifndef COMMON_H
#define COMMON_H
#include<stdbool.h>

void assert_func(bool condition, char* file, int line,char* message);
void todo_func(char* file, int line, char* quote);

/* Helper macro for exiting program on parts to do. */
#define TODO(quote) todo_func(__FILE__,__LINE__,quote);

#define ASSERT(condition,message) assert_func(condition,__FILE__,__LINE__,message);


#endif