/*******************************************************************
* SSD GCN Project
********************************************************************/

#ifndef __MACCONTROLLER_H__
#define __MACCONTROLLER_H__

#include <iostream>
#include <string>
#include "DRAMInterface.h"
#include "BufferInterface.h"
#include "Common.h"

using namespace std;

struct Pipeline
{
	float value;
	uint64_t rowindex;
	uint64_t colindex;
	uint64_t w_addr;
	uint64_t step;
	bool active;
	bool x_ready;
	bool a_col_ready;
	bool a_row_ready;
	bool w_ready;
}

class MACController {
public :
	MACController(Info *info
				  DRAMInterface *dram, 
				  BufferInterface *buffer);
	~MACController();
	void MAC(ReqFlag &rf, MACFlag &mf);
private:
	uint64_t id;
	uint64_t accdimension;
	uint64_t o_addr;
	uint64_t row_start;
	uint64_t x_h;
	uint64_t w_fold;
	DRAMInterface *dram;
	BufferInterface *buffer;

	// mac state variable
	uint64_t present_col;
	uint64_t present_row;
	uint64_t num_of_pipe;
	uint64_t pipe_start;
	uint64_t pipefilled;
	uint64_t present_w_fold;
	uint64_t w_addr;
	uint64_t mac_col;
	uint64_t mac_row;
	uint64_t total_row; // num_of_row
	uint64_t remain_col;
	Pipeline *pipe;
};

#endif