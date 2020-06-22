/*******************************************************************
* SSD GCN Project
********************************************************************/

#include "DRAMInterface.h"

using namespace std;

extern uint64_t cycle;
extern uint64_t dram_use_byte;
extern uint64_t read_count;
extern uint64_t write_count;
extern uint64_t mac1_count;
extern uint64_t mac2_count;

DRAMInterface::DRAMInterface(const string& configfile, 
							 const string& systemfile, 
							 const string& logfile, 
							 const string& outputfile, 
							 unsigned memorysize, 
							 float clk_period_in_ns,
							 BufferInterface **buffer_)
{
	dram = DRAMSim::getMemorySystemInstance(configfile, systemfile, logfile, outputfile, memorysize);
	dram->setCPUClockSpeed(static_cast<uint64_t>(1.0/(clk_period_in_ns * 1e-9)));
	read_cb = new DRAMSim::Callback<DRAMInterface, void, unsigned, uint64_t, uint64_t>(this, &DRAMInterface::ReadCompleteCallback);
  	write_cb = new DRAMSim::Callback<DRAMInterface, void, unsigned, uint64_t, uint64_t>(this, &DRAMInterface::WriteCompleteCallback);
  	dram->RegisterCallbacks(read_cb, write_cb, NULL);;
	buffer = buffer_;
}

DRAMInterface::~DRAMInterface()
{
	delete dram;
	delete read_cb;
	delete write_cb;
}

void DRAMInterface::UpdateCycle()
{
	dram->update();
}

void DRAMInterface::DRAMRequest(uint64_t id, uint64_t address, bool is_write)
{
	RT tmp = {id, address};
	table.push_back(RT);
	dram->addTransaction(is_write, address);
}

void DRAMInterface::ReadCompleteCallback(unsigned id, uint64_t address, uint64_t clock_cycle) 
{ 
	uint64_t bid;
	Type belong = WhereisItBelong(address);
	string print;
	if (belong == OUTPUT)
		belong = WEIGHT;
	vector<RT>::iterator iter;

	for (iter = table.begin(); iter != table.end(); iter++)
	{
		if (*(iter).address = address)
			break;
	}

	buffer[*(iter).id]->FillBuffer(address, belong);
	table.erase(iter);
	if (belong != WEIGHT)
	{
		buffer->present_ax_req -= MAX_READ_BYTE;
	}
	if (belong == WEIGHT)
		print = "WEIGHT";
	else if (belong == X)
		print = "X";
	else if (belong == A_COL)
		print = "A_COLUMN";
	else if (belong == A_ROW)
		print = "A_ROW";

	cout<<"Cycle: "<<dec<<cycle<<". Data Read Complete. Buffer ID: "<<dec<<*(iter).ids<<"Type: "<<print<<" Address: "<<hex<<address<<endl;
	dram_use_byte += MAX_READ_BYTE;
	read_count++;
}

void DRAMInterface::WriteCompleteCallback(unsigned id, uint64_t address, uint64_t clock_cycle) 
{
	cout<<"Cycle: "<<dec<<cycle<<". Output Write Complete. Address: "<<hex<<address<<endl;
	if (!buffer->isA)
		mac1_count--;
	else
		mac2_count--;

	dram_use_byte += MAX_READ_BYTE;
	write_count++;
}

Type DRAMInterface::WhereisItBelong(uint64_t address) {
  if (address < X_START)
    return WEIGHT;
  else if (address < A_COL_START)
    return X;
  else if (address < A_ROW_START)
    return A_COL;
  else if (address < OUTPUT_START)
    return A_ROW;
  else
  	return OUTPUT;
}
