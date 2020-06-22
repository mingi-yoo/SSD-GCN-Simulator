/*******************************************************************
* SSD GCN Project
********************************************************************/

#ifndef __DATACONTROLLER_H__
#define __DATACONTROLLER_H__

#include <iostream>
#include <string>
#include "Distributer.h"
#include "Common.h"

using namespace std;

class DataController {
public:
	vector<float> *ifvalue; // ifvalue data for buffer control
	vector<uint64_t> *adjcolindex; // adjcolindex data for buffer control
	vector<uint64_t> *adjrowindex; // adjrowindex data for buffer control
	Index *index; //data index each of accelerator
	uint64_t *x_size;
	uint64_t *a_col_size;
	uint64_t *a_row_size;
	DataController(Distributer *distributer);
	~DataController();
	uint64_t ReturnSize(uint64_t id, Type iswhat)
	vector<float> ReturnIFM(uint64_t id); // give Ifvalue to buffer
	vector<uint64_t> ReturnAdjColIndex(uint64_t id); // give adjcolindex to buffer
	vector<uint64_t> ReturnAdjRowIndex(uint64_t id); // give adjrowindex to buffer
	bool ReadEnd(uint64_t id, Type iswhat);
	void Reset();
};

#endif