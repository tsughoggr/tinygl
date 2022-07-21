#include <u.h>
#include <libc.h>
#include "msghandling.h"
#include "../include/GL/gl.h"

#include "zgl.h"


#ifdef __TINYC__
#define NO_DEBUG_OUTPUT
#endif

#ifndef NO_DEBUG_OUTPUT
#endif
/* Use this function to output messages when something unexpected
   happens (which might be an indication of an error). *Don't* use it
   when there's GLinternal errors in the code - these should be handled
   by asserts. */
void tgl_warning(const char* format, ...) {
#ifndef NO_DEBUG_OUTPUT
	va_list args;
	va_start(args, format);
	fprint(2, "*WARNING* ");
	vfprint(2, format, args);
	va_end(args);
#endif /* !NO_DEBUG_OUTPUT */
}

/* This function should be used for debug output only. */
void tgl_trace(const char* format, ...) {
#ifndef NO_DEBUG_OUTPUT
	va_list args;
	va_start(args, format);
	fprint(2, "*DEBUG* ");
	vfprint(2, format, args);
	va_end(args);
#endif /* !NO_DEBUG_OUTPUT */
}

/* Use this function to output info about things in the code which
   should be fixed (missing handling of special cases, important
   features not implemented, known bugs/buglets, ...). */
void tgl_fixme(const char* format, ...) {
#ifndef NO_DEBUG_OUTPUT
	va_list args;
	va_start(args, format);
	fprint(2, "*FIXME* ");
	vfprint(2, format, args);
	va_end(args);
#endif /* !NO_DEBUG_OUTPUT */
}

void gl_fatal_error(char* format, ...) {
#ifndef NO_DEBUG_OUTPUT
	va_list ap;
	va_start(ap, format);
	fprint(2, "TinyGL: fatal error: ");
	vfprint(2, format, ap);
	fprint(2, "\n");
	exits(nil);
	va_end(ap);
#else
	exits(nil);
#endif
}
