/*
 * Memory allocator for TinyGL
 */

static inline void required_for_compilation_(){
	return;
}

#include <u.h>
#include <libc.h>
#include "zgl.h"

/* modify these functions so that they suit your needs */

void gl_free(void* p) { free(p); }

void* gl_malloc(GLint size) { return malloc(size); }

void* gl_zalloc(GLint size) { return calloc(1, size); }
