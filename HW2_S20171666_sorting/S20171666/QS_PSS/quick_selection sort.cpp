#include<cstdio>
#include<cstdlib>
#include<Windows.h>
#include <stdio.h>

#define CHECK_TIME_START QueryPerformanceFrequency((_LARGE_INTEGER*) &freq); QueryPerformanceCounter((_LARGE_INTEGER*)&start);
#define CHECK_TIME_END(a) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a = (float)((float) (end - start)/freq);

#define SWAP(a, b) {\
	swap_temp = a; a = b; b = swap_temp;\
}

#define FILE_NAME "input_array_2_10.bin"

#define M 32
__int64 start, freq, end;
float resultTime;
int swap_temp;

int Validation(int* data, int left, int right);
void Quick_Sort_PSS(int* data, int left, int right);

int main()
{
	int N, i;
	int temp;
	int* qui;
	FILE* fa = fopen(FILE_NAME, "rb");
	fread(&N, sizeof(int), 1, fa);
	qui = (int*)(malloc(N * sizeof(int)));
	for (i = 0; i < N; i++)
	{
		fread(&temp, sizeof(int), 1, fa);
		qui[i] = temp;
	}
	CHECK_TIME_START
		Quick_Sort_PSS(qui, 0, N);
	CHECK_TIME_END(resultTime)
		if (Validation(qui, 0, N) == 1)
		{
			printf("Sorting Completed\n");
			/*for (i = 0; i < N; i++) {
				printf("\t[%d]'s data is %d\n", i, qui[i]);
			}*/
		}
		else
			printf("Sorting Error!\n");
	printf("Total Time is [ %f (ms)]\n", resultTime*1000.0f);
	fclose(fa);
	return 0;

}

void Quick_Sort_PSS(int* data, int left, int right)
{
	int min;
	int i, j;
	int p = left;
	if (right - left < M)
	{
		for (i = left; i < right; i++)
		{
			min = i;
			for (j = i; j < right; j++)
			{
				if (data[j] < data[min])
				{
					min = j;
				}
			}
			SWAP(data[i], data[min]);
		}
	}
	else if (left<right)
	{
		j = left;
		for (i = left + 1; i < right; i++)
		{
			if (data[i] < data[left])
			{
				j++;
				SWAP(data[j], data[i]);
			}
		}
		SWAP(data[left], data[j]);

		p = j;
		Quick_Sort_PSS(data, left, p);
		Quick_Sort_PSS(data, p + 1, right);

	}
}

int Validation(int* data, int left, int right) {
	for (int i = left; i < right - 1; i++) {
		if (data[i] > data[i + 1]) return -1;
	}
	return 1;
}