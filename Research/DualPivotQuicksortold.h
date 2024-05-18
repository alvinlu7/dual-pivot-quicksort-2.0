#pragma once
#include <array>
#include <stdexcept>
#include <stdio.h>
#include <chrono>
#include <iostream>

int depth_old = 0;

template <class T>
void swap_old(T a[], int i, int j) {
	T temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

template <class T>
void dualPivotQuicksort_old(T a[], int left, int right, int div) {
	depth_old++;
	int len = right - left;

	if (len < 27) {//use insertion sort on small arrays
		for (int i = left + 1; i <= right; i++) {
			for (int j = i; j > left && a[j] < a[j - 1]; j--) {
				swap_old(a, j, j - 1);
			}
		}
		return;
	}

	//auto begin = std::chrono::high_resolution_clock::now();
	int third = len / div;


	//"medians"
	int m1 = left + third;
	int m2 = right - third;

	if (m1 <= left) {
		m1 = left + 1;
	}
	if (m2 >= right) {
		m2 = right - 1;
	}
	if (a[m1] < a[m2]) {
		swap_old(a, m1, left);
		swap_old(a, m2, right);
	}
	else {
		swap_old(a, m1, right);
		swap_old(a, m2, left);
	}
	//pivots
	T pivot1 = a[left];
	T pivot2 = a[right];

	//pointers
	int less = left + 1;
	int great = right - 1;

	//auto end = std::chrono::high_resolution_clock::now();
	//std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ns" << std::endl;
	//std::cin.get();
	//sorting
	for (int k = less; k <= great; k++) {
		if (a[k] < pivot1) {
			swap_old(a, k, less++);
		}
		else if (a[k] > pivot2) {
			while (k < great && a[great] > pivot2) {
				great--;
			}
			swap_old(a, k, great--);
			if (a[k] < pivot1) {
				swap_old(a, k, less++);
			}
		}
	}
	//swaps
	int dist = great - less;
	if (dist < 13) {
		div++;
	}
	swap_old(a, less - 1, left);
	swap_old(a, great + 1, right);

	//subarrays
	dualPivotQuicksort_old(a, left, less - 2, div);
	dualPivotQuicksort_old(a, great + 2, right, div);

	//equal elements
	if (dist > len - 13 && pivot1 != pivot2) {
		for (int k = less; k <= great; k++) {
			if (a[k] == pivot1) {
				swap_old(a, k, less++);
			}
			else if (a[k] == pivot2) {
				swap_old(a, k, great--);

				if (a[k] == pivot1) {
					swap_old(a, k, less++);
				}
			}
		}
	}

	//subarray
	if (pivot1 < pivot2) {
		dualPivotQuicksort_old(a, less, great, div);
	}
}

void rangeCheck_old(int size, int fromIndex, int toIndex) {
	if (fromIndex > toIndex) {
		throw std::invalid_argument("fromIndex > toIndex");
	}
	if (fromIndex < 0) {
		throw std::range_error("" + fromIndex);
	}
	if (toIndex > size) {
		throw std::range_error("" + toIndex);
	}
}

template <class T>
void qsort_old(T a[], int fromIndex, int toIndex, int size) {
	rangeCheck_old(size, fromIndex, toIndex - 1);
	dualPivotQuicksort_old(a, fromIndex, toIndex - 1, 3);
}

template <class T>
int qsort_old(T a[], int size) {
	depth_old = 0;
	qsort_old(a, 0, size, size);
	return depth_old;
}





