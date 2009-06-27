#include "memory.h"
#include <cstdio>
#include <iostream>
#include <stdexcept>

Memory::Memory()
{
	m_mem = new frame[MEMORY_SIZE];
}

Memory::~Memory()
{
	delete[] m_mem;
}

double Memory::getData(addr address) const
{
	if( ( address >= 0 ) && ( address < MEMORY_SIZE ) )
	{
		if(address % 2)
			return m_mem[address].odd_frame.data;
		else
			return m_mem[address].even_frame.data;

	}
    throw std::out_of_range("Wrong data-read address");
}

bool Memory::setData(addr address, double data)
{
	if( ( address >= 0 ) && ( address < MEMORY_SIZE ) )
	{
		if(address % 2)
			m_mem[address].odd_frame.data = data;
		else
			m_mem[address].even_frame.data = data;			

		return true;
	}
	return false;
}

command Memory::getCommand(addr address) const
{
	if( ( address >= 0 ) && ( address < MEMORY_SIZE ) )
	{
		if(address % 2)
			return m_mem[address].odd_frame.cmd;
		else
			return m_mem[address].even_frame.cmd;
			
	}
    throw std::out_of_range("Wrong command address");
}


void Memory::loadFile(const std::string _filename)
{
    FILE *f = fopen(_filename.c_str(), "rb");
	if(f != NULL)
	{
		while(!feof(f))
		{
				fread(m_mem, sizeof(frame), MEMORY_SIZE, f);
		}
		fclose( f );
	}
    else
    {
        std::cerr << "Unable to open file `" << _filename << "\'." << std::endl;
        return;
    }
}
