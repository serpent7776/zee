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
 * File:      zee.c
 * Version:   0.41
 * By:        Midgard Serpent
 */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include "zee.h"
#include "zee_filelist.h"

//==============================================================================
typedef struct{
	int	input;	//input descriptor
	int	bufsize;	//buffer size
	zee_filelist*	output;	//list of output descriptors
	char*	buffer;
}_Zee;

//==============================================================================
Zee zee_create()
{
	_Zee* zee=malloc(sizeof(_Zee)); 
	if (zee){
		//errno=0;
		zee->input=0;	//read data from stdin
		zee->bufsize=256;
		zee->buffer=NULL;	//buffer will be allocated in zee_run
		zee->output=zfl_create();
		return (zee->output!=NULL) ? (Zee)zee : NULL;
	}
	return NULL;
}

void zee_destroy(Zee z)
{
	_Zee* zee=(_Zee*)z;
	zfl_destroy(zee->output);
	free(zee);
}

void zee_set_input(Zee z, int fd)
{
	_Zee* zee=(_Zee*)z;
	zee->input=fd;
}

//add given file descriptor to file list
bool zee_add_output_d(Zee z, int fd)
{
	_Zee* zee=(_Zee*)z;
	if (zee==NULL || fd<=0){
		errno=EFAULT;
		return false;
	}

	return zfl_add(zee->output, fd)>=0;
}

//add file filename to filelist and open it in given mode
bool zee_add_output_p(Zee z, const char* filename, char mode)
{
	_Zee* zee=(_Zee*)z;
	if (zee==NULL || filename==NULL){
		errno=EFAULT;
		return false;
	}
	int F;
	switch(mode){
		case 'w': F=O_TRUNC;	break;
		case 'a': F=O_APPEND;	break;
		default: errno=EINVAL; return false;
	}
	const int flags= O_WRONLY | O_CREAT | /*O_SEQUENTIAL | O_BINARY |*/ F;
	const mode_t perm=S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
	int fd=open(filename, flags, perm);
	if (fd!=-1){
		return zfl_add(zee->output, fd)>=0;
	}
	return false;
}

bool zee_run(Zee z)
{
	_Zee* zee=(_Zee*)z;
	if (zee->buffer)	//zee->buffer should always be NULL here
		free(zee->buffer);

	zee->buffer=malloc(zee->bufsize);
	if (zee->buffer){
		int rd;
		for(;;){
			rd=read(zee->input, zee->buffer, zee->bufsize);
			if (rd<=0) break;
			const int sz=zfl_count(zee->output);
			for(int j=0; j<sz; ++j){
				write(zfl_get(zee->output, j), zee->buffer, rd);
			}
		}
		free(zee->buffer);
	}
	return false;
}

//==============================================================================

