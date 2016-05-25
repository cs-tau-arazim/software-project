#include "unit_test_util.h"
#include "../SPListElement.h"
#include "../SPList.h"
#include "../SPBPriorityQueue.h"
#include "../SPLogger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

//static SPBPQueue quickQ(int size, ...);
static int maxSize = 20;

static SPBPQueue quickQ(int size, ...) {
	va_list items;
	SPBPQueue source = spBPQueueCreate(maxSize);
	va_start(items, size);
	for (int i = 0; i < size; i++) {
		spBPQueueEnqueue(source, va_arg(items, SPListElement));
	}
	va_end(items);
	return source;
}

static bool testQCreate() {
	SPBPQueue source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(source != NULL);
	spBPQueueDestroy(source);
	return true;
}

static bool testQCopy() {
	//spLoggerCreate(NULL,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);

	ASSERT_TRUE(spBPQueueCopy(NULL) == NULL);
	//spLoggerPrintDebug("null check","spb_priority_queue_test.c",__func__,__LINE__);
	SPBPQueue source = spBPQueueCreate(10);
	SPBPQueue copy = spBPQueueCopy(source);



	ASSERT_TRUE(copy != NULL);
	ASSERT_TRUE(0 == spBPQueueSize(copy));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(0 == spBPQueueSize(copy));
	SPBPQueue source2 = quickQ(4, e1, e2, e3, e4);
	SPBPQueue copy2 = spBPQueueCopy(source2);
	ASSERT_TRUE(4 == spBPQueueSize(copy2));

	SPListElement epeek = spBPQueuePeek(copy2);
	ASSERT_TRUE(spListElementCompare(e1, epeek) == 0);
	spBPQueueDequeue(copy2);
	spListElementDestroy(epeek);	

	epeek = spBPQueuePeek(copy2);
	ASSERT_TRUE(spListElementCompare(e2, epeek) == 0);
	spBPQueueDequeue(copy2);
	spListElementDestroy(epeek);	

	epeek = spBPQueuePeek(copy2);
	ASSERT_TRUE(spListElementCompare(e3, epeek) == 0);
	spBPQueueDequeue(copy2);
	spListElementDestroy(epeek);	

	epeek = spBPQueuePeek(copy2);
	ASSERT_TRUE(spListElementCompare(e4, epeek) == 0);
	spBPQueueDequeue(copy2);
	spListElementDestroy(epeek);	

	epeek = spBPQueuePeek(copy2);
	ASSERT_TRUE(epeek == NULL);

	spListElementDestroy(epeek);	
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spBPQueueDestroy(copy);
	spBPQueueDestroy(copy2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}


static bool testQGetSize() {
	SPBPQueue source = NULL;
	ASSERT_TRUE(-1 == spBPQueueSize(source));
	source = quickQ(0);
	ASSERT_TRUE(0 == spBPQueueSize(source));
	SPListElement e1 = spListElementCreate(1, 1.0);
	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(1 == spBPQueueSize(source));
	spListElementSetIndex(e1, 2);
	spListElementSetValue(e1, 2.0);
	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(2 == spBPQueueSize(source));
	spBPQueueDequeue(source);
	ASSERT_TRUE(1 == spBPQueueSize(source));
	for (int i = 0 ; i < 2*maxSize ; i++)
	{
		ASSERT_TRUE(spBPQueueSize(source) <= maxSize);
		SPListElement e = spListElementCreate(i, 1.0);
		spBPQueueEnqueue(source, e);
		spListElementDestroy(e);
	}
	spBPQueueDestroy(source);
	spListElementDestroy(e1);


	return true;
}

static bool testQGetMaxSize() {
	SPBPQueue source = NULL;
	ASSERT_TRUE(-1 == spBPQueueGetMaxSize(source));
	source = quickQ(0);
	ASSERT_TRUE(maxSize == spBPQueueGetMaxSize(source));
	SPListElement e1 = spListElementCreate(1, 1.0);
	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(maxSize == spBPQueueGetMaxSize(source));
	spListElementSetIndex(e1, 2);
	spListElementSetValue(e1, 2.0);
	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(maxSize == spBPQueueGetMaxSize(source));
	spBPQueueDequeue(source);
	ASSERT_TRUE(maxSize == spBPQueueGetMaxSize(source));
	spBPQueueDestroy(source);
	spListElementDestroy(e1);
	return true;
}


static bool testQPeek() {
	SPBPQueue source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueuePeek(source) == NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue source2 = quickQ(4, e1, e2, e3, e4);
	SPListElement first = spBPQueuePeek(source2);
	SPListElement first2 = spBPQueuePeek(source2);
	ASSERT_TRUE(spListElementCompare(e1, first) == 0);
	ASSERT_TRUE(
			spListElementCompare(first, first2) == 0
					&& first2 != first); // new copy so not ==
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spListElementDestroy(first);
	spListElementDestroy(first2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testQPeekLast() {
	SPBPQueue source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueuePeekLast(source) == NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue source2 = quickQ(4, e1, e2, e3, e4);
	SPListElement last = spBPQueuePeekLast(source2);
	SPListElement last2 = spBPQueuePeekLast(source2);
	ASSERT_TRUE(spListElementCompare(e4, last) == 0);
	ASSERT_TRUE(
			spListElementCompare(last, last2) == 0
					&& last2 != last); // new copy so not ==
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spListElementDestroy(last);
	spListElementDestroy(last2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testQMinValue() {
	SPBPQueue source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueMinValue(source) == -1);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue source2 = quickQ(4, e1, e2, e3, e4);
	SPBPQueue source3 = quickQ(4, e4, e3, e1, e2);
	double min2  = spBPQueueMinValue(source2);
	double min3  = spBPQueueMinValue(source3);
	ASSERT_TRUE(min2 == 1.0);
	ASSERT_TRUE(min3 == 1.0);
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spBPQueueDestroy(source3);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testQMaxValue() {
	SPBPQueue source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueMinValue(source) == -1);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue source2 = quickQ(4, e1, e2, e3, e4);
	SPBPQueue source3 = quickQ(4, e4, e3, e1, e2);
	double max2  = spBPQueueMaxValue(source2);
	double max3  = spBPQueueMaxValue(source3);
	ASSERT_TRUE(max2 == 4.0);
	ASSERT_TRUE(max3 == 4.0);
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spBPQueueDestroy(source3);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testQEmpty() {
	SPBPQueue source = NULL;
	ASSERT_TRUE(spBPQueueIsEmpty(source) == true);
	source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueIsEmpty(source) == true);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPBPQueue source2 = quickQ(1, e1);
	ASSERT_TRUE(spBPQueueIsEmpty(source2) == false);
	spBPQueueDequeue(source2);
	ASSERT_TRUE(spBPQueueIsEmpty(source) == true);
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spListElementDestroy(e1);
	return true;
}

static bool testQFull() {
	SPBPQueue source = NULL;
	ASSERT_TRUE(spBPQueueIsFull(source) == false);
	source = spBPQueueCreate(maxSize);
	while(spBPQueueSize(source) < maxSize)
	{
		ASSERT_TRUE(spBPQueueIsFull(source) == false);
		SPListElement e1 = spListElementCreate(1, 1.0);
		spBPQueueEnqueue(source, e1);
		spListElementDestroy(e1);
	}
	ASSERT_TRUE(spBPQueueIsFull(source) == true);
	spBPQueueDestroy(source);
	return true;
}


static bool testQEnqueue() {
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueEnqueue(NULL, NULL));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue source = quickQ(3, e2, e1, e4);
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, e3));
	ASSERT_TRUE(4 == spBPQueueSize(source));
	SPListElement peek = spBPQueuePeek(source);
	SPListElement peekLast = spBPQueuePeekLast(source);
	ASSERT_TRUE(spListElementCompare(e1, peek)==0);
	ASSERT_TRUE(spListElementCompare(e4, peekLast)==0);

	SPBPQueue source2 = spBPQueueCreate(maxSize);
	for (int i = 0 ; i < maxSize ; i++)
	{
		SPListElement e = spListElementCreate(i, (double)i);
		ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source2, e));
		spListElementDestroy(e);
	}
	for (int i = maxSize ; i < 2*maxSize ; i++)
	{
		SPListElement e = spListElementCreate(i, (double)i);
		ASSERT_TRUE(SP_BPQUEUE_FULL == spBPQueueEnqueue(source2, e));
		spListElementDestroy(e);
	}
	ASSERT_TRUE(spBPQueueMinValue(source2) == 0.0);

	ASSERT_TRUE((int)spBPQueueMaxValue(source2) == maxSize-1);
	spBPQueueClear(source2);
	for (int i = 2*maxSize-1 ; i >=0 ; i--)
	{
		SPListElement e = spListElementCreate(i, (double)i);
		spBPQueueEnqueue(source2, e);
		spListElementDestroy(e);
	}
	ASSERT_TRUE(spBPQueueMinValue(source2) == 0.0);
	ASSERT_TRUE((int)spBPQueueMaxValue(source2) == maxSize-1);
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spListElementDestroy(peek);
	spListElementDestroy(peekLast);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testQDequeue() {
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueDequeue(NULL));

	SPBPQueue source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueDequeue(source));
	for (int i = 0 ; i < maxSize ; i++)
	{
		SPListElement e = spListElementCreate(i, (double)i);
		spBPQueueEnqueue(source, e);
		spListElementDestroy(e);
	}
	for (int i = 0 ; i < maxSize ; i++)
	{
		ASSERT_TRUE((int)spBPQueueMinValue(source) == i);
		ASSERT_TRUE(spBPQueueDequeue(source) == SP_BPQUEUE_SUCCESS);
	}
	spBPQueueDestroy(source);
	return true;
}


static bool testQClear() {
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue source = quickQ(4, e1, e2, e3, e4);
	spBPQueueClear(source);
	ASSERT_TRUE(0 == spBPQueueSize(source));


	SPBPQueue source2 = spBPQueueCreate(maxSize);
	spBPQueueClear(source2);
	ASSERT_TRUE(0 == spBPQueueSize(source));

	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testQDestroy() {
	spBPQueueDestroy(NULL);
	return true;
}



int main() {
	RUN_TEST(testQCreate);
	RUN_TEST(testQCopy);

	RUN_TEST(testQEnqueue);
	RUN_TEST(testQDequeue);

	RUN_TEST(testQGetSize);
	RUN_TEST(testQGetMaxSize);
	RUN_TEST(testQPeek);
	RUN_TEST(testQPeekLast);
	RUN_TEST(testQMinValue);
	RUN_TEST(testQMaxValue);
	RUN_TEST(testQEmpty);
	RUN_TEST(testQFull);

	RUN_TEST(testQClear);
	RUN_TEST(testQDestroy);

	return 0;
}
