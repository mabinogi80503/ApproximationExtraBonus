#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

void readData(char* fileName);
void create2DArray(int** &array, int size);
int findBestCutEdgeNumber(int *type);
int randomAlgorithm();
int greedyAlgorithm();
void myDelete();

const int RANDOM_MAX_TIME = 100;
int** gMatrix = NULL;
int gNodeNumber = 0;

int main(int args, char* argv[]) {
	char* fileName[] = {
		"RandomGraph_prob_0.2_vertex_1000",
		"RandomGraph_prob_0.3_vertex_1000",
		"RandomGraph_prob_0.4_vertex_1000",
		"RandomGraph_prob_0.5_vertex_1000",
		"RandomGraph_prob_0.6_vertex_1000",
		"Generative_vertex_1000_edge_3",
		"Generative_vertex_1000_edge_4",
		"Generative_vertex_1000_edge_5",
		"Generative_vertex_1000_edge_6",
		"Generative_vertex_1000_edge_10" };

	for( int i = 0; i < 10; ++i ) {
		readData(fileName[i]);

		int r = 0, g = 0;
		cout << "FileName: " << fileName[i] << endl;
		cout << "Random Best\tGreedy Best" << endl;
		r = randomAlgorithm();
		g = greedyAlgorithm();
		cout << r << "\t" << g << endl;
		cout << "------------------------" << endl;
		cout << (r > g ? "Random Win!" : "Greedy Win!") << endl;
		cout << endl;

		myDelete();
	}
	return 0;
}

int greedyAlgorithm() {
	srand((unsigned)time(NULL));
	int bestCutEdgeCount = -1;

	int* seq = new int[gNodeNumber];
	for( int i = 0; i < gNodeNumber; ++i ) seq[i] = i;

	for( int i = 0; i < gNodeNumber; ++i ) {
		int n = rand() % gNodeNumber;
		int t = seq[i];
		seq[i] = seq[n];
		seq[n] = t;
	}

	int* type = new int[gNodeNumber];
	for( int i = 0; i < gNodeNumber; ++i ) {
		type[i] = -1;
	}

	for( int i = 0; i < gNodeNumber; ++i ) {
		int typeA = 0, typeB = 0;
		for( int j = 0; j < gNodeNumber; ++j ) {
			if( gMatrix[seq[i]][j] ) {
				if( type[j] == 0 ) typeA++;
				else if( type[j] == 1 ) typeB++;
			}
		}

		if( typeA == 0 && typeB == 0 ) {
			type[seq[i]] = rand() % 2;
		} else type[seq[i]] = typeA >= typeB ? 1 : 0;
	}

	bestCutEdgeCount = findBestCutEdgeNumber(type);
	return bestCutEdgeCount;
}

int randomAlgorithm() {
	srand((unsigned)time(NULL));
	int bestCutEdgeCount = -1;

	for( int tc = 0; tc < RANDOM_MAX_TIME; ++tc ) {
		int* type = new int[gNodeNumber];
		for( int i = 0; i < gNodeNumber; ++i ) {
			type[i] = rand() % 2;
		}

		bestCutEdgeCount = findBestCutEdgeNumber(type);
		delete type;
	}

	return bestCutEdgeCount;
}

int findBestCutEdgeNumber(int *type) {
	int bestCutEdgeCount = -1;
	int cutEdgeCount = 0;
	for( int j = 0; j < gNodeNumber; ++j ) {
		for( int k = j; k < gNodeNumber; ++k ) {
			if( gMatrix[j][k] && type[j] != type[k] ) cutEdgeCount++;
		}
		if( cutEdgeCount > bestCutEdgeCount ) bestCutEdgeCount = cutEdgeCount;
	}
	return bestCutEdgeCount;
}

void readData(char* fileName) {
	filebuf buf;
	buf.open(fileName, ios::in);

	istream reader(&buf);
	reader >> gNodeNumber;

	create2DArray(gMatrix, gNodeNumber);

	for( int i = 0; i < gNodeNumber; ++i ) {
		for( int j = 0; j < gNodeNumber; ++j ) {
			reader >> gMatrix[i][j];
		}
	}
	
	buf.close();
}

void create2DArray(int** &array, int size) {
	array = new int*[size];
	int* offsetSpace = new int[size*size];

	for( int i = 0; i < size; ++i ) {
		gMatrix[i] = offsetSpace;
		offsetSpace += size;
	}

	memset(gMatrix[0], 0, size * size *sizeof(int));
}

void myDelete() {
	delete gMatrix[0];
	delete gMatrix;
}
