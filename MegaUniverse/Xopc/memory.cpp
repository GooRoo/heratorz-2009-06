#include "memory.h"
#include <iosfwd>

Memory::Memory(int size)
{
	m_size = size;

	m_mem = new frame[size];
}
Memory::~Memory()
{
	delete[] m_mem;
}

double Memory::getData(int address) const
{
	if( ( address >= 0 ) && ( address < m_size ) )
	{
		if(address % 2)
			return m_mem[address].odd_frame.data;
		else
			return m_mem[address].even_frame.data;

	}
	throw 0;
}

bool Memory::setData(int address, double data)
{
	if( ( address >= 0 ) && ( address < m_size ) )
	{
		if(address % 2)
			m_mem[address].odd_frame.data = data;
		else
			m_mem[address].even_frame.data = data;			

		return true;
	}
	return false;
}

command Memory::getCommand(int address) const
{
	if( ( address >= 0 ) && ( address < m_size ) )
	{
		if(address % 2)
			return m_mem[address].odd_frame.cmd;
		else
			return m_mem[address].even_frame.cmd;
			
	}
	throw 0;

}


void Memory::loadFile(char* filename)
{
	FILE *f = fopen( filename, "r" );
	if(f)
	{
		while(!feof(f))
		{
				fread(m_mem, sizeof(frame), m_size, f);
		}
		fclose( f );
	}
}
