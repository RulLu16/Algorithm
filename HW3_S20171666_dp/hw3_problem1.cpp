#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME "input.txt"
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y)) 
int is_match(int i, int j,char* x, char* y,int s, int f);
int dp(char* x, char* y, int s, int f, int p, int** gap, int** dp_table,int xsize, int ysize);
//void make_gap(char* x, char* y,char* new_x, char* new_y, int** gap, int xsize, int ysize,int* gap_xsize, int* gap_ysize);

int main() {
	FILE* fp = fopen(FILE_NAME, "r");
	FILE* out = fopen("output.txt", "w");
	int s, p, f;
	char name[100];
	int xsize, ysize;
	char* x;
	char* y; // string x,y
	int i, j;
	char* gap_x; // string that have gaps
	char* gap_y;
	int** gap; // 1=i-1,j-1 / 2=i-1,j / 3=i,j-1
	int gap_xsize = 0;
	int gap_ysize = 0;
	int** dp_table;
	char temp;
	int x_point;
	int y_point;
	int newx_point = 0;
	int newy_point = 0; // for making gap_x,gap_y
	i = 0;
	while (1) {
		fscanf(fp, "%c",&temp);
		if (temp == '\n')
			break;
		name[i] = temp;
		i++;
	}
	name[i] = '\0';
	fscanf(fp,"%d %d %d", &s, &f, &p);
	FILE* in = fopen(name, "rb");
	fread(&xsize, sizeof(int), 1, in);
	fread(&ysize, sizeof(int), 1, in);
	x = (char*)malloc(sizeof(char)*xsize);
	y = (char*)malloc(sizeof(char)*ysize);
	gap_x = (char*)malloc(sizeof(char)*xsize*ysize);
	gap_y = (char*)malloc(sizeof(char)*xsize*ysize);
	dp_table = (int**)malloc(sizeof(int*)*(xsize+1));
	gap = (int**)malloc(sizeof(int*)*(xsize+1));
	for (i = 0; i <= xsize; i++) {
		dp_table[i] = (int*)malloc(sizeof(int)*(ysize+1));
		gap[i] = (int*)malloc(sizeof(int)*(ysize+1));
	}
	x[0] = '0';
	y[0] = '0';
	for (i = 1; i <= xsize; i++) 
		fread(&x[i], sizeof(char), 1, in);
	for (i = 1; i <= ysize; i++) 
		fread(&y[i], sizeof(char), 1, in);
	dp(x, y, s, f, p, gap, dp_table, xsize, ysize);// execute dp algorithm
	fprintf(out, "%d\n", dp_table[xsize][ysize]);
	//make_gap(x, y, gap_x, gap_y, gap, xsize, ysize, &gap_xsize, &gap_ysize);
	i = xsize;
	j = ysize;
	x_point = xsize;
	y_point = ysize;
	while (1) {
		if (i == 0 && j == 0)
			break;
		switch (gap[i][j]) {
		case 1:
			gap_x[newx_point] = x[x_point];
			gap_y[newy_point] = y[y_point];
			i--;
			x_point--;
			newx_point++;
			j--;
			y_point--;
			newy_point++;
			break;
		case 2:
			gap_x[newx_point] = x[x_point];
			gap_y[newy_point] = '_';
			i--;
			x_point--;
			newx_point++;
			newy_point++;
			gap_ysize++;
			break;
		case 3:
			gap_x[newx_point] = '_';
			gap_y[newy_point] = y[y_point];
			j--;
			y_point--;
			newx_point++;
			newy_point++;
			gap_xsize++;
			break;
		}
	}
	fprintf(out, "%d\n", gap_xsize + xsize);
	fprintf(out, "%d\n", gap_xsize);
	for (i = newx_point-1; i >=0; i--) 
		if (gap_x[i] == '_')
			fprintf(out, "%d\n", newx_point - i);
	fprintf(out, "%d\n", gap_ysize);
	for (i = newy_point-1; i >=0; i--)
		if (gap_y[i] == '_')
			fprintf(out, "%d\n", newy_point - i);
}

int dp(char* x, char* y, int s, int f, int p, int** gap, int** dp_table,int xsize, int ysize) {
	int i, j;
	int match, ingap_x, ingap_y;
	int maxitem = 0;
	dp_table[0][0] = 0;
	for (i = 1; i <= xsize; i++) {
		dp_table[i][0] = (0 - p)*i;
		gap[i][0] = 2;
	}
	for (i = 1; i <= ysize; i++) {
		dp_table[0][i] = (0 - p)*i;
		gap[0][i] = 3;
	}
	for (i = 1; i <= xsize; i++) {
		for (j = 1; j <= ysize; j++) {
			match = dp_table[i - 1][j - 1] + is_match(i, j, x, y, s, f);
			ingap_x = dp_table[i - 1][j] - p;
			ingap_y = dp_table[i][j - 1] - p;
			maxitem = MAX(match, MAX(ingap_x, ingap_y));
			dp_table[i][j] = maxitem;
			if (maxitem == match)
				gap[i][j] = 1;
			else if (maxitem == ingap_x)
				gap[i][j] = 2;
			else
				gap[i][j] = 3;
		}
	}
	return dp_table[xsize][ysize];
}

int is_match(int i, int j,char* x, char* y,int s, int f) {
	if (x[i] == y[j])
		return s;
	else
		return 0-f;
}