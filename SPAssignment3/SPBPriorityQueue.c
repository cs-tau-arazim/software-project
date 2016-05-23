#include "SPBPriorityQueue.h"
#include <stdlib.h>
#include <assert.h>

/*
 * SPBPriorityQueue.c
 */

struct sp_bp_queue_t {
	SPList list;
	int maxSize;
};

SPBPQueue spBPQueueCreate(int maxSize) {
	assert(maxSize > 0);
	SPBPQueue bpQueue = (SPBPQueue) malloc(sizeof(*bpQueue));
	if (bpQueue == NULL)
		return NULL;
	bpQueue->list = spListCreate();
	bpQueue->maxSize = maxSize;
	if (bpQueue->list == NULL)
		return NULL;
	return bpQueue;
}

void spBPQueueClear(SPBPQueue source) {
	if (source == NULL)
		return;
	spListClear(source->list);
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element) {
	if (source == NULL || element == NULL)
		return SP_BPQUEUE_INVALID_ARGUMENT;
	if (spListGetSize(source->list) == 0) {
		spListInsertFirst((source->list), element);
		return SP_BPQUEUE_SUCCESS;
	}

	SPListElement nextIter = spListGetFirst(source->list);

	while (nextIter != NULL && spListElementGetValue(nextIter) < spListElementGetValue(element)) {
		nextIter = spListGetNext(source->list);
	}

	if (nextIter == NULL) {
		spListInsertLast(source->list, element);
	} else {
		spListInsertBeforeCurrent(source->list, element);
	}
	if (spListGetSize(source->list) == (source->maxSize + 1)) {
		SPListElement iter = spListGetFirst(source->list);
		nextIter = iter;
		while (nextIter != NULL) {
			iter = nextIter;
			nextIter = spListGetNext(source->list);
		}

		nextIter = spListGetFirst(source->list);
		while (nextIter != iter) {
			nextIter = spListGetNext(source->list);
		}

		spListRemoveCurrent(source->list);
		return SP_BPQUEUE_FULL;
	}
	else {
		return SP_BPQUEUE_SUCCESS;
	}
}

