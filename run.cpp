#include "IniParser.h"
#include "DataReader.h"
#include "Distributer.h"
#include "Common.h"
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

extern char *optarg;

int main(int argc, char** argv)
{
	int option = 0;
	string ini;
	string data;
	
	if (argc == 1)
	{
		cout<<"You must follow this form: \'./sim -i inifile_path -d datafile_path\'"<<endl;
		return 0;
	}

	while ((option = getopt(argc, argv, "i:d:")) != EOF)
	{
		switch (option)
		{
			case 'i':
				ini = optarg;
				break;
			case 'd':
				data = optarg;
				break;
			case '?':
				cout<<"You must follow this form: \'./sim -i inifile_path -d datafile_path\'"<<endl;
				return 0;
		}
	}
	IniParser *iniparser = new IniParser(ini);
	DataReader *datareader = new DataReader(data);

	//print_initialize(iniparser,datareader);

	Distributer *dis = new Distributer(iniparser, datareader);
	dis->Print();

	return 0;
}




