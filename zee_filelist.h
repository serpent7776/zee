/* vim: set ts=4 sw=4 cin: */
/*
 * Copyright © 2013 Midgard Serpent. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *	1. Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright
 *	   notice, this list of conditions and the following disclaimer in the
 *	   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Project:   zee
 * File:      zee_filelist.h
 * Version:   0.41
 * By:        Midgard Serpent
 */
#ifndef ZEE_FILELIST_H
#define ZEE_FILELIST_H 41

#include <stdlib.h>
#include <sys/errno.h>

//==============================================================================
//structure contains  list of file descriptors
//it is implemented as a table actually
typedef struct{
	int size;	//number of allocated elements
	int count;	//number of used elements
	int* buffer;
}zee_filelist;

inline int zfl_size(zee_filelist* zfl)
{
	return zfl ? zfl->size : -1;
}
inline int zfl_count(zee_filelist* zfl)
{
	return zfl ? zfl->count : -1;
}

inline zee_filelist* zfl_create()
{
	zee_filelist* zfl=malloc(sizeof(zee_filelist));
	if (zfl){
		zfl->size=2;	//allocate memory for 8 file descriptors
		zfl->count=0;
		zfl->buffer=malloc(sizeof(int)*(zfl->size));
		if (zfl->buffer){
			return zfl;
		}
	}
	return NULL;
}

inline void zfl_destroy(zee_filelist* zfl)
{
	if (zfl!=NULL){
		free(zfl->buffer);
		free(zfl);
	}
}

inline int zfl_add(zee_filelist* zfl, int fd)
{
	if (zfl==NULL){
		errno=EINVAL;
		return -1;
	}
	if (zfl->buffer==NULL){
		errno=EFAULT;
		return -1;
	}

	if (zfl->count>=zfl->size){
		zfl->size+=32;
		zfl->buffer=realloc(zfl->buffer, zfl->size);
	}
	zfl->buffer[zfl->count]=fd;
	return zfl->count++;
}

inline int zfl_get(zee_filelist* zfl, int i)
{
	if (zfl==NULL){
		errno=EINVAL;
		return -1;
	}
	if (zfl->buffer==NULL){
		errno=EFAULT;
		return -1;
	}

	return i<zfl->count ? zfl->buffer[i] : -1;
}
//==============================================================================

#endif
