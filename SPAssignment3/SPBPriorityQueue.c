#include "SPBPriorityQueue.h"
#include "SPListElement.h"
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

SPBPQueue spBPQueueCopy(SPBPQueue source) {
	SPBPQueue newQueue = (SPBPQueue) malloc(sizeof(*newQueue));
	if (newQueue == NULL)
		return NULL;
	newQueue->list = spListCopy(source->list);
	newQueue->maxSize = source->maxSize;
	newQueue->size = source->size;
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


