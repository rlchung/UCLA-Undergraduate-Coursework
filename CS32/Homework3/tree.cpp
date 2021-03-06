// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a2     the function must return
//	10 20 40			1
//	10 40 30			2
//	20 10 40			0
//	50 40 30			3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
	if (n1 < n2)//If n1 is smaller than n2 (what we want to find), return false
		return 0;
	if (n2 <= 0)//If n2 is empty, return true because n1 contains null set
		return 1;
	if (n1 <= 0)//If n1 is empty, return false
		return 0;

	int restCount = countIncludes(a1 + 1, n1 - 1, a2, n2);

	if (a1[0] == a2[0])//If front element from both arrays are equal, check to see if the rest of them are equal
	{
		restCount += countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
		return restCount;
	}
	else//If front elements don't equal, exclude first element from a1 and check all of a2 in a1
		return restCount;

}

// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > splitter come before all the other elements,
// and all the elements whose value is < splitter come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= splitter, or n if there is no such element, and firstLess is
// set to the index of the first element that is < splitter, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > splitter
//   * for firstNotGreater <= i < firstLess, a[i] == splitter
//   * for firstLess <= i < n, a[i] < splitter
// All the elements > splitter end up in no particular order.
// All the elements < splitter end up in no particular order.
void split(double a[], int n, double splitter,
	int& firstNotGreater, int& firstLess)
{
	if (n < 0)
		n = 0;

	// It will always be the case that just before evaluating the loop
	// condition:
	//  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
	//  Every element earlier than position firstNotGreater is > splitter
	//  Every element from position firstNotGreater to firstUnknown-1 is
	//    == splitter
	//  Every element from firstUnknown to firstLess-1 is not known yet
	//  Every element at position firstLess or later is < splitter

	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < splitter)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > splitter)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
	if (n <= 1)
		return;

	int firstHalf, secondHalf;

	split(a, n, a[n / 2], firstHalf, secondHalf);

	//order elements from greatest to least in the first half/before a[n/2]
	order(a, firstHalf);

	//order elements from greatest to least in the second half/after a[n/2]
	order(a + secondHalf, n - secondHalf) //increment a to start at secondHalf and subtract from n to get proper size of new array
}