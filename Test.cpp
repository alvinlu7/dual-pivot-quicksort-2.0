#pragma once
#include <iostream>
#include <string>
#include "DualPivotQuicksort.h"
#include "DualPivotQuicksortold.h"
#include <math.h>
#include <chrono>
#include <random> 
#include <fstream>
#include <string>
#include <climits>

using namespace std;

class testObj {
	public:
		int key;
		long** value;
};
bool operator== (const testObj &t1, const testObj &t2) {
	return t1.key == t2.key;
};
bool operator!= (const testObj &t1, const testObj &t2) {
	return t1.key != t2.key;
};
bool operator> (const testObj &t1, const testObj &t2) {
	return t1.key > t2.key;
};
bool operator< (const testObj &t1, const testObj &t2) {
	return t1.key < t2.key;
};

int main() {

	ofstream data;
	data.open("array_var_size.csv");
	data << "algorithm,array_size,depth,runtime" << endl;

	int max_int_size = 1000000000;
	int medians = 5;
	int matrix_size = 10;

	for (int size = 2; size <= pow(2, 27); size *= 2) {
		cout << "Running array sizes " << size << endl;
		for (int iteration = 0; iteration < 32; iteration++) {
			cout << 100 * ((float)iteration) / 32 << "% complete" << endl;
			//Setup random number generator
			std::mt19937 generator(rand() % 1000);
			std::uniform_real_distribution<double> dis(0.0, 1.0);
			double randomRealBetweenZeroAndOne = dis(generator);
			//Initialize arrays and vectors
			testObj* test = new testObj[size];
			testObj* test2 = new testObj[size];

			for (int i = 0; i < size; i++) {
				test[i].key = dis(generator)* max_int_size;
				test2[i].key = test[i].key;
				long** matrix = new long*[matrix_size];
				long** matrix2 = new long*[matrix_size];
				for (int i = 0; i < matrix_size; i++) {
					matrix[i] = new long[matrix_size];
					matrix2[i] = new long[matrix_size];
				}
				for (int i = 0; i < matrix_size; i++) {
					for (int j = 0; j < size; j++) {
						matrix[i][j] = dis(generator) * LLONG_MAX;
						matrix2[i][j] = matrix[i][j];
					}
				}
				test[i].value = matrix;
				test2[i].value = matrix2;
			}
			//std::vector<int> v(test, test + size);

			//Test my quicksort algorithm
			auto begin = std::chrono::high_resolution_clock::now();
			int depth = qsort(test, size, medians);
			auto end = std::chrono::high_resolution_clock::now();
			//std::cout << "My quicksort: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ms" << std::endl;
			long runtime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
			data << "mine," << size << "," << depth << "," << runtime << endl;

			//Test Yaroslavskiy's algorithm
			begin = std::chrono::high_resolution_clock::now();
			depth = qsort_old(test2, size);
			end = std::chrono::high_resolution_clock::now();
			//std::cout << "Yaroslavskiy quicksort: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "ms" << std::endl;
			runtime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
			data << "yaroslavskiy," << size << "," << depth << "," << runtime << endl;

			//Test C++ std library's algorithm
			/*begin = std::chrono::high_resolution_clock::now();
			std::sort(v.begin(), v.end());
			end = std::chrono::high_resolution_clock::now();
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
			*/

			for (int k = 0; k < size; ++k) {
				for (int i = 0; i < matrix_size; ++i) {
					delete[] test[k].value[i];
				}
				delete[] test[k].value;
			}
			delete test;
			
			for (int k = 0; k < size; ++k) {
				for (int i = 0; i < matrix_size; ++i) {
					delete[] test2[k].value[i];
				}
				delete[] test2[k].value;
			}
			delete test2;
		}
	}
	data.close();
	cin.get();
}