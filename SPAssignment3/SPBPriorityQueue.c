#include "SPBPriorityQueue.h"
#include <stdlib.h>
#include "SPList.h"

/*
 * SPBPriorityQueue.c
 */

struct sp_bp_queue_t{
	SPList list;
	int bound;
	int size;
};
