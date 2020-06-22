/*******************************************************************
* SSD GCN Project
********************************************************************/

#include "DataController.h"

using namespace std;

DataController::DataController(Distributer *distributer) {
	ifvalue = distributer->ifvalue;
	adjcolindex = distributer->adjcolindex;
	adjrowindex = distributer->adjrowindex;
	index = new Index[distributer->acc_num];
	x_size = new uint64_t[distributer->acc_num];
	a_col_size = new uint64_t[distributer->acc_num];
	a_row_size = new uint64_t[distributer->acc_num];
	
	for (int i = 0; i < distributer->acc_num; i++)
	{
		index[i].valindex = 0;
		index[i].colindex = 0;
		index[i].rowindex = 0;
		x_size[i] = ifvalue[i].size();
		a_col_size[i] = adjcolindex[i].size();
		a_row_size[i] = adjrowindex[i].size();
	}
}

DataController::~DataController() {
	delete index;
}

uint64_t DataController::ReturnSize(uint64_t id, Type iswhat)
{
	uint64_t ret;

	switch (iswhat)
	{
		case X:
			ret = x_size[id];
			break;
		case A_COL:
			ret = a_col_size[id];
			break;
		case A_ROW:
			ret = a_row_size[id];
			break;
	}

	return ret;
}

vector<float> DataController::ReturnIFM(uint64_t id) {
	vecter<float> ret;

	for (int i = index[id].valindex; i < index[id].valindex + MAX_READ_INT; i++)
	{
		ret.push_back(ifvalue[id][i]);
		if (i == ifvalue[id].size()-1)
			break;
	}

	index[id].valindex += MAX_READ_INT;

	return ret;
}

vector<uint64_t> DataController::ReturnAdjColIndex(uint64_t id) {
	vecter<uint64_t> ret;

	for (int i = index[id].colindex; i < index[id].colindex + MAX_READ_INT; i++)
	{
		ret.push_back(adjcolindex[index][i]);
		if (i == adjcolindex[id].size()-1)
			break;
	}

	index[id].colindex += MAX_READ_INT;

	return ret;
}

vector<uint64_t> DataController::ReturnAdjRowIndex(uint64_t id) {
	vecter<uint64_t> ret;

	for (int i = index[id].rowindex; i < index[id].rowindex + MAX_READ_INT; i++)
	{
		ret.push_back(adjrowindex[index][i]);
		if (i == adjrowindex[id].size()-1)
			break;
	}

	index[id].rowindex += MAX_READ_INT;

	return ret;
}

bool DataController::ReadEnd(uint64_t id, Type iswhat) {
	bool ret;

	switch (iswhat)
	{
		case X:
			ret = (index[id].valindex > ifvalue[id].size());
			break;
		case A_COL:
			ret = (index[id].colindex > adjcolindex[id].size());
			break;
		case A_ROW:
			ret = (index[id].colindex > adjcolindex[id].size());
			break;
	}

	return ret;
}

void DataController::Reset() {
	for (int i = 0; i < distributer->acc_num; i++)
	{
		index[i].valindex = 0;
		index[i].colindex = 0;
		index[i].rowindex = 0;
	}
}