#include "SPBPriorityQueue.h"
#include <stdlib.h>
#include "SPList.h"

/*
 * SPBPriorityQueue.c
 */

struct sp_bp_queue_t{
	SPList list;
	int size;
	int maxSize;
};

SPBPQueue spBPQueueCreate(int maxSize) {
	// TODO assert maxsize > 0
	SPBPQueue bpQueue = (SPBPQueue) malloc(sizeof(*bpQueue));
	if (bpQueue == NULL)
		return NULL;
	bpQueue->list = spListCreate();
	bpQueue->maxSize = maxSize;
	bpQueue->size = 0;
	if ( bpQueue->list == NULL)
		return NULL;
	return bpQueue;
}
