/*******************************************************************
* SSD GCN Project
********************************************************************/

#include "Accelerator.h"

using namespace std;

Accelerator::Accelerator(Info *info,
						 DRAMInterface *dram, 
						 BufferInterface *buffer) {
	this->id = info->id;
	this->accdimension = info->accdimension;
	req = new RequestController(id, info, dram, buffer);
	mac = new MACController(id, info, dram, buffer);

	// set request flag
	rf.x_over = false;
	rf.x_empty = true;
	rf.a_col_over = false;
	rf.a_col_empty = true;
	rf.a_row_over = false;
	rf.a_row_empty = true;
	rf.isA = false;

	// set mac flag
	mf.x_empty = true;
	mf.a_col_empty = true;
	mf.a_row_empty = true;
	mf.isA = false;
	mf.over = false;
}

Accelerator::~Accelerator() {
	delete req;
	delete mac;
}

void Accelerator::Run() {
	Request();
	MAC();
}

void Accelerator::Request() {
	req->Request(rf, mf);
}

void Accelerator::MAC() {
	mac->MAC(rf, mf);
}