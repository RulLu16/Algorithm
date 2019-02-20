#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#define CHECK_TIME_START QueryPerformanceFrequency((_LARGE_INTEGER*) &freq); QueryPerformanceCounter((_LARGE_INTEGER*)&start);
#define CHECK_TIME_END(a) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a = (float)((float) (end - start)/freq);

#define FILE_NAME "commands.txt"
#define SWAP(a, b) {\
	swap_temp = a; a = b; b = swap_temp;\
}
__int64 start, freq, end;
float resultTime;

typedef struct {
	int from;
	int to;
	int weight;
}graph;
graph swap_temp;
typedef struct {
	long long m_weight;
	long long m_component;
	int rank;
	int parent;
}ver;

void sort(graph* edge, int left, int right);
void insertion(graph* edge, int left, int right);
int find(ver* vertex, int v);
void combine(ver* vertex, int from, int to, int weight);
void mst(graph* edge, ver* vertex, int e);
void print_mst(ver* vertex, int v, FILE* out);

int main() {
	FILE* fp = fopen(FILE_NAME,"r");
	FILE* input;
	FILE* output;
	char name[200];
	char temp;
	int i = 0;
	int check;
	graph* edge;
	ver* vertex;
	int vertex_num;
	int edge_num;
	int max_weight;

	while (1) {
		fscanf(fp, "%c", &temp);
		if (temp == '\n')
			break;
		name[i] = temp;
		i++;
	}
	name[i] = '\0';
	//printf("%s\n", name);
	input = fopen(name, "r");
	i = 0;
	while (1) {
		check = fscanf(fp, "%c", &temp);
		if (temp == '\n' || check == EOF)
			break;
		name[i] = temp;
		i++;
	}
	name[i] = '\0';
	//printf("%s\n", name);
	output = fopen(name, "w");

	fscanf(input, "%d", &vertex_num);
	fscanf(input, "%d", &edge_num);
	fscanf(input, "%d", &max_weight);
	vertex = (ver*)malloc(sizeof(ver)*vertex_num);
	for (i = 0; i < vertex_num; i++) {
		vertex[i].parent = i;
		vertex[i].m_weight = 0;
		vertex[i].m_component = 1;
		vertex[i].rank = 0;
	}
	edge = (graph*)malloc(sizeof(graph)*edge_num);
	for (i = 0; i < edge_num; i++) 
		fscanf(input, "%d %d %d", &edge[i].from, &edge[i].to, &edge[i].weight);
	CHECK_TIME_START
	sort(edge, 0, edge_num);
	insertion(edge, 0, edge_num);
	CHECK_TIME_END(resultTime)
	printf("Total Time is [ %f (ms)]\n", resultTime/*1000.0f*/);
	mst(edge, vertex, edge_num);
	print_mst(vertex, vertex_num, output);
	CHECK_TIME_END(resultTime)
	printf("Total Time is [ %f (ms)]\n", resultTime/*1000.0f*/);

	fclose(fp);
	fclose(input);
	fclose(output);
}

void sort(graph* edge, int left, int right) {
	int temp;
	int i, j;
	int mid = (left + right) / 2;
	int p = left;
	if (right - left < 32)
		return;
	else if (left<right)
	{
		if (edge[left].weight > edge[mid].weight)
			SWAP(edge[left], edge[mid]);
		if (edge[left].weight > edge[right-1].weight)
			SWAP(edge[left], edge[right-1]);
		if (edge[mid].weight > edge[right-1].weight)
			SWAP(edge[mid], edge[right-1]);
		SWAP(edge[mid], edge[left+1]);
		j = left + 1;
		for (i = left + 2; i < right; i++)
		{
			if (edge[i].weight < edge[left].weight)
			{
				j++;
				SWAP(edge[j], edge[i]);
			}
		}
		SWAP(edge[left], edge[j]);

		p = j;
		if (p < mid) {
			sort(edge, left, p);
			sort(edge, p + 1, right);
		}
		else
		{
			sort(edge, p + 1, right);
			sort(edge, left, p);
		}

	}
}

void insertion(graph* edge, int left, int right) {
	graph temp;
	int i, j;
	for (i = left; i < right; i++)
	{
		temp = edge[i];
		for (j = i; j >0; j--)
		{
			if (temp.weight < edge[j-1].weight)
				edge[j] = edge[j - 1];
			else
				break;
		}
		edge[j] = temp;
	}
}

int find(ver* vertex, int v) {
	if (vertex[v].parent == v)
		return v;
	else
		return find(vertex, vertex[v].parent);
}

void combine(ver* vertex, int from, int to, int weight) {
	int u, v;
	u = find(vertex, from);
	v = find(vertex, to);
	if (u == v)
		return;
	else {
		if (vertex[u].rank == vertex[v].rank) {
			vertex[v].parent = u;
			vertex[u].rank++;
			vertex[u].m_component += vertex[v].m_component;
			vertex[u].m_weight = vertex[u].m_weight + vertex[v].m_weight + weight;
		}
		else if (vertex[u].rank < vertex[v].rank) {
			vertex[u].parent = v;
			vertex[v].m_component += vertex[u].m_component;
			vertex[v].m_weight = vertex[v].m_weight + vertex[u].m_weight + weight;
		}
		else {
			vertex[v].parent = u;
			vertex[u].m_component += vertex[v].m_component;
			vertex[u].m_weight = vertex[u].m_weight + vertex[v].m_weight + weight;
		}
	}
}

void mst(graph* edge, ver* vertex, int e) {
	int i;
	for (i = 0; i < e; i++) {
		if (edge[i].from == edge[i].to)
			continue;
		combine(vertex, edge[i].from, edge[i].to, edge[i].weight);
	}
}

void print_mst(ver* vertex, int v, FILE* out) {
	int i;
	int num = 0;
	for (i = 0; i < v; i++) {
		if (vertex[i].parent == i)
			num++;
	}
	fprintf(out,"%d\n", num);
	for (i = 0; i < v; i++) {
		if (vertex[i].parent == i)
			fprintf(out,"%lld %lld\n", vertex[i].m_component, vertex[i].m_weight);
	}
}
