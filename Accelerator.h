/*******************************************************************
* SSD GCN Project
********************************************************************/

#ifndef __ACCELERATOR_H__
#define __ACCELERATOR_H__

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "DRAMInterface.h"
#include "BufferInterface.h"
#include "RequestController.h"
#include "MACController.h"
#include "Common.h"

using namespace std;

class Accelerator {
public :
	Accelerator(uint64_t id, 
				uint64_t accdimension,
				Info *info,
				DRAMInterface *dram, 
				BufferInterface *buffer);
	~Accelerator();
	void Run();
	void Request();
	void MAC();
private:
	uint64_t id;
	uint64_t accdimension;
	RequestController *req;
	MACController *mac;
	Info *info;
	ReqFlag rf;
	MACFlag mf;
};

#endif