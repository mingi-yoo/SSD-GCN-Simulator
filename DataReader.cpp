
/*******************************************************************
* SSD GCN Project
********************************************************************/

#include "DataReader.h"

using namespace std;

uint64_t mac1_count;
uint64_t mac2_count;

DataReader::DataReader(string path) { ReadData(path); }

DataReader::~DataReader() {}

vector<float> DataReader::GetFloatVal(string line, char delimiter)  {
	vector<float> internal;
	stringstream ss(line);
	string temp;

	while (getline(ss, temp, delimiter)) {
		if (temp == "\n")
			break;
		internal.push_back(stof(temp));
	}
	cout<<"OK"<<endl;

	return internal;
}

vector<uint64_t> DataReader::GetUint64Val(string line, char delimiter) {
	vector<uint64_t> internal;
	stringstream ss(line);
	string temp;

	while (getline(ss, temp, delimiter)) {
		if (temp == "\n")
			break;
		internal.push_back(stoull(temp));
	}

	return internal;
}

bool DataReader::ReadData(string path) {
	ifstream openFile(path);
	if (openFile.is_open()) {
		//get weight_h
		string line;
		getline(openFile, line);
		weight_h = stoi(line);
		x_w = weight_h;
		getline(openFile, line);
		//get weight_w(is equal to x_h)
		weight_w = stoi(line);
		getline(openFile, line);
		x_h = stoi(line);
		a_h = x_h;
		//get x matrix
		getline(openFile, line);
		ifvalue = GetFloatVal(line, ' ');
		mac1_count = x_h;
		//get adjacency csr
		getline(openFile, line);
		adjcolindex = GetUint64Val(line, ' ');
		getline(openFile, line);
		adjrowindex = GetUint64Val(line, ' ');
		mac2_count = adjrowindex.size() - 1;
		openFile.close();
		return true;
	}
	else {
		throw invalid_argument("Cannot open datafile.");
	}
}
