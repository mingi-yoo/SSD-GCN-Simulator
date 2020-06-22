/*******************************************************************
* SSD GCN Project
********************************************************************/

#include "MACController.h"

using namespace std;

MACController::MACController(Info *info, 
							 DRAMInterface *dram, 
							 BufferInterface *buffer) {
	id = info->id;
	accdimension = info->accdimension;
	o_addr = info->o_addr;
	row_start = info->row_start;
	x_h = info->x_h;
	w_fold = info->w_fold;
	this->dram = dram;
	this->buffer= buffer;

	present_col = 0;
	present_row = row_start;
	mac_col = 0;
	mac_row = row_start;
	remain_col = 0;
	pipe = new Pipeline[info->mac_cycle_time];
	for (int i = 0; i < info->mac_cycle_time; i++)
	{
		pipe[i].active = false;
		pipe[i].step = 0;
		pipe[i].x_ready = false;
		pipe[i].w_ready = false;
		pipe[i].a_row_ready = false;
		pipe[i].a_col_ready = false;
	}
	num_of_pipe = info->mac_cycle_time;
	pipefilled = 0;
	pipe_start = 0;
	present_w_fold = 0;

	total_row = buffer->x_size/x_h;

}

MACController::~MACController() {
	delete value;
}

void MACController::MAC(ReqFlag &rf, MACFlag &mf) {
	if (!mf.isA)
	{
		if (!mf.x_empty && !(pipefilled == num_of_pipe) && !pipe[pipe_start].x_ready)
		{
			// if x is in buffer, check weight address for mac 
			pipe[pipe_start].value = buffer->ReadXFromBuffer(true);
			pipe[pipe_start].x_ready = true;
			pipe[pipe_start].rowindex = present_row;
			pipe[pipe_start].colindex = present_col;
			w_addr = WEIGHT_START + (present_col * w_fold + present_w_fold) * MAX_READ_BYTE;
			// if weight is exist, turn on the flag for mac
			if (buffer->WeightSearch(w_addr))
			{
				pipe[pipe_start].w_addr = w_addr;
				pipe[pipe_start].w_ready = true;
				pipe[pipe_start].active = true;
				pipe_start++;
				pipefilled++;
				if (pipe_start == num_of_pipe)
					pipe_start = 0;
			}
			present_col++;
			if (present_col == x_h)
			{
				present_col = 0;
				present_row++;
			}
			if (!buffer->IsFilled(X, true))
				mf.x_empty = true;
		}
		else if (!pipe[pipe_start].w_ready)
		{
			// x is ready, but weight is not, then check again
			if (buffer->WeightSearch(w_addr))
			{
				pipe[pipe_start].w_addr = w_addr;
				pipe[pipe_start].w_ready = true;
				pipe[pipe_start].active = true;
				pipe_start++;
				pipefilled++;
				if (pipe_start == num_of_pipe)
					pipe_start = 0;
			}
		}
		// MAC
		for (int i = 0; i < num_of_pipe; i++)
		{
			if (pipe[i].active)
			{
				cout<<"MAC1 Running... w_fold: "<<dec<<present_w_fold<<
				", Row: "<<pipe[i].rowindex<<
				", Col: "<<pipe[i].colindex<<
				", Val: "<<pipe[i].value<<endl;
				pipe[i].step++;
				if (pipe[i].step == num_of_pipe)
				{
					pipe[i].x_ready = false;
					pipe[i].w_ready = false;
					pipe[i].active = false;
					mac_col++;
					pipefilled--;
					if (mac_col == x_h-1)
					{
						cout<<"Row: "<<dec<<mac_row<<" is Complete."<<endl;
						address = OUTPUT_START + (mac_row * w_fold + present_w_fold) * MAX_READ_BYTE;
						dram->DRAMRequest(id, address, true);
						mac_row++;
						mac_col = 0;
						if (mac_row == total_row)
							mf.over = true;
					}
				}
			}
		}
	}
	else
	{
		if (!mf.a_row_empty && remain_col == 0)
		{
			remain_col = buffer->ReadAFromBuffer(false, true);
			if (!buffer->IsFilled(A_ROW,true))
				mf.a_row_empty = true;
		}
		if (!mf.a_col_empty && !(pipefilled == num_of_pipe) && !pipe[pipe_start].a_col_ready && remain_col != 0)
		{
			pipe[pipe_start].colindex = buffer->ReadAFromBuffer(true, true);
			pipe[pipe_start].rowindex = present_row;
		}
	}
}