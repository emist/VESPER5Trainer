#include "MemoryUtility.h"
#include "PayManager.h"
#include "PaySegment.h"
#include "Modifier.h"
#include "HexOperations.h"
#include "Logger.h"
#include <sstream>
#include <cstdio>

using namespace std;
using namespace MemUtil;

int main(int argc, char * argv[])
{	
	std::string processName("VESPER5.exe");
	
	DWORD processID = MemoryUtility::FindProcessId(processName);

	if ( processID == 0 )
		std::wcout << "Could not find " << processName.c_str() << std::endl;
	else
		std::wcout << "Process ID is " << processID << std::endl;
	
	stringstream ss;
	long base;
	long offset;
	ss << hex << "5885";
	ss >> hex >> offset;

	base = MemoryUtility::GetModuleBase(processName.c_str(), processID);
	cout << "Base: " << hex << base << endl;

	long move = base+offset;
	//ss << hex << MemUtil::HexOperations::hexadd(offset, base);
	//ss >> hex >> move;

	cout << "Address:" << hex << move << endl;

	HANDLE proc = MemoryUtility::AttachToProcess(processID, false);
	if(proc == NULL)
	{
		cout << "Could not get handle to process" << endl;
		system("PAUSE");
	}
	
	

	PayManager payManager(proc);

	//release date checks
	Payload moremoves("EB11"); //JMP SHORT ....
	PaySegment moveSegment(move, moremoves);
	

	payManager.AddSegment(moveSegment);
	payManager.Inject();

}