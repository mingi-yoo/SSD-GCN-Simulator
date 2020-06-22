/*******************************************************************
* SSD GCN Project
********************************************************************/

#ifndef __DATAREADER_H__
#define __DATAREADER_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <stdexcept>
#include "Common.h"

using namespace std;

class DataReader {
public:
	DataReader(string path);
	~DataReader();
	uint64_t weight_h;
	uint64_t weight_w;
	uint64_t x_h;
	uint64_t x_w;
	uint64_t a_h;
	vector<float> ifvalue;
	vector<uint64_t> adjcolindex;
	vector<uint64_t> adjrowindex;

private:
	vector<float> GetFloatVal(string line, char delimiter);
	vector<uint64_t> GetUint64Val(string line, char delimiter);
	bool ReadData(string path);
};

#endif