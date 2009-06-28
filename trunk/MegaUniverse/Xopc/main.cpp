#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "virtualmachine.h"
#include "orbit_controller.h"

int main(int _argc, char * _argv[])
{
    try 
    {
	    VirtualMachine vm;
		vm.loadBinary("bin1.obf");
	
		OrbitController controller;
		vm.connect(&controller);
	
		vm.run();
    }
    catch (const std::exception & ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

