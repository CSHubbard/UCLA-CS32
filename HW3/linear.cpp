
//bool somePredicate(double x)
//{
//	return x < 0;
//}

// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
	if (n == 0)
		return false;
	if (n == 1)
		return somePredicate(a[0]);
	else
	{
		if (!somePredicate(a[n - 1]))
			return false;
		else
			return allTrue(a, n - 1);
	}
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		if (somePredicate(a[0]))
			return 0;
		else
			return 1;
	else
	{
		if (!somePredicate(a[n - 1]))
			return countFalse(a, n - 1)+1;
		else
			return countFalse(a, n - 1);
	}
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
	if (n == 0)
		return -1;
	if (n == 1)
		if (somePredicate(a[0]))
			return -1;
		else
			return n-1;
	else 
	{
		int temp = firstFalse(a, n - 1);
		if (temp != -1)
			return temp;
		if (!somePredicate(a[n - 1]))
			return n-1;
		else
			return firstFalse(a, n - 1);
	}
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
	if (n == 0)
		return -1;
	else if (n == 1)
		return 0;
	else
	{
		if (a[n - 1] < a[indexOfMin(a, n - 1)])
			return n - 1;
		else
			return indexOfMin(a, n-1);
	}
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 > n1)
		return 0;
	if (n1 == 0 && n2 == 0)
		return true;
	else if (n1 == 0)
		return false;
	
	else if (n1 == 1)
	{
		if (a1[0] == a2[n2 - 1])
			return true;
		else
			return false;
	}

	if (n2 == 0)
		return true;

	else if (n2 == 1)
	{
		if (a1[n1 - 1] == a2[0])
			return true;
		else
		return	includes(a1, n1 - 1, a2, n2);
	}

	if (a1[n1-1] == a2[n2-1])
	{
	return	includes(a1, n1, a2, n2 - 1);
	}
	else
	{
	return	includes(a1, n1 - 1, a2, n2);
	}
}


//int main()
//{
//	double a[7] = {5,4,5,2,3,5,1};
//	double b[3] = {4,5,1};
//	double c[5] = { 1,4,-1,3,4 };
//
//	double d[3] = { 3,2,4 };
//	double r[1] = { 2 };
//
//	allTrue(c, 5);
//	countFalse(c, 5);
//	firstFalse(c, 5);
//	indexOfMin(a, 7);
//	includes(d, 3, r, 1);
//	includes(a,7,b,3);
//
//}