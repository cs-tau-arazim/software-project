#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
#include "SPList.h"

/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
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
 * TODO Complete documentation
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * TODO Complete documentation
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from target SPBPQueue.
 * @param SPBPQueue Target queue to remove all element from
 * @return clears Queue elements. if source is null, does nothing.
 */
void spBPQueueClear(SPBPQueue source);

/**
 * TODO Complete documentation
 */
int spBPQueueSize(SPBPQueue source);



/**
 * TODO Complete documentation
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
 * SP_BPQUEUE_FULL if SPQueue is full and we had to push something out TODO maybe chage to only edge case
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as queue or element
 * SP_BPQUEUE_OUT_OF_MEMORY if an allocation failed
 * SP_BPQUEUE_SUCCESS the element has been inserted successfully
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * TODO Complete documentation
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * TODO Complete documentation
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * TODO Complete documentation
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * TODO Complete documentation
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * TODO Complete documentation
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * TODO Complete documentation
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * TODO Complete documentation
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
