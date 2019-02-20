#include <stdio.h>
#include <stdlib.h>
#define FILE_NAME "input.txt"
int dp(int pack_size, int* pack, int rsize, int lsize, int* right, int* left);
int match(int card, int pack);
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y)) 

int main() {
	FILE* fp = fopen(FILE_NAME, "r");
	FILE* out = fopen("output.txt", "w");
	int left_size, right_size;
	int i, j, temp, result;
	int* left; // card on left hand
	int* right; // card on right hand
	int* card_one;
	int* card_two;
	fscanf(fp, "%d", &left_size);
	left = (int*)malloc(sizeof(int)*left_size);
	for (i = 0; i < left_size; i++)
		fscanf(fp, "%d", &left[i]);
	fscanf(fp, "%d", &right_size);
	right = (int*)malloc(sizeof(int)*right_size);
	for (i = 0; i < right_size; i++)
		fscanf(fp, "%d", &right[i]);
	fscanf(fp, "%d", &temp);
	card_one = (int*)malloc(sizeof(int)*temp);
	for (i = 0; i < temp; i++)
		fscanf(fp, "%d", &card_one[i]);
	result = dp(temp, card_one, right_size, left_size, right, left);
	fprintf(out, "%d", result);

	fscanf(fp, "%d", &temp);
	card_two = (int*)malloc(sizeof(int)*temp);
	for (i = 0; i < temp; i++)
		fscanf(fp, "%d", &card_two[i]);

	result = dp(temp, card_two, right_size, left_size, right, left);
	fprintf(out, "%d", result);
}

int dp(int pack_size, int* pack, int rsize, int lsize, int* right, int* left) {
	int i;
	int rpoint = 0;
	int lpoint = 0;
	int r, l, max;
	int* dp_table = (int*)malloc(sizeof(int)*(rsize + lsize + 1));
	dp_table[0] = 0;
	for (i = 1; i <= rsize + lsize; i++) {
		r = dp_table[i - 1] + match(right[rpoint], pack[i - 1]);
		l = dp_table[i - 1] + match(left[lpoint], pack[i - 1]);
		max = MAX(r, l);
		dp_table[i] = max;
		if (max == r)
			rpoint++;
		else
			lpoint++;
	}
	if (dp_table[rsize + lsize] == rsize + lsize)
		return 1;
	return 0;
	
}

int match(int card, int pack) {
	if (card == pack)
		return 1;
	return 0;
}