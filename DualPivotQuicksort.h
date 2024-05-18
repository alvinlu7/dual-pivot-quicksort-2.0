#pragma once
#include <array>
#include <stdexcept>
#include <random> 
#include <stdio.h>
#include <chrono>
#include <iostream>

//set up random number generator
std::mt19937 generator(123);
std::uniform_real_distribution<double> dis(0.0, 1.0);
int depth = 0;

template <class T>
void swap(T a[], int i, int j) {
	T temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}

template <class T>
void dualPivotQuicksort(T a[], int left, int right, int medians) {
	depth++;
	int len = right - left;

	if (len < 27) {//use insertion sort on small arrays
		for (int i = left + 1; i <= right; i++) {
			for (int j = i; j > left && a[j] < a[j - 1]; j--) {
				swap(a, j, j - 1);
			}
		}
		return;
	}
	//auto begin = std::chrono::high_resolution_clock::now();
	//make an array of potential medians and their indexes
	int fourth = (right - left) / 4;
	int A_index = left + 1;
	int B_index = left + fourth;
	int C_index = left + fourth * 2;
	int D_index = right - fourth;
	int E_index = right - 1;
	T A = a[A_index];
	T B = a[B_index];
	T C = a[C_index];
	T D = a[D_index];
	T E = a[E_index];
	
	T temp;
	int temp_index;
	int m1;
	int m2;

	//sorting network
	if (A > B) { //swap a with b
		temp = A;
		A = B;
		B = temp;
		temp_index = A_index;
		A_index = B_index;
		B_index = temp_index;
	}
	if (C > D) {//swap c with d
		temp = C;
		C = D;
		D = temp;
		temp_index = C_index;
		C_index = D_index;
		D_index = temp_index;
	}
	if (A > C) { // swap a/c and b/d
		temp = A;
		A = C;
		C = temp;
		temp_index = A_index;
		A_index = C_index;
		C_index = temp_index;

		temp = B;
		B = D;
		D = temp;
		temp_index = B_index;
		B_index = D_index;
		D_index = temp_index;
	}
	
	if (E > C) {
		if (E > D) { // A C D E
			if (B > D) {
				if (B > E)
				{
					m1 = C_index;
					m2 = E_index;
				}
				else
				{
					m1 = C_index;
					m2 = B_index;
				}
			}
			else {
				if (B < C) {
					m1 = B_index;
					m2 = D_index;
				}
				else {
					m1 = C_index;
					m2 = D_index;
				}
			}
		}
		else {//A C E D
			if (B > E) {
				if (B > D) {
					m1 = C_index;
					m2 = D_index;
				}
				else {
					m1 = C_index;
					m2 = B_index;
				}
			}
			else {
				if (B < C) {
					m1 = B_index;
					m2 = E_index;
				}
				else {
					m1 = C_index;
					m2 = E_index;
				}
			}
		}
	}
	else {
		if (E < A) { // E A C D
			if (B > C) {
				if (B > D) {
					m1 = A_index;
					m2 = D_index;
				}
				else {
					m1 = A_index;
					m2 = B_index;
				}
			}
			else {
				m1 = A_index;
				m2 = C_index;
			}
		}
		else { //A E C D
			if (B > C) {
				if (B > D) {
					m1 = E_index;
					m2 = D_index;
				}
				else {
					m1 = E_index;
					m2 = B_index;
				}
			}
			else {
				if (B < E) {
					m1 = B_index;
					m2 = C_index;
				}
				else {
					m1 = E_index;
					m2 = C_index;
				}
			}
		}
	}

	//auto end = std::chrono::high_resolution_clock::now();
	//std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ns" << std::endl;
	
	//cin.get();

	int div = 3;
	
	//"medians"

	swap(a, m1, left);
	swap(a, m2, right);

	//pivots
	T pivot1 = a[left];
	T pivot2 = a[right];

	//pointers
	int less = left + 1;
	int great = right - 1;

	//sorting
	for (int k = less; k <= great; k++) {
		if (a[k] < pivot1) {
			swap(a, k, less++);
		}
		else if (a[k] > pivot2) {
			while (k < great && a[great] > pivot2) {
				great--;
			}
			swap(a, k, great--);
			if (a[k] < pivot1) {
				swap(a, k, less++);
			}
		}
	}
	//swaps
	int dist = great - less;
	if (dist < 13) {
		div++;
	}
	swap(a, less - 1, left);
	swap(a, great + 1, right);

	//subarrays
	dualPivotQuicksort(a, left, less - 2, medians);
	dualPivotQuicksort(a, great + 2, right, medians);

	//equal elements
	if (dist > len - 13 && pivot1 != pivot2) {
		for (int k = less; k <= great; k++) {
			if (a[k] == pivot1) {
				swap(a, k, less++);
			}
			else if (a[k] == pivot2) {
				swap(a, k, great--);

				if (a[k] == pivot1) {
					swap(a, k, less++);
				}
			}
		}
	}

	//subarray
	if (pivot1 < pivot2) {
		dualPivotQuicksort(a, less, great, medians);
	}
}

void rangeCheck(int size, int fromIndex, int toIndex) {
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
void qsort(T a[], int fromIndex, int toIndex, int size, int medians) {
	rangeCheck(size, fromIndex, toIndex - 1);
	dualPivotQuicksort(a, fromIndex, toIndex - 1, medians);
}

template <class T>
int qsort(T a[], int size, int medians) {
	depth = 0;
	qsort(a, 0, size, size, medians);
	return depth;
}





