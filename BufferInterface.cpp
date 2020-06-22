/*******************************************************************
* SSD GCN Project
********************************************************************/

#include "BufferInterface.h"

using namespace std;

BufferInterface::BufferInterface(uint64_t id,
								 uint64_t axbuffersize,
								 uint64_t wbuffersize,
								 uint64_t obuffersize,
								 DataController *data) {
	// set buffer size
	this->id = id;
	buffersize.axbuffersize = id;
	buffersize.wbuffersize = wbuffersize;
	buffersize.obuffersize = obuffersize;
	buffersize.cur_ax_space = 0;
	buffersize.cur_w_space = 0;
	buffersize.cur_o_space = 0; 

	//set index for request
	reqindex.valindex = ifvalue.begin();
	reqindex.colindex = adjcolindex.begin();
	reqindex.rowindex = adjrowindex.begin();

	// set index for mac
	macindex.valindex = ifvalue.begin();
	macindex.colindex = adjcolindex.begin();
	macindex.rowindex = adjrowindex.begin();

	// other
	this->data = data;
	ax_is_full = false;
	w_is_full = false;
	o_is_full = false;
	x_size = data->ReturnSize(id, X);
	a_col_size = data->ReturnSize(id, A_COL);
	a_row_size = data->ReturnSize(id, A_ROW);

}

BufferInterface::~BufferInterface() {}

void BufferInterface::FillBuffer(uint64_t address, Type iswhat) {
	switch (iswhat)
	{
		case X:
			vector<float> ret = data->ReturnIFM(id);
			for (int i = 0; i < ret.size(); i++)
				ifvalue.push_back(ret[i]);
			break;
		case A_COL:
			vector<uint64_t> ret = data->ReturnAdjColIndex(id);
			for (int i = 0; i < ret.size(); i++)
				adjcolindex.push_back(ret[i]);
			break;
		case A_ROW:
			vector<uint64_t> ret = data->ReturnAdjRowIndex(id);
			for (int i = 0; i < ret.size(); i++)
				adjrowindex.push_back(ret[i]);
			break;
	}
}

bool BufferInterface::IsFilled(Type iswhat, bool ismac) {
	bool ret;

	if (ismac)
	{
		switch (iswhat)
		{
			case X:
				ret = (ifvalue.size() > 0);
				break;
			case A_COL:
				ret = (adjcolindex.size() > 0);
				break;
			case A_ROW:
				ret = (adjrowindex.size() > 1);
				break;
		}
	}
	else
	{
		switch (iswhat)
		{
			case X:
				ret = (reqindex.valindex != ifvalue.end());
				break;
			case A_COL:
				ret = (reqindex.colindex != adjcolindex.end());
				break;
			case A_ROW:
				ret = ((++reqindex.rowindex) != adjrowindex.end());
				reqindex.rowindex--;
				break;
		}
	}
	return ret;
}

float BufferInterface::ReadXFromBuffer(bool ismac) {
	float ret;

	if (ismac)
	{
		ret = *(macindex.valindex);
		ifvalue.erase(macindex.valindex);
		reqindex.valindex--;
		buffersize.cur_ax_space -= UNIT_INT_BYTE;
		if (buffersize.axbuffersize - buffersize.cur_ax_space >= MAX_READ_BYTE)
			ax_is_full = false;
	}
	else 
	{
		ret = *(reqindex.valindex);
		reqindex++;
	}

	return ret;
}

uint64_t BufferInterface::ReadAFromBuffer(bool iscol, bool ismac) {
	uint64_t ret;

	if (iscol)
	{
		if (ismac)
		{
			ret = *(macindex.colindex);
			adjcolindex.erase(macindex.colindex);
			reqindex.colindex--;
			buffersize.cur_ax_space -= UNIT_INT_BYTE;
			if (buffersize.axbuffersize - buffersize.cur_ax_space >= MAX_READ_BYTE)
				ax_is_full = false;
		}
		else
		{
			ret = *(reqindex.colindex);
			reqindex++;
		}
	}
	else
	{
		if (ismac)
		{
			ret = *(macindex.rowindex);
			macindex.rowindex++;
			ret = *(macindex.rowindex) - ret;
			macindex.rowindex--;
			adjrowindex.erase(macindex.rowindex);
			reqindex.rowindex--;
		}
		else
		{
			ret = *(reqindex.rowindex);
			reqindex.rowindex++;
			ret = *(reqindex.rowindex) - ret;
		}
	}
	
	return ret;
}

bool BufferInterface::ReadEnd(Type iswhat) {
	return data->ReadEnd(id, iswhat);
}

void BufferInterface::UpdataBufferSpace(Type iswhat) {
	switch (iswhat)
	{
		case X:
		case A_COL:
		case A_ROW:
			buffersize.cur_ax_space += MAX_READ_BYTE;
			if (buffersize.axbuffersize - buffersize.cur_ax_space < MAX_READ_BYTE)
				ax_is_full = true;
			break;
		case WEIGHT:
			buffersize.cur_w_space += MAX_READ_BYTE;
			if (buffersize.wbuffersize - buffersize.cur_w_space < MAX_READ_BYTE)
				w_is_full = true;
			break;
		case OUTPUT:
			buffersize.cur_o_space += MAX_READ_BYTE;
			if (buffersize.obuffersize - buffersize.cur_o_space < MAX_READ_BYTE)
				o_is_full = true;
			break;
	}
}
