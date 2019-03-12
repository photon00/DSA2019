/* ******************************************************
 *  This program copy the original value to another array
 *  and perform quickselect at each slide.
 * ******************************************************/

#include <cstdio>
#include <algorithm>

int quickselect(int k, int* start, int num, int p); 
void copy(int*, int*, int);
void qcheck(int* arr, int num, int pivot_idx){
	int tmp = arr[pivot_idx];
	for (int i=0; i<pivot_idx; ++i){
		if (arr[i] > tmp)
			printf("Fuck left side goes wrong at %d, piv at %d\n", i, pivot_idx);
	}
	for (int i=pivot_idx+1; i<num; ++i){
		if (arr[i] < tmp)
			printf("Fuck right side goes wrong at %d, piv at %d\n", i, pivot_idx);
	}
}

int n, m, k, *array;
int main(){
	scanf("%d%d%d", &n, &m, &k);
	array = new int[n];

	for (int i=0; i<n; ++i){
		scanf("%d", &array[i]);
	}
	int *clone = new int[m];
	for (int i=0; i<n-m; ++i){
		copy(array+i, clone, m);
		printf("%d\n", quickselect(k, clone, m, 0));
	}
	copy(array+n-m, clone, m);
	printf("%d\n", quickselect(k, clone, m, 1));
	delete [] clone;
	delete [] array;
}

int quickselect(int k, int* start, int num, int p){ 
	int i=0, r=num-1;
	int pivot = start[r];
	for (int j=0; j<r; j++){
		if (start[j] < pivot)
			std::swap(start[i++], start[j]);
	}
	std::swap(start[i], start[r]);
	//qcheck(start, num, i);

	if (i == k-1) return start[k-1];
	if (i > k-1) return quickselect(k, start, i, p);
	return quickselect(k-i-1, &start[i+1], num-i-1, p);
}

void copy(int* array, int* clone, int num){
	for (int i=0; i<num; ++i)
		clone[i] = array[i];
}

