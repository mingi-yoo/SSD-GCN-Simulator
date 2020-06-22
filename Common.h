/*******************************************************************
* SSD GCN Project
********************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <queue>
#include <vector>
#include <cassert>

using namespace std;

/*
* global variables
* defined in IniParser.cpp
*/
extern uint64_t MAX_READ_BYTE; 
extern uint64_t MAX_READ_INT; 
extern uint64_t UNIT_INT_BYTE; 

extern uint64_t A_COL_START;  
extern uint64_t A_ROW_START;  
extern uint64_t X_START; 
extern uint64_t WEIGHT_START; 
extern uint64_t OUTPUT_START;
extern uint64_t	OUTPUT2_START;

extern uint64_t mac1_count;
extern uint64_t mac2_count;

enum Type {A_COL, A_ROW, X, WEIGHT, OUTPUT};

struct Index {
	uint64_t valindex;
	uint64_t colindex;
	uint64_t rowindex;
};

struct ReqFlag {
	bool x_over, x_empty;
	bool a_col_over, a_col_empty;
	bool a_row_over, a_row_empty;
	bool isA;
};

struct MACFlag {
	bool x_empty;
	bool a_col_empty;
	bool a_row_empty;
	bool isA;
	bool over;
}

struct Info {
	uint64_t id;
	uint64_t accdimension;
	uint64_t mac_cycle_time;
	uint64_t x_addr;
	uint64_t a_col_addr;
	uint64_t a_row_addr;
	uint64_t o_addr;
	uint64_t row_start;
	uint64_t x_h;
	uint64_t w_fold;
	uint64_t v_fold;
	uint64_t v_fold_last;
}

//for weight buffer
struct WB_Data {
	uint64_t address;
	uint64_t req; // 몇 번 리퀘스트 받았는가?
};

//{Row, Col} or {weight_h, weight_w}
struct Tuple {
	uint64_t tuple[2];
};

struct WeightBuffer {
	uint64_t remain_space;
	vector<WB_Data> active;
	vector<WB_Data> expire;
	vector<WB_Data> request;
};

struct OutputBuffer {
	uint64_t remain_space;
	queue<uint64_t> address; 
};

#endif


//퍼포먼스 카운터