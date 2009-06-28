#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "virtualmachine.h"
#include "orbit_controller.h"
#include "orbit_gui.h"

int main(int _argc, char * _argv[])
{
    QApplication app(_argc, _argv);
    try 
    {
	    VirtualMachine vm;
        vm.loadBinary("bin1.obf");

		OrbitController controller;
		vm.connect(&controller);
	
        OrbitGui gui;
        vm.setGui(&gui);
        gui.show();

        vm.start();
        return app.exec();
    }
    catch (const std::exception & ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}


