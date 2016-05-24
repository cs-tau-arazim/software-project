#include "SPBPriorityQueue.h"
#include "SPListElement.h"
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


SPBPQueue spBPQueueCopy(SPBPQueue source) {
	if (source == NULL)
		return NULL;

	SPBPQueue newQueue = (SPBPQueue) malloc(sizeof(*newQueue));
	if (newQueue == NULL)
		return NULL;
	newQueue->list = spListCopy(source->list);
	newQueue->maxSize = source->maxSize;
	if (newQueue->list == NULL)
		return NULL;
	return newQueue;
}

void spBPQueueDestroy(SPBPQueue source)
{
	if (source == NULL) {
		return;
	}
	spListDestroy(source->list);

	free(source);
}

int spBPQueueSize(SPBPQueue source)
{
	if (source == NULL)
		return -1;
	return spListGetSize(source->list);
}

int spBPQueueGetMaxSize(SPBPQueue source)
{
	if (source == NULL)
		return -1;
	return source->maxSize;
}

SPListElement spBPQueuePeek(SPBPQueue source)
{
	if (source == NULL || spListGetSize(source->list) == 0 || source->list == NULL)
		return NULL;
	SPListElement elem = spListGetFirst(source->list);
	return spListElementCopy(elem);
}

SPListElement spBPQueuePeekLast(SPBPQueue source)
{
	if (source == NULL || spListGetSize(source->list) == 0 || source->list == NULL)
		return NULL;
	SPListElement elem = spListGetFirst(source->list);
	SPListElement nextElem = spListGetNext(source->list);
	while (nextElem != NULL)
	{
		elem = nextElem;
		nextElem = spListGetNext(source->list);
	}
	return spListElementCopy(elem);
}

double spBPQueueMinValue(SPBPQueue source)
{
	if (source == NULL || spListGetSize(source->list) == 0 || source->list == NULL)
		return -1;
	return spListElementGetValue(spListGetFirst(source->list));
}

double spBPQueueMaxValue(SPBPQueue source)
{
	if (source == NULL || spListGetSize(source->list) == 0 || source->list == NULL)
		return -1;
	SPListElement elem = spListGetFirst(source->list);
	SPListElement nextElem = spListGetNext(source->list);
	while (nextElem != NULL)
	{
		elem = nextElem;
		nextElem = spListGetNext(source->list);
	}
	return spListElementGetValue(elem);
}

bool spBPQueueIsEmpty(SPBPQueue source)
{
	if (source == NULL || spListGetSize(source->list) == 0 || source->list == NULL)
		return true;
	return false;
}

bool spBPQueueIsFull(SPBPQueue source)
{
	if (source == NULL || spListGetSize(source->list) == 0 || source->list == NULL)
		return false;
	if (spListGetSize(source->list) == source->maxSize)
		return true;
	return false;
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

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source)
{
	if (source == NULL || spListGetSize(source->list) == 0 || source->list == NULL)
		return SP_BPQUEUE_INVALID_ARGUMENT;
	spListGetFirst(source->list);
	spListRemoveCurrent(source->list);
	return SP_BPQUEUE_SUCCESS;
}


