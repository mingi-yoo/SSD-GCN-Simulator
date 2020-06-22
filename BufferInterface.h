/*******************************************************************
* SSD GCN Project
********************************************************************/

#ifndef __BUFFERINTERFACE_H__
#define __BUFFERINTERFACE_H__

#include <iostream>
#include <string>
#include "DataController.h"
#include "Common.h"

using namespace std;

struct Buffersize {
	uint64_t axbuffersize, cur_ax_space;
	uint64_t wbuffersize, cur_w_space;
	uint64_t obuffersize, cur_o_space;
};

struct BufferIndex {
	vector<float>::iterator valindex;
	vector<uint64_t>::iterator colindex;
	vector<uint64_t>::iterator rowindex;
}

class BufferInterface {
public:
	bool ax_is_full;
	bool w_is_full;
	bool o_is_full;
	uint64_t x_size;
	uint64_t a_col_size;
	uint64_t a_row_size;
	BufferInterface(uint64_t id, 
					uint64_t axbuffersize,
					uint64_t wbuffersize,
					uint64_t obuffersize, 
					DataController *data);
	~BufferInterface();
	void FillBuffer(uint64_t address, Type iswhat);
	bool IsFilled(Type iswhat, bool ismac);
	bool MACIsFilled(Type iswhat);
	float ReadXFromBuffer(bool ismac);
	uint64_t ReadAFromBuffer(bool iscol, bool ismac);
	bool ReadEnd(Type iswhat);
private:
	DataController *data;
	uint64_t id;
	Buffersize buffersize;
	vector<float> ifvalue;
	vector<uint64_t> adjrowindex;
	vector<uint64_t> adjcolindex;
	BufferIndex reqindex, macindex;
};

#endif
