/*******************************************************************
* SSD GCN Project
********************************************************************/

#ifndef __REQUESTCONTROLLER_H__
#define __REQUESTCONTROLLER_H__

#include <iostream>
#include <string>
#include <cmath>
#include "DRAMInterface.h"
#include "BufferInterface.h"
#include "Common.h"

using namespace std;

class RequestController {
public :
	RequestController(Info *info,
					  DRAMInterface *dram, 
					  BufferInterface *buffer);
	~RequestController();
	void Request(ReqFlag &rf, MACFlag &mf);
private:
	uint64_t id;
	uint64_t x_addr;
	uint64_t a_col_addr;
	uint64_t a_row_addr;
	uint64_t w_addr;
	uint64_t x_h;
	uint64_t w_fold;
	DRAMInterface *dram;
	BufferInterface *buffer;

	// for request counting
	uint64_t total_x_req, cur_x_req;
	uint64_t total_a_col_req, cur_a_col_req;
	uint64_t total_a_row_req, cur_a_row_req;

	// flag for A req
	bool col_req;

	// other
	uint64_t present_w_fold;
	uint64_t remain_col;

	// Request function
	void DataRequest(Type iswhat);
};

#endif