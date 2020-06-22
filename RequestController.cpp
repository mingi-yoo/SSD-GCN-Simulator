/*******************************************************************
* SSD GCN Project
********************************************************************/

#include "RequestController.h"

using namespace std;

RequestController::RequestController(Info *info,
								  	 DRAMInterface *dram, 
								  	 BufferInterface *buffer){
	id = info->id;
	x_addr = info->x_addr;
	a_col_addr = info->a_col_addr;
	a_row_addr = info->a_row_addr;
	x_h = info->x_h;
	w_fold = info->w_fold;
	this->dram = dram;
	this->buffer= buffer;

	total_x_req = ceil((float)buffer->x_size/MAX_READ_INT);
	total_a_col_req = ceil((float)buffer->a_col_size/MAX_READ_INT);
	total_a_row_req = ceil((float)buffer->a_row_size/MAX_READ_INT);
	cur_x_req = 0;
	cur_a_col_req = 0;
	cur_a_row_req = 0;

	col_req = false;

	present_col = 0;
	present_w_fold = 0;
	remain_col = 0;
}

RequestController::~RequestController() {}

void RequestController::Request(ReqFlag &rf, MACFlag &mf) {
	if (!rf.isA)
	{
		if (rf.x_empty && !rf.x_over && !buffer->ax_is_full)
		{
			// There is no X data in buffer
			// And X is not over
			if (rf.x_empty && buffer->IsFilled(X))
			{
				rf.x_empty = false;
				mf.x_empty = false;
			}
			DataRequest(rf, X);
		}
		else if (!rf.x_empty && !buffer->w_is_full)
		{
			// For MAC, Request weight of X.
			float x = buffer->ReadXFromBuffer(false);
			w_addr = WEIGHT_START + (present_col * w_fold + present_w_fold) * MAX_READ_BYTE;
			if (!buffer->IsFilled(X, false))
				rf.x_empty = true;
			present_col++;
			if (present_col == x_h)
				present_col = 0;
			DataRequest(rf, WEIGHT); 
		}
	}
	else
	{
		if (rf.a_row_empty && !rf.a_row_over && !buffer->ax_is_full && !col_req)
		{
			DataRequest(rf, A_ROW);
			col_req = true;
		}
		else if (rf.a_col_empty && !rf.a_col_over && !buffer->ax_is_full)
		{
			if (rf.a_row_empty && buffer->IsFilled(A_ROW, false))
			{
				rf.a_row_empty = false;
				mf.a_row_empty = false;
			}
			if (rf.a_col_empty && buffer->IsFilled(A_COL, false))
			{
				rf.a_col_empty = false;
				mf.a_row_empty = false;
			}
			DataRequest(rf. A_COL);
		}
		else if (!rf.a_row_empty && !rf.a_col_empty && !buffer->w_is_full)
		{
			if (remain_col == 0)
				remain_col = buffer->ReadAFromBuffer(false, false);
			if (remain_col != 0)
			{
				present_col = buffer->ReadAFromBuffer(true, false);
				remain_col--;
				w_addr = OUTPUT_START + (present_col * w_fold + present_w_fold) * MAX_READ_BYTE;
				if (!buffer->IsFilled(A_ROW, false))
					rf.a_row_empty = true;
				if (!buffer->IsFilled(A_COL, false))
					rf.a_col_empty = true;
				DataRequest(rf, WEIGHT);
			}
		}
	}
}

void RequestController::DataRequest(ReqFlag &rf, Type iswhat) {
	uint64_t address;

	switch (iswhat)
	{
		case X:
			address = x_addr;
			x_addr += MAX_READ_BYTE;
			cur_x_req++;
			if (cur_x_req >= total_x_req)
				rf.x_over = true;
			dram->DRAMRequest(id, address, false);
			buffer->UpdateBufferSpace(iswhat);
			cout<<"Request X. Address: "<<hex<<address<<endl;
			break;
		case A_COL:
			address = a_col_addr;
			a_col_addr += MAX_READ_BYTE;
			cur_a_col_req++;
			if (cur_a_col_req >= total_a_col_req)
				rf.a_col_over = true;
			dram->DRAMRequest(id, address, false);
			buffer->UpdateBufferSpace(iswhat);
			cout<<"Request A_COL. Address: "<<hex<<address<<endl;
			break;
		case A_ROW:
			address = a_row_addr;
			a_row_addr += MAX_READ_BYTE;
			cur_a_row_req++;
			if (cur_a_row_req >= total_a_row_req)
				rf.a_row_over = true;
			dram->DRAMRequest(id, address, false);
			buffer->UpdateBufferSpace(iswhat);
			cout<<"Request A_COL. Address: "<<hex<<address<<endl;
			break;
		case WEIGHT:
			dram->DRAMRequest(id, address, false);
			buffer->UpdateBufferSpace(iswhat);
			buffer->WeightRequest(address);
			cout<<"Request WEIGHT. Address: "<<hex<<address<<endl;
			break;
	}
}