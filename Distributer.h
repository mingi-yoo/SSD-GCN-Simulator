/*******************************************************************
* SSD GCN Project
********************************************************************/

#ifndef __DISTRIBUTER_H__
#define __DISTRIBUTER_H__

#include "IniParser.h"
#include "DataReader.h"
#include "Common.h"
#include <iostream>
#include <cmath>

using namespace std;

class Distributer {
public:
	Distributer(IniParser *ini, DataReader *data);
	~Distributer();
	uint64_t w_h, w_w, x_h, x_w, a_h, a_w; // matrix information
	uint64_t w_fold;
	uint64_t axbuffersize, wbuffersize, obuffersize;
	vector<float> *ifvalue; //data for individual accelerator
	vector<uint64_t> *adjcolindex;
	vector<uint64_t> *adjrowindex;
	uint64_t acc_num;
	DataReader *data;
	uint64_t w_fold;
	uint64_t v_fold, v_fold_last;
	uint64_t *x_addr;
	uint64_t *a_col_addr;
	uint64_t *a_row_addr;
	uint64_t *o_addr;
	uint64_t *row_start;
	void Print();
private:
	void Distribute();
};

#endif
