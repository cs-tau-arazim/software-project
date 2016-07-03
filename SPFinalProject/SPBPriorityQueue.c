#include "SPBPriorityQueue.h"
#include <stdlib.h>

/*
 * SPBPriorityQueue.c
 */

/**
 * the internal struct sp_bp_queue_t- we will implement the BPQueue using the already
 * implemented list, and a maxSize field.
 */
struct sp_bp_queue_t {
	SPList list;
	int maxSize;
};

/**
 * Create a new queue
 */
SPBPQueue spBPQueueCreate(int maxSize) {
	SPBPQueue bpQueue;
	if (maxSize <= 0) // check edge case
		return NULL;

	bpQueue = (SPBPQueue) malloc(sizeof(*bpQueue)); // allocate queue
	if (bpQueue == NULL)
		return NULL;

	bpQueue->list = spListCreate();
	bpQueue->maxSize = maxSize;
	if (bpQueue->list == NULL) // check if list create failed
		return NULL;

	return bpQueue;
}

SPBPQueue spBPQueueCopy(SPBPQueue source) {
	SPBPQueue newQueue;
	if (source == NULL) // check edge case
		return NULL;

	newQueue = (SPBPQueue) malloc(sizeof(*newQueue));
	if (newQueue == NULL)
		return NULL;

	newQueue->list = spListCopy(source->list);
	newQueue->maxSize = source->maxSize;
	if (newQueue->list == NULL) { // check if list copy failed
		spBPQueueDestroy(newQueue);
		return NULL;
	}
	return newQueue;
}

void spBPQueueDestroy(SPBPQueue source) {
	if (source == NULL) {
		return;
	}

	spListDestroy(source->list);
	free(source);
}

int spBPQueueSize(SPBPQueue source) {
	if (source == NULL)
		return -1;
	return spListGetSize(source->list);
}

int spBPQueueGetMaxSize(SPBPQueue source) {
	if (source == NULL)
		return -1;
	return source->maxSize;
}

SPListElement spBPQueuePeek(SPBPQueue source) {
	SPListElement elem;
	if (spBPQueueIsEmpty(source))
		return NULL;

	elem = spListGetFirst(source->list);
	return spListElementCopy(elem); // NEW COPY
}

SPListElement spBPQueuePeekLast(SPBPQueue source) {
	SPListElement elem, nextElem;
	if (spBPQueueIsEmpty(source))
		return NULL;

	elem = spListGetFirst(source->list);
	nextElem = spListGetNext(source->list);

	// get next until the last element
	while (nextElem != NULL) {
		elem = nextElem;
		nextElem = spListGetNext(source->list);
	}
	return spListElementCopy(elem); // NEW COPY
}

double spBPQueueMinValue(SPBPQueue source) {
	if (spBPQueueIsEmpty(source))
		return -1;
	return spListElementGetValue(spListGetFirst(source->list));
}

double spBPQueueMaxValue(SPBPQueue source) {
	SPListElement elem, nextElem;
	if (spBPQueueIsEmpty(source))
		return -1;

	elem = spListGetFirst(source->list);
	nextElem = spListGetNext(source->list);

	// get next until the last element
	while (nextElem != NULL) {
		elem = nextElem;
		nextElem = spListGetNext(source->list);
	}
	return spListElementGetValue(elem);
}

bool spBPQueueIsEmpty(SPBPQueue source) {
	// check if source doesn't exists or empty
	if (source == NULL
			|| spListGetSize(source->list) == 0|| source->list == NULL)
		return true;
	return false;
}

bool spBPQueueIsFull(SPBPQueue source) {
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
	SPListElement nextIter, iter;
	double elementValue, iterValue;
	int elementIndex, iterIndex;

	// check edge cases
	if (source == NULL || element == NULL)
		return SP_BPQUEUE_INVALID_ARGUMENT;

	if (spListGetSize(source->list) == 0) { // if first element
		spListInsertFirst((source->list), element);
		return SP_BPQUEUE_SUCCESS;
	}

	nextIter = spListGetFirst(source->list); // points to the next element on the list

	iterValue = spListElementGetValue(nextIter);
	elementValue = spListElementGetValue(element);
	iterIndex = spListElementGetIndex(nextIter);
	elementIndex = spListElementGetIndex(element);

	// search where to insert the element
	while (nextIter != NULL
			&& (iterValue < elementValue || (iterValue == elementValue && iterIndex < elementIndex))) {
		nextIter = spListGetNext(source->list);
		iterValue = spListElementGetValue(nextIter);
		iterIndex = spListElementGetIndex(nextIter);
	}

	if (nextIter == NULL) { // reached the end of the list
		// check if malloc failed
		if (spListInsertLast(source->list, element) == SP_LIST_OUT_OF_MEMORY) {
			return SP_BPQUEUE_OUT_OF_MEMORY;
		}

	} else {
		// check if malloc failed
		if (spListInsertBeforeCurrent(source->list, element)
				== SP_LIST_OUT_OF_MEMORY) { // inserts the element before nextIter
			return SP_BPQUEUE_OUT_OF_MEMORY;
		}
	}

	if (spListGetSize(source->list) == (source->maxSize + 1)) { // passed the max size
		iter = spListGetFirst(source->list);
		nextIter = iter;

		// search the end of the list
		while (nextIter != NULL) {
			iter = nextIter;
			nextIter = spListGetNext(source->list);
		}
		// now 'iter' equal last element in queue

		nextIter = spListGetFirst(source->list);
		// make the internal iterator of the list to point to the last element
		while (nextIter != iter) {
			nextIter = spListGetNext(source->list);
		}


		// check if the element inserted is at the end and now will be removed.
		if(spListElementCompare(iter, element ) == 0) {
			spListRemoveCurrent(source->list); // removes the last element
			return SP_BPQUEUE_FULL; // notify user that insertion failed
		}

		spListRemoveCurrent(source->list); // removes the last element
		return SP_BPQUEUE_SUCCESS;
	} else {
		return SP_BPQUEUE_SUCCESS;
	}
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source) {
	// check if source queue is invalid
	if (spBPQueueIsEmpty(source))
		return SP_BPQUEUE_INVALID_ARGUMENT;

	spListGetFirst(source->list);
	spListRemoveCurrent(source->list);

	return SP_BPQUEUE_SUCCESS;
}

