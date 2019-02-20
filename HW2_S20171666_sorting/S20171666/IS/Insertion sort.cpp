#include<cstdio>
#include<cstdlib>
#include<Windows.h>

#define CHECK_TIME_START QueryPerformanceFrequency((_LARGE_INTEGER*) &freq); QueryPerformanceCounter((_LARGE_INTEGER*)&start);
#define CHECK_TIME_END(a) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a = (float)((float) (end - start)/freq);

#define SWAP(a, b) {\
	swap_temp = a; a = b; b = swap_temp;\
}

#define FILE_NAME "input_array_2_15.bin"
__int64 start, freq, end;
float resultTime;
int swap_temp;

int Validation(int* data, int left, int right);
int Insertion_Sort(int* data, int left, int right);

int main()
{
	int N,i;
	int temp;
	int* ins;
	FILE* fa = fopen(FILE_NAME, "rb");
	fread(&N, sizeof(int), 1, fa);
	ins = (int*)(malloc(N * sizeof(int)));
	for (i = 0; i < N; i++)
	{
		fread(&temp, sizeof(int), 1, fa);
		ins[i] = temp;
	}

	if (Insertion_Sort(ins, 0, N) == 1)
	{
		printf("Sorting Completed\n");
		/*for (int i = 0; i < N; i++) {
			printf("\t[%d]'s data is %d\n", i, ins[i]);
		}*/
	}
	else
		printf("Sorting Error!\n");

	printf("Total Time is [ %f (ms)]\n", resultTime*1000.0f);
	fclose(fa);
	return 0;
	
}

int Insertion_Sort(int* data, int left, int right)
{
	int temp;
	int i, j;
	CHECK_TIME_START
	for (i = left; i < right; i++)
	{
		temp = data[i];
		for (j = i; j >0; j--)
		{
			if (temp < data[j - 1])
				data[j] = data[j - 1];
			else
				break;
		}
		data[j] = temp;
	}
	CHECK_TIME_END(resultTime)
	return Validation(data, left, right);
}

int Validation(int* data, int left, int right) {
	for (int i = left; i < right - 1; i++) {
		if (data[i] > data[i + 1]) return -1;
	}
	return 1;
}