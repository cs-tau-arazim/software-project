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
	if (bpQueue->list == NULL) // check if list crate failed
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
	if (newQueue->list == NULL) // check if list copy failed
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
	if (spBPQueueIsEmpty(source))
		return NULL;

	SPListElement elem = spListGetFirst(source->list);
	return spListElementCopy(elem); // NEW COPY
}

SPListElement spBPQueuePeekLast(SPBPQueue source)
{
	if (spBPQueueIsEmpty(source))
		return NULL;

	SPListElement elem = spListGetFirst(source->list);
	SPListElement nextElem = spListGetNext(source->list);
	// get next until the last element
	while (nextElem != NULL)
	{
		elem = nextElem;
		nextElem = spListGetNext(source->list);
	}
	return spListElementCopy(elem); // NEW COPY
}

double spBPQueueMinValue(SPBPQueue source)
{
	if (spBPQueueIsEmpty(source))
		return -1;
	return spListElementGetValue(spListGetFirst(source->list));
}

double spBPQueueMaxValue(SPBPQueue source)
{
	if (spBPQueueIsEmpty(source))
		return -1;

	SPListElement elem = spListGetFirst(source->list);
	SPListElement nextElem = spListGetNext(source->list);
	// get next until the last element
	while (nextElem != NULL)
	{
		elem = nextElem;
		nextElem = spListGetNext(source->list);
	}
	return spListElementGetValue(elem);
}

bool spBPQueueIsEmpty(SPBPQueue source)
{
	// check if source doesn't exists or empty
	if (source == NULL || spListGetSize(source->list) == 0 || source->list == NULL)
		return true;
	return false;
}

bool spBPQueueIsFull(SPBPQueue source)
{
	if (spBPQueueIsEmpty(source))
		return false;
	if (spListGetSize(source->list) == source->maxSize) // check if full
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

	if (spListGetSize(source->list) == 0) { // if first element
		spListInsertFirst((source->list), element);
		return SP_BPQUEUE_SUCCESS;
	}

	SPListElement nextIter = spListGetFirst(source->list); // points to the next element on the list

	// search where to insert the element
	while (nextIter != NULL && spListElementGetValue(nextIter) < spListElementGetValue(element)) {
		nextIter = spListGetNext(source->list);
	}

	if (nextIter == NULL) { // reached the end of the list
		spListInsertLast(source->list, element);
	} else {
		spListInsertBeforeCurrent(source->list, element); // inserts the element before nextIter
	}

	if (spListGetSize(source->list) == (source->maxSize + 1)) { // passed the max size
		SPListElement iter = spListGetFirst(source->list);
		nextIter = iter;

		// search the end of the list
		while (nextIter != NULL) {
			iter = nextIter;
			nextIter = spListGetNext(source->list);
		}

		nextIter = spListGetFirst(source->list);
		// make the internal iterator of the list to point to the last element
		while (nextIter != iter) {
			nextIter = spListGetNext(source->list);
		}

		spListRemoveCurrent(source->list); // removes the last element
		return SP_BPQUEUE_FULL;
	}
	else {
		return SP_BPQUEUE_SUCCESS;
	}
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source)
{
	if (spBPQueueIsEmpty(source))
		return SP_BPQUEUE_INVALID_ARGUMENT;

	spListGetFirst(source->list);
	spListRemoveCurrent(source->list);

	return SP_BPQUEUE_SUCCESS;
}


