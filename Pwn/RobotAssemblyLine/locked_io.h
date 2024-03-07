#ifndef LOCKED_IO_H
#define LOCKED_IO_H

#include <stdio.h>
#include <stdarg.h>

int locked_vfprintf(FILE* fp, const char* format, va_list ap) __attribute__((format(printf, 2, 0)));
int locked_fprintf(FILE* fp, const char* format, ...) __attribute__((format(printf, 2, 3)));
int locked_printf(const char* format, ...) __attribute__((format(printf, 1, 2)));
void locked_perror(const char* prefix);
void locked_printerr(const char *prefix, int errval);


#endif /* LOCKED_IO_H */
