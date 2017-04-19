#ifndef FILE_HANDLING_TEST_H
#define FILE_HANDLING_TEST_H

#include "config.h"
#include "stdlib.h"
#include "sntptest.h"

#include <string.h>
#include <unistd.h>


enum DirectoryType {
	INPUT_DIR = 0,
	OUTPUT_DIR = 1
};

#define SRCDIR_DEF "/home/situlingyun/Documents/protocol-fuzzer/protocol-target/NTP-P8/A.x86_64-pc-ubuntu16.04-clang/sntp/../../sntp/tests/data/";

extern	const char * 	CreatePath(const char* filename,
				   enum DirectoryType argument);
extern	void		DestroyPath(const char* pathname);
extern	int		GetFileSize(FILE *file);
extern	bool		CompareFileContent(FILE* expected, FILE* actual);
extern	void		ClearFile(const char * filename) ;

#endif // FILE_HANDLING_TEST_H
