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
 * File:      main.c
 * Version:   0.41
 * By:        Midgard Serpent
 */

#include <sys/stat.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <wchar.h>*/
#include "zee.h"

#define ZEEVER              0.41
#define ZEEVERINT           41
#define ZEEVERSTR           "zee v0.41"

//==============================================================================
const char* s_help= \
  "read from stdin and write to stdout and file\n" \
  "usage: zee [file]\n" \
  "if file is not given write only to stdout\n";
const char* s_err="zee error";
const char* E_write="cannot open file '%s' for writing\n";
const char* E_invopt="Invalid option -%s\n";
//==============================================================================
int main(int argc, char** argv)
{
	errno=0;
	Zee zee=zee_create();
	if (zee==NULL){
		perror(s_err);
		return 2;	//init error
	}

	zee_add_output_d(zee, 1); //always write to stdout 

	char mode='w';	//'a' - append; 'w' - overwrite
	for(int arg=1; arg<argc; ++arg){
		if (argv[arg][0]=='-'){
			const char* str=&argv[arg][1];
			if (strcmp(str, "a")==0){
				mode='a';
			}else if (strcmp(str, "w")==0){
				mode='w';
			}else if (strcmp(str, "h")==0){
				fprintf(stderr, "%s by Midgard Serpent\n%s", ZEEVERSTR, s_help);
				return 0;
			}else{
				fprintf(stderr, E_invopt, str);
				fprintf(stderr, "%s by Midgard Serpent\n%s", ZEEVERSTR, s_help);
				return 1; //invalid option given in commandline
			}
		}else{
			const bool b=zee_add_output_p(zee, argv[arg], mode);
			if (!b){
				fprintf(stderr, E_write, argv[arg]);
				perror(s_err);
			}
		}
	}

	zee_run(zee);

	return 0;
}

//==============================================================================
