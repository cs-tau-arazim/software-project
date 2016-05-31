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


// macros often used in the code:

#define CREATE_4_ELEMENTS() \
	e1 = spListElementCreate(1, 1.0);\
	e2 = spListElementCreate(2, 2.0);\
	e3 = spListElementCreate(3, 3.0);\
	e4 = spListElementCreate(4, 4.0);\

#define DESTROY_4_ELEMENTS() \
	spListElementDestroy(e1);\
	spListElementDestroy(e2);\
	spListElementDestroy(e3);\
	spListElementDestroy(e4);\


static int maxSize = 100;

static SPBPQueue quickQ(int size, ...) {
	SPBPQueue source;

	va_list items;
	source = spBPQueueCreate(maxSize);
	va_start(items, size);
	for (int i = 0; i < size; i++) {
		spBPQueueEnqueue(source, va_arg(items, SPListElement));
	}
	va_end(items);
	return source;
}

static bool bpqCreateTest() {
	SPBPQueue source;

	source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(source != NULL);
	spBPQueueDestroy(source);
	return true;
}

static bool bpqCopyTest() {
	SPBPQueue source, source2, copy, copy2;
	SPListElement e1, e2, e3, e4, epeek;

	ASSERT_TRUE(spBPQueueCopy(NULL) == NULL); // check edge case

	source = spBPQueueCreate(10);
	copy = spBPQueueCopy(source);
	ASSERT_TRUE(copy != NULL);
	ASSERT_TRUE(0 == spBPQueueSize(copy));

	CREATE_4_ELEMENTS(); // e1, e2, e3, e4

	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(0 == spBPQueueSize(copy)); // ensure the copy is a NEW COPY

	source2 = quickQ(4, e1, e2, e3, e4);
	copy2 = spBPQueueCopy(source2);
	ASSERT_TRUE(4 == spBPQueueSize(copy2)); // check that size of copy is correct


	// check that all elements copied correctly
	epeek = spBPQueuePeek(copy2);
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

	// free all memory
	spListElementDestroy(epeek);	
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spBPQueueDestroy(copy);
	spBPQueueDestroy(copy2);
	DESTROY_4_ELEMENTS(); // e1, e2, e3, e4
	return true;
}


static bool bpqGetSizeTest() {
	SPBPQueue source = NULL;
	SPListElement e1;

	ASSERT_TRUE(-1 == spBPQueueSize(source)); //check edge case

	source = quickQ(0);
	ASSERT_TRUE(0 == spBPQueueSize(source));

	// insert a new element and check size
	e1 = spListElementCreate(1, 1.0);
	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(1 == spBPQueueSize(source));

	// make sure that inserting same element twice works
	spListElementSetIndex(e1, 2);
	spListElementSetValue(e1, 2.0);
	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(2 == spBPQueueSize(source));

	// remove an element and check size
	spBPQueueDequeue(source);
	ASSERT_TRUE(1 == spBPQueueSize(source));

	// insert more then maxSize elements and check that size is always less then maxSize
	for (int i = 0 ; i < 2*maxSize ; i++)
	{
		SPListElement e;
		ASSERT_TRUE(spBPQueueSize(source) <= maxSize);
		e = spListElementCreate(i, 1.0);
		spBPQueueEnqueue(source, e);
		spListElementDestroy(e);
	}

	// free memory
	spBPQueueDestroy(source);
	spListElementDestroy(e1);

	return true;
}

static bool bpqGetMaxSizeTest() {
	SPBPQueue source = NULL;
	SPListElement e1;

	ASSERT_TRUE(-1 == spBPQueueGetMaxSize(source)); //check edge case

	// check that max size is always maxSize
	source = quickQ(0);
	ASSERT_TRUE(maxSize == spBPQueueGetMaxSize(source));

	// insert a new element and check max size
	e1 = spListElementCreate(1, 1.0);
	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(maxSize == spBPQueueGetMaxSize(source));

	// insert another element and check max size
	spListElementSetIndex(e1, 2);
	spListElementSetValue(e1, 2.0);
	spBPQueueEnqueue(source, e1);
	ASSERT_TRUE(maxSize == spBPQueueGetMaxSize(source));

	// remove an element and check max size
	spBPQueueDequeue(source);
	ASSERT_TRUE(maxSize == spBPQueueGetMaxSize(source));

	// free memory
	spBPQueueDestroy(source);
	spListElementDestroy(e1);
	return true;
}


static bool bpqPeekTest() {
	SPBPQueue source, source2;
	SPListElement  e1, e2, e3, e4, first, first2;

	source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueuePeek(source) == NULL); // check edge case

	CREATE_4_ELEMENTS(); // e1, e2, e3, e4

	// insert element in unsorted order
	source2 = quickQ(4, e4, e2, e1, e3);

	first = spBPQueuePeek(source2);
	first2 = spBPQueuePeek(source2);

	ASSERT_TRUE(spListElementCompare(e1, first) == 0); // check that peek is the minimum
	ASSERT_TRUE(
			spListElementCompare(first, first2) == 0
					&& first2 != first); // new copy so not ==

	// free memory
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spListElementDestroy(first);
	spListElementDestroy(first2);
	DESTROY_4_ELEMENTS(); // e1, e2, e3, e4
	return true;
}

static bool bpqPeekLastTest() {
	SPBPQueue source, source2;
	SPListElement e1, e2, e3, e4, last, last2;

	source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueuePeekLast(source) == NULL); // check edge case

	CREATE_4_ELEMENTS(); // e1, e2, e3, e4

	// insert element in unsorted order
	source2 = quickQ(4, e3, e1, e4, e2);

	last = spBPQueuePeekLast(source2);
	last2 = spBPQueuePeekLast(source2);

	ASSERT_TRUE(spListElementCompare(e4, last) == 0); // check that peekLast is the maximum
	ASSERT_TRUE(
			spListElementCompare(last, last2) == 0
					&& last2 != last); // new copy so not ==

	// free memory
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spListElementDestroy(last);
	spListElementDestroy(last2);
	DESTROY_4_ELEMENTS(); // e1, e2, e3, e4
	return true;
}

static bool bpqMinValueTest() {
	SPBPQueue source, source2, source3;
	SPListElement e1, e2, e3, e4;

	source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueMinValue(source) == -1); // check edge case

	CREATE_4_ELEMENTS(); // e1, e2, e3, e4

	source2 = quickQ(4, e1, e2, e3, e4);
	source3 = quickQ(4, e4, e3, e1, e2);
	double min2  = spBPQueueMinValue(source2);
	double min3  = spBPQueueMinValue(source3);

	// check that min value is 1 in both cases
	ASSERT_TRUE(min2 == 1.0);
	ASSERT_TRUE(min3 == 1.0);

	// free memory
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spBPQueueDestroy(source3);
	DESTROY_4_ELEMENTS(); // e1, e2, e3, e4
	return true;
}

static bool bpqMaxValueTest() {
	SPBPQueue source, source2, source3;
	SPListElement e1, e2, e3, e4;

	source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueMinValue(source) == -1); // check edge case

	CREATE_4_ELEMENTS(); // e1, e2, e3, e4

	source2 = quickQ(4, e1, e2, e3, e4);
	source3 = quickQ(4, e4, e3, e1, e2);
	double max2  = spBPQueueMaxValue(source2);
	double max3  = spBPQueueMaxValue(source3);

	// check that min value is 4 in both cases
	ASSERT_TRUE(max2 == 4.0);
	ASSERT_TRUE(max3 == 4.0);

	// free memory
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spBPQueueDestroy(source3);
	DESTROY_4_ELEMENTS(); // e1, e2, e3, e4
	return true;
}

static bool bpqEmptyTest() {
	SPBPQueue source, source2;
	SPListElement e1;

	source = NULL;
	ASSERT_TRUE(spBPQueueIsEmpty(source) == true); // check edge case

	source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueIsEmpty(source) == true); // check that new queue is empty

	// insert a new element and check that not empty
	e1 = spListElementCreate(1, 1.0);
	source2 = quickQ(1, e1);
	ASSERT_TRUE(spBPQueueIsEmpty(source2) == false);

	// remove the element and check that empty
	spBPQueueDequeue(source2);
	ASSERT_TRUE(spBPQueueIsEmpty(source) == true);

	// free memory
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spListElementDestroy(e1);
	return true;
}

static bool bpqFullTest() {
	SPBPQueue source;

	source = NULL;
	ASSERT_TRUE(spBPQueueIsFull(source) == false); // check edge case

	source = spBPQueueCreate(maxSize);

	// insert maxSize element and check that full at the end
	while(spBPQueueSize(source) < maxSize)
	{
		SPListElement e1;
		ASSERT_TRUE(spBPQueueIsFull(source) == false); // check that not full in the process
		e1 = spListElementCreate(1, 1.0);
		spBPQueueEnqueue(source, e1);
		spListElementDestroy(e1);
	}
	ASSERT_TRUE(spBPQueueIsFull(source) == true);

	// free memory
	spBPQueueDestroy(source);
	return true;
}


static bool bpqEnqueueTest() {
	SPBPQueue source, source2;
	SPListElement e1, e2, e3, e4, peek, peekLast;

	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueEnqueue(NULL, NULL)); // check edge case

	CREATE_4_ELEMENTS(); // e1, e2, e3, e4

	source = quickQ(3, e2, e1, e4);
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, e3)); // check that enqueue succeeded
	ASSERT_TRUE(4 == spBPQueueSize(source));

	// check that enqueue inserts in order
	peek = spBPQueuePeek(source);
	peekLast = spBPQueuePeekLast(source);
	ASSERT_TRUE(spListElementCompare(e1, peek)==0);
	ASSERT_TRUE(spListElementCompare(e4, peekLast)==0);

	source2 = spBPQueueCreate(maxSize);

	// insert 2*maxSize elements from lowest to highest value and check that min and max are correct
	for (int i = 0 ; i < maxSize ; i++)
	{
		SPListElement e;
		e = spListElementCreate(i, (double)i);
		ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source2, e));
		spListElementDestroy(e);
	}
	for (int i = maxSize ; i < 2*maxSize ; i++)
	{
		SPListElement e;
		e = spListElementCreate(i, (double)i);
		ASSERT_TRUE(SP_BPQUEUE_FULL == spBPQueueEnqueue(source2, e)); // check full when inserting more then maxSize elements
		spListElementDestroy(e);
	}
	ASSERT_TRUE(spBPQueueMinValue(source2) == 0.0);

	// check that all elements with value too high are not in the queue
	ASSERT_TRUE((int)spBPQueueMaxValue(source2) == maxSize-1);
	spBPQueueClear(source2);

	// insert 2*maxSize elements from highest to lowest value and check that min and max are correct and same as before
	for (int i = 2*maxSize-1 ; i >=0 ; i--)
	{
		SPListElement e;
		e = spListElementCreate(i, (double)i);
		spBPQueueEnqueue(source2, e);
		spListElementDestroy(e);
	}

	ASSERT_TRUE(spBPQueueMinValue(source2) == 0.0);
	ASSERT_TRUE((int)spBPQueueMaxValue(source2) == maxSize-1);

	// free memory
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	spListElementDestroy(peek);
	spListElementDestroy(peekLast);
	DESTROY_4_ELEMENTS(); // e1, e2, e3, e4
	return true;
}

static bool bpqDequeueTest() {
	SPBPQueue source;

	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueDequeue(NULL)); // check edge case

	source = spBPQueueCreate(maxSize);
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueDequeue(source)); // check edge case

	// insert maxSize elements, then remove them
	for (int i = 0 ; i < maxSize ; i++)
	{
		SPListElement e;
		e = spListElementCreate(i, (double)i);
		spBPQueueEnqueue(source, e);
		spListElementDestroy(e);
	}
	for (int i = 0 ; i < maxSize ; i++)
	{
		ASSERT_TRUE((int)spBPQueueMinValue(source) == i); // check that the minimum was removed on the last dequeue
		ASSERT_TRUE(spBPQueueDequeue(source) == SP_BPQUEUE_SUCCESS); // check that dequeue succeeded
	}

	// free memory
	spBPQueueDestroy(source);
	return true;
}


static bool bpqClearTest() {
	SPBPQueue source, source2;
	SPListElement e1, e2, e3, e4;

	CREATE_4_ELEMENTS(); // e1, e2, e3, e4

	source = quickQ(4, e1, e2, e3, e4);
	spBPQueueClear(source);
	ASSERT_TRUE(0 == spBPQueueSize(source)); // check that size is 0 after clearing


	source2 = spBPQueueCreate(maxSize);
	spBPQueueClear(source2);
	ASSERT_TRUE(0 == spBPQueueSize(source)); // check that size is 0 after clearing

	// free memory
	spBPQueueDestroy(source);
	spBPQueueDestroy(source2);
	DESTROY_4_ELEMENTS(); // e1, e2, e3, e4
	return true;
}

static bool bpqDestroyTest() {
	spBPQueueDestroy(NULL); // check no error when destroying null
	return true;
}



int main() {
	RUN_TEST(bpqCreateTest);
	RUN_TEST(bpqCopyTest);

	RUN_TEST(bpqEnqueueTest);
	RUN_TEST(bpqDequeueTest);

	RUN_TEST(bpqGetSizeTest);
	RUN_TEST(bpqGetMaxSizeTest);
	RUN_TEST(bpqPeekTest);
	RUN_TEST(bpqPeekLastTest);
	RUN_TEST(bpqMinValueTest);
	RUN_TEST(bpqMaxValueTest);
	RUN_TEST(bpqEmptyTest);
	RUN_TEST(bpqFullTest);

	RUN_TEST(bpqClearTest);
	RUN_TEST(bpqDestroyTest);

	return 0;
}
