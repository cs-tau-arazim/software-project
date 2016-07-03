#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "SPImageProc.h"

#define ENTER_QUERY "Please enter an image path:\n"
#define EXITING "Exiting..."
#define BEST_CANDIDATES "Best candidates for - %s - are:\n"
extern "C" {
#include "main_aux.h"
}

#define LINE_LENGTH 1024

// For Debbuging:
// 	printf("%d, %s\n",__LINE__, __func__); //TODO remove


int main() {
	treeTest();
}
