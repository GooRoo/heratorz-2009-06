#include <cstdlib>
#include "virtualmachine.h"
#include "orbit_controller.h"

int main(int _argc, char * _argv[])
{
	VirtualMachine vm;
	vm.loadBinary("bin1.obf");

	OrbitController controller;
	vm.connect(&controller);

	vm.run();

    return 0;
}

