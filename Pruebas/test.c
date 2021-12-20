#include "test.h"
#include <stdio.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
void probar(const char* mensaje, bool ok ){
	printf("%s --->  ", mensaje );
	if (!ok){
		printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET "\n");
	}else{
		printf(ANSI_COLOR_GREEN "OK !" ANSI_COLOR_RESET "\n");
	}
}