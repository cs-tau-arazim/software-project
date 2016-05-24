#include "unit_test_util.h"
#include "../SPListElement.h"
#include "../SPList.h"
#include "../SPBPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

//static SPBPQueue quickQ(int size, ...);

static SPBPQueue quickQ(int size, ...) {
	va_list items;
	SPBPQueue source = spBPQueueCreate(size);
	va_start(items, size);
	for (int i = 0; i < size; i++) {
		spBPQueueEnqueue(source, va_arg(items, SPListElement));
	}
	va_end(items);
	return source;
}

static bool testQCreate() {
	SPBPQueue source = spBPQueueCreate(10);
	ASSERT_TRUE(source != NULL);
	spBPQueueDestroy(source);
	return true;
}

static bool testQCopy() {
	ASSERT_TRUE(spBPQueueCopy(NULL) == NULL);
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
	ASSERT_TRUE(spListElementCompare(e1, spBPQueuePeek(copy2)) == 0);
	spBPQueueDequeue(copy2);
	ASSERT_TRUE(spListElementCompare(e2, spBPQueuePeek(copy2)) == 0);
	spBPQueueDequeue(copy2);
	ASSERT_TRUE(spListElementCompare(e3, spBPQueuePeek(copy2)) == 0);
	spBPQueueDequeue(copy2);
	ASSERT_TRUE(spListElementCompare(e4, spBPQueuePeek(copy2)) == 0);
	spBPQueueDequeue(copy2);
	ASSERT_TRUE(spBPQueuePeek(copy2) == NULL);
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

/*
static bool testListGetSize() {
	SPList list = quickList(0);
	ASSERT_TRUE(0 == spListGetSize(list));
	SPListElement e1 = spListElementCreate(1, 1.0);
	spListInsertFirst(list, e1);
	ASSERT_TRUE(1 == spListGetSize(list));
	spListElementSetIndex(e1, 2);
	spListElementSetValue(e1, 2.0);
	spListInsertFirst(list, e1);
	ASSERT_TRUE(2 == spListGetSize(list));
	spListGetFirst(list);
	spListRemoveCurrent(list);
	ASSERT_TRUE(1 == spListGetSize(list));
	spListDestroy(list);
	spListElementDestroy(e1);
	return true;
}

static bool testListGetFirst() {
	SPList list = spListCreate();
	ASSERT_TRUE(spListGetFirst(list) == NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	SPListElement first = spListGetFirst(list2);
	ASSERT_TRUE(spListElementCompare(e1, first) == 0);
	ASSERT_TRUE(
			spListElementCompare(first, spListGetFirst(list2)) == 0
					&& spListGetFirst(list2) == first);
	spListDestroy(list);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListGetNext() {
	ASSERT_TRUE(spListGetNext(NULL) == NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListInsertFirst() {
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertFirst(NULL, NULL));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(3, e1, e2, e3);
	ASSERT_TRUE(SP_LIST_SUCCESS == spListInsertFirst(list2, e4));
	ASSERT_TRUE(4 == spListGetSize(list2));
	ASSERT_TRUE(spListElementCompare(e4, spListGetFirst(list2))==0);
	ASSERT_TRUE(spListElementCompare(e1, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e2, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(list2))==0);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListInsertLast() {
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertLast(NULL, NULL));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(3, e2, e3, e4);
	ASSERT_TRUE(SP_LIST_SUCCESS == spListInsertLast(list2, e1));
	ASSERT_TRUE(4 == spListGetSize(list2));
	ASSERT_TRUE(spListElementCompare(e2, spListGetFirst(list2))==0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e4, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e1, spListGetNext(list2))==0);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListInsertBeforeCurrent() {
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertBeforeCurrent(NULL, NULL));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(3, e2, e3, e4);
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertBeforeCurrent(list2,NULL));
	ASSERT_TRUE(
			SP_LIST_INVALID_CURRENT == spListInsertBeforeCurrent(list2, e1));
	ASSERT_TRUE(3 == spListGetSize(list2));
	spListGetFirst(list2);
	spListGetNext(list2);
	ASSERT_TRUE(SP_LIST_SUCCESS == spListInsertBeforeCurrent(list2, e1));
	ASSERT_TRUE(4 == spListGetSize(list2));
	ASSERT_TRUE(spListElementCompare(e2, spListGetFirst(list2))==0);
	ASSERT_TRUE(spListElementCompare(e1, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e4, spListGetNext(list2))==0);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListInsertAfterCurrent() {
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertAfterCurrent(NULL, NULL));

	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(3, e2, e3, e4);
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertAfterCurrent(list2,NULL));
	ASSERT_TRUE(SP_LIST_INVALID_CURRENT == spListInsertAfterCurrent(list2, e1));
	spListGetFirst(list2);
	spListGetNext(list2);
	ASSERT_TRUE(SP_LIST_SUCCESS == spListInsertAfterCurrent(list2, e1));
	ASSERT_TRUE(4 == spListGetSize(list2));
	ASSERT_TRUE(spListElementCompare(e2, spListGetFirst(list2))==0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e1, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e4, spListGetNext(list2))==0);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListClear() {
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	ASSERT_TRUE(spListClear(list2) == SP_LIST_SUCCESS);
	ASSERT_TRUE(0 == spListGetSize(list2));
	SPList list = spListCreate();
	spListClear(list);
	ASSERT_TRUE(0 == spListGetSize(list));
	spListDestroy(list);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListDestroy() {
	spListDestroy(NULL);
	return true;
}

*/

int main() {

	RUN_TEST(testQCreate);
	RUN_TEST(testQCopy);

	//RUN_TEST(testListGetSize);
	//RUN_TEST(testListGetFirst);
	//RUN_TEST(testListGetNext);
	//RUN_TEST(testListInsertFirst);
	//RUN_TEST(testListInsertLast);
	//RUN_TEST(testListInsertBeforeCurrent);
	//RUN_TEST(testListInsertAfterCurrent);
	//RUN_TEST(testListClear);
	//RUN_TEST(testListDestroy);

	return 0;
}
