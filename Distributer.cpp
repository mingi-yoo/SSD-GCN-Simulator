/*******************************************************************
* SSD GCN Project
********************************************************************/

#include "Distributer.h"


using namespace std;

uint64_t mac1_count;
uint64_t mac2_count;

Distributer::Distributer(IniParser *ini, DataReader *data) {
	// set matrix information
	w_h = data->weight_h;
	w_w = data->weight_w;
	x_h = data->x_h;
	x_w = w_h;
	a_h = x_h;
	a_w = x_h;

	//set data space for each of accelerators
	acc_num = ini->acc_num;
	axbuffersize = ini->axbuffer;
	wbuffersize = ini->weightbuffer;
	obuffersize = ini->outputbuffer;
	ifvalue = new vector<float>[ini->acc_num];
	adjcolindex = new vector<uint64_t>[ini->acc_num];
	adjrowindex = new vector<uint64_t>[ini->acc_num];

	// set MAC count
	w_fold = ceil((float)w_w/MAX_READ_INT);
	v_fold = ceil((float)MAX_READ_INT/acc_num);
	v_fold_last = ceil((float)(w_w - w_fold * MAX_READ_INT)/acc_num);
	mac1_count = mac1_count * w_fold;
	mac2_count = mac2_count * w_fold;
	this->data = data;

	// set Address
	x_addr = new uint64_t[acc_num];
	a_col_addr = new uint64_t[acc_num];
	a_row_addr = new uint64_t[acc_num];
	o_addr = new uint64_t[acc_num];
	row_start = new uint64_t[acc_num];

	for (int i = 0; i < acc_num; i++)
	{
		x_addr[i] = X_START + celi((float)(ifvalue[0].size() * UNIT_INT_BYTE)/MAX_READ_BYTE) * MAX_READ_BYTE * i;
		a_col_addr[i] = A_COL_START + ceil((float)(adjcolindex[0].size() * UNIT_INT_BYTE)/MAX_READ_BYTE) * MAX_READ_BYTE * i;
		a_row_addr[i] = A_ROW_START + ceil((float)(adjrowindex[0].size() * UNIT_INT_BYTE)/MAX_READ_BYTE) * MAX_READ_BYTE * i;
		o_addr[i] = OUTPUT_START + (ceil((float)a_h/acc_num)) * w_fold * MAX_READ_BYTE * i;
		row_start[i] = (ceil((float)a_h/acc_num)) * i;
	}

	Distribute();
}

Distributer::~Distributer() {
	delete ifvalue;
	delete adjcolindex;
	delete adjrowindex;
	delete x_addr;
	delete a_col_addr;
	delete a_row_addr;
	delete o_addr;
	delete row_start;
}

//Distribute data to each accelerator
void Distributer::Distribute() {
	int size = ceil((float)x_h/acc_num);
	int col_size = 0;
	int next_start = col_size;
	uint64_t last = 0;

	adjrowindex[0].push_back(data->adjrowindex[0]);
	for (int i = 0; i < acc_num; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < x_w; k++)
			{
				ifvalue[i].push_back(data->ifvalue[(i*size+j)*x_w+k]);
			}
			if (j == 0 && i != 0)
				adjrowindex[i].push_back(0);
			adjrowindex[i].push_back(data->adjrowindex[j+size*i+1] - last);
			col_size = adjrowindex[i].back();
			if (j == size-1 || j+size*i == size-1)
			{
				for (int k = next_start; k < col_size; k++)
				{
					adjcolindex[i].push_back(data->adjcolindex[k]);
				}
				next_start += col_size;
			}
			if (j+size*i == size-1)
				break;
		}
		last = adjrowindex[i].back();
	}
}

//for debugging
void Distributer::Print()  {
	for (int i = 0; i < acc_num; i++)
	{
		for (int j = 0; j < ifvalue[i].size(); j++)
		{
			cout<<ifvalue[i][j]<<" ";
			if (j%x_w == x_w-1)
				cout<<endl;
		}
		for (int j = 0; j < adjcolindex[i].size(); j++)
		{
			cout<<adjcolindex[i][j]<<" ";
		}
		cout<<endl;
		for (int j = 0; j < adjrowindex[i].size(); j++)
		{
			cout<<adjrowindex[i][j]<<" ";
		}
		cout<<endl;
	}
}