#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>

// TODO check edge cases- to we need to confirm that asssertion works?

// First make sure that createPoint handles all edge cases
bool pointCreateTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 0; // should error
	int index = 1;
	SPPoint p, q;
	p = spPointCreate(data, dim, index);
	ASSERT_TRUE(p == NULL);

	dim = 5;
	index = -1;
	q = spPointCreate(data, dim, index);
	ASSERT_TRUE(q == NULL);

	// also nothing to free because everything is null.
	return true;
}

//Check if copy Works
bool pointCopyTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint p,q, s;
	p = spPointCreate(data, dim, index);
	q = spPointCopy(p); // copy point twice
	s = spPointCopy(q);

	ASSERT_TRUE(
			spPointGetIndex(p) == spPointGetIndex(q)
					&& spPointGetIndex(s) == spPointGetIndex(q));
	ASSERT_TRUE(
			spPointGetDimension(p) == spPointGetDimension(q)
					&& spPointGetDimension(s) == spPointGetDimension(q));
	for (int i = 0; i < spPointGetDimension(p); i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == spPointGetAxisCoor(q, i));
		ASSERT_TRUE(spPointGetAxisCoor(s, i) == spPointGetAxisCoor(q, i));
	}
	spPointDestroy(p);
	spPointDestroy(q);
	spPointDestroy(s);

	return true;
}


bool pointGetDimensionTest() {
	double data1[2] = { 1.0, 1.0 };
	double data2[3] = { 1.0, 0.0, 3.0 };
	int dim1 = 2;
	int dim2 = 3;
	int index1 = 1;
	int index2 = 1;

	SPPoint p, q;
	p = spPointCreate((double *) data1, dim1, index1);
	q = spPointCreate((double *) data2, dim2, index2);

	ASSERT_TRUE(spPointGetDimension(p) == 2);
	ASSERT_TRUE(spPointGetDimension(q) == 3);

	spPointDestroy(p);
	spPointDestroy(q);

	return true;
}

bool pointGetIndexTest() {
	double data1[2] = { 1.0, 1.0 };
	double data2[3] = { 1.0, 0.0, 3.0 };
	int dim1 = 2;
	int dim2 = 3;
	int index1 = 1;
	int index2 = 10;

	SPPoint p, q;
	p = spPointCreate((double *) data1, dim1, index1);
	q = spPointCreate((double *) data2, dim2, index2);

	ASSERT_TRUE(spPointGetIndex(p) == 1);
	ASSERT_TRUE(spPointGetIndex(q) == 10);

	spPointDestroy(p);
	spPointDestroy(q);

	return true;
}

// spPointGetAxisCoor Test
bool pointGetAxisCoorTest() {
	double data1[2] = { 1.0, 1.0 };
	double data2[3] = { 1.0, 0.0, 3.0 };
	int dim1 = 2;
	int dim2 = 3;
	int index1 = 1;
	int index2 = 10;

	SPPoint p, q;
	p = spPointCreate((double *) data1, dim1, index1);
	q = spPointCreate((double *) data2, dim2, index2);

	ASSERT_TRUE(spPointGetAxisCoor(p,0) == 1.0);
	ASSERT_TRUE(spPointGetAxisCoor(q,2) == 3.0);

	spPointDestroy(p);
	spPointDestroy(q);

	return true;
}


// Check L2Distance function
bool pointL2DistanceTest() {
	double data1[3] = { 1.0, 1.0, 0.0};
	double data2[3] = { 1.0, 0.0, -50.34};
	int dim1 = 3;
	int dim2 = 3;
	int index1 = 1;
	int index2 = 1;

	SPPoint p, q;
	p = spPointCreate((double *) data1, dim1, index1);
	q = spPointCreate((double *) data2, dim2, index2);

	ASSERT_TRUE(spPointL2SquaredDistance(p, p) == 0.0);
	ASSERT_TRUE(spPointL2SquaredDistance(q, q) == 0.0);
	ASSERT_FALSE(spPointL2SquaredDistance(p, q) == 0.0);

	// compare precise values
	double d;
	int i;

	d = spPointL2SquaredDistance(p, q);
	i = d * 10000;
	ASSERT_TRUE(i == 25351156);

	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}
/**
int main() {
	RUN_TEST(pointCreateTest);
	RUN_TEST(pointCopyTest);
	RUN_TEST(pointGetDimensionTest);
	RUN_TEST(pointGetIndexTest);
	RUN_TEST(pointGetAxisCoorTest);
	RUN_TEST(pointL2DistanceTest);
	return 0;
}*/
