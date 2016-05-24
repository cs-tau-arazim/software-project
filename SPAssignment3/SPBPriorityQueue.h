#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
#include "SPList.h"

/**
 * SP Bounded Priority Queue summary
 *
 * Implements a Bounded Priority Queue type.
 * The elements of the queue are of type SPListElement, please refer
 * to SPListElement.h for usage.
 *
 * The elements are sorted from lowest value to highest.
 * If the queue is at full capacity then adding an element will result
 * in removing the highest value element from the queue.
 *
 * The following functions are available:
 *
 * spBPQueueCreate			- creates an empty queue with a given maximum capacity
 * spBPQueueCopy			- creates a copy of a given queue
 * spBPQueueDestroy			- frees all memory allocation associated with the queue
 * spBPQueueClear			- removes all the elements in the queue
 * spBPQueueSize			- returns the number of elements in the queue
 * spBPQueueGetMaxSize		- returns the maximum capacity of the queue
 * spBPQueueEnqueue			- Inserts a NEW COPY element to the queue
 * spBPQueueDequeue			- removes the element with the lowest value
 * spBPQueuePeek			- returns a NEW COPY of the element with the lowest value
 * spBPQueuePeekLast		– returns a NEW COPY of the element with the highest value
 * spBPQueueMinValue		- returns the minimum value in the queue
 * spBPQueueMaxValue		- returns the maximum value in the queue
 * spBPQueueIsEmpty 		– returns true if the queue is empty
 * spBPQueueIsFull			- returns true if the queue is full
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new BPQueue.
 *
 * This function creates a new empty BPQueue.
 * @assert maxSize > 0
 * @param the maxSize bound for the BPQueue.
 * @return
 * 	NULL - If allocations failed.
 * 	A new BPQueue in case of success.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of target BPQueue.
 *
 * The new copy will contain all the elements from the source BPQueue. maxSize will be the same.
 *
 * @param source The target BPQueue to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A BPQueue containing the same elements as source otherwise.
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * spBPQueueDestroy: Deallocates an existing BPQueue.
 *
 * @param source Target BPQueue to be deallocated. If source is NULL nothing will be
 * done
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from target SPBPQueue.
 * @param SPBPQueue Target queue to remove all element from
 * @return clears Queue elements. if source is null, does nothing.
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the number of elements in a BPQueue.
 *
 * @param source The target BPQueue which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the BPQueue.
 */
int spBPQueueSize(SPBPQueue source);



/**
 * Returns the max size of a BPQueue.
 *
 * @param source The target BPQueue which max size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the nax size of source.
 */
int spBPQueueGetMaxSize(SPBPQueue source);


/**
 * Adds a new element to the BPQueue.
 * a new copy of the element will be inserted at its appropriate (prioritized) place.
 *
 * @param source The BPQueue for which to add an element prioritized.
 * @param element The element to insert. A copy of the element will be
 * inserted.
 * @return
 * SP_BPQUEUE_FULL if SPQueue is full and we had to push something out
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as queue or element
 * SP_BPQUEUE_OUT_OF_MEMORY if an allocation failed
 * SP_BPQUEUE_SUCCESS the element has been inserted successfully
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the lowest value element from the BPQueue.
 *
 * @param source The BPQueue for which to remove the lowest value element.
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as queue or the queue is empty
 * SP_BPQUEUE_SUCCESS the element has been removed successfully
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * returns a NEW COPY of the element with the lowest value
 *
 * @param source The BPQueue for which to return the lowest value element.
 * @return
 * NULL if a NULL pointer was sent or the BPQueue is empty.
 * The lowest value element of the BPQueue otherwise
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * returns a NEW COPY of the element with the highest value
 *
 * @param source The BPQueue for which to return the highest value element.
 * @return
 * NULL if a NULL pointer was sent or the BPQueue is empty.
 * The highest value element of the BPQueue otherwise
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * returns the value of the element with the lowest value
 *
 * @param source The BPQueue for which to return the lowest value.
 * @return
 * -1 if a NULL pointer was sent or the BPQueue is empty.
 * The lowest value of the BPQueue otherwise
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * returns the value of the element with the highest value
 *
 * @param source The BPQueue for which to return the highest value.
 * @return
 * -1 if a NULL pointer was sent or the BPQueue is empty.
 * The highest value of the BPQueue otherwise
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * returns true if the BPQueue is empty
 *
 * @param source The BPQueue for which to check if empty.
 * @return
 * true if a NULL pointer was sent or the BPQueue is empty.
 * false otherwise.
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * returns true if the BPQueue is full
 *
 * @param source The BPQueue for which to check if full.
 * @return
 * true if source != NULL and the BPQueue is full.
 * false otherwise.
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
