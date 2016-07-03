
#include "SPPoint.h"
#include <stdlib.h>
#include <assert.h>



/**
 * SPPoint Summary
 * Encapsulates a point with variable length dimension. The coordinates
 * values are double types, and each point has a non-negative index which
 * represents the image index to which the point belongs.
 *
 * The following functions are supported:
 *
 * spPointCreate        	- Creates a new point
 * spPointCopy				- Create a new copy of a given point
 * spPointDestroy 			- Free all resources associated with a point
 * spPointGetDimension		- A getter of the dimension of a point
 * spPointGetIndex			- A getter of the index of a point
 * spPointGetAxisCoor		- A getter of a given coordinate of the point
 * spPointL2SquaredDistance	- Calculates the L2 squared distance between two points
 *
 */

/** Type for defining the point **/
struct sp_point_t{
	double* data;
	int dim;
	int index;
};

/**
 * SPPoint spPointCreate(double* data, int dim, int index)
 *
 * creates a new point.
 */
SPPoint spPointCreate(double* data, int dim, int index)
{
	double* newData;
	SPPoint p;
	if (dim <= 0 || index <= 0 || data == NULL) // check edge cases
		return NULL;

	newData = (double*)malloc(dim*sizeof(double)); // allocate data

	if (newData == NULL)
		return NULL;

	int i;
	for ( i = 0 ; i < dim ; i++)
	{
		newData[i] = data[i]; // set all array values
	}
	p = (SPPoint)malloc(sizeof(*p)); // allocate point
	if (p == NULL) {
		free(newData);	
		return NULL;
	}
	p->data = newData; // set values
	p->dim = dim;
	p->index = index;

	return p;
}

/**
 * SPPoint spPointCopy(SPPoint source)
 *
 * copies given point source.
 */
SPPoint spPointCopy(SPPoint source)
{
	SPPoint p;
	assert(source != NULL); // check edge case

	p = (SPPoint) malloc(sizeof(*p));
	// check for malloc failure
	if (p == NULL)
		return NULL;

	p->data = (double*) malloc((source->dim)*sizeof(double)); // allocate array
	p->dim = source->dim;
	p->index = source->index;

	// check for malloc failure
	if (p->data == NULL) {
		free(p);	
		return NULL;
	}
	int i;
	for (i = 0 ; i < source->dim ; i++)
	{
		p->data[i] = source->data[i]; // copy array values
	}

	return p;

}

/**
 * void spPointDestroy(SPPoint point)
 *
 * destroys given point from memory
 */
void spPointDestroy(SPPoint point)
{
	if (point == NULL) // check edge case
		return;

	free(point->data);
	free(point);
}

/**
 * int spPointGetDimension(SPPoint point)
 *
 * return point's dimensions.
 */
int spPointGetDimension(SPPoint point)
{
	assert(point != NULL); // assert edge case
	return point->dim;
}

/**
 * A getter for the index of the point
 */
int spPointGetIndex(SPPoint point)
{
	assert(point != NULL);
	return point->index;
}

/**
 * A getter for specific coordinate value
 *
 * The value of the given coordinate (p_axis will be returned)
 */
double spPointGetAxisCoor(SPPoint point, int axis)
{
	assert(point!=NULL); // assert edge case
	assert(axis < point->dim);
	return point->data[axis];
}

/**
 * Calculates the L2-squared distance between p and q.
 * The L2-squared distance is defined as:
 * (p_1 - q_1)^2 + (p_2 - q_1)^2 + ... + (p_dim - q_dim)^2
 */
double spPointL2SquaredDistance(SPPoint p, SPPoint q)
{
	double sum;
	assert(p != NULL && q != NULL && p->dim == q->dim);

	sum = 0;
	// calculate sum of squares.
	int i;
	for(i = 0; i < p->dim; i++) {
		sum += (p->data[i] - q->data[i]) * (p->data[i] - q->data[i]);
	}
	return sum;
}


