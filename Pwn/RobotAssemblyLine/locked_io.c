#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "locked_io.h"


static pthread_mutex_t g_output_mutex = PTHREAD_MUTEX_INITIALIZER;


int locked_vfprintf(FILE* fp, const char* format, va_list ap)
{
	pthread_mutex_lock(&g_output_mutex);
	vfprintf(fp, format, ap);
	pthread_mutex_unlock(&g_output_mutex);
}


int locked_fprintf(FILE* fp, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	locked_vfprintf(fp, format, ap);
	va_end(ap);
}


int locked_printf(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	locked_vfprintf(stdout, format, ap);
	va_end(ap);
}


void locked_printerr(const char *prefix, int errval)
{
	char errbuf[100];
	int err = strerror_r(errval, errbuf, sizeof(errbuf));
	if (err) {
		snprintf(errbuf, sizeof(errbuf), "%s: %d\n", prefix, errval);
	}
	
	if (prefix) {
		locked_fprintf(stderr, "%s: %s\n", prefix, errbuf);
	}
	else {
		locked_fprintf(stderr, "%s\n", errbuf);
	}
}


void locked_perror(const char* prefix)
{
	locked_printerr(prefix, errno);
}
