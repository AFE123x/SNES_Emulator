#ifndef COMMON_H
#define COMMON_H
void todo_func(char* file, int line);

/* Helper macro for exiting program on parts to do. */
#define TODO() todo_func(__FILE__,__LINE__);
#endif