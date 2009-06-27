#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <string>

typedef unsigned short addr;

union command {
	struct {
		unsigned int r2 : 14;
		unsigned int r1 : 14;
		unsigned int op : 4;
	} d_command;

	struct {
		unsigned int r1 : 14;			
		unsigned int imm : 7;
		unsigned int cmp_type : 3;
		unsigned int op : 4;
		unsigned int flag : 4;
	} s_command;
};

union frame 
{
	struct {
		double data;
		command cmd;
	} even_frame;

	struct {
		command cmd;
		double data;
	} odd_frame;
};



class Memory
{
public:
	Memory();
	~Memory();

    void loadFile(const std::string filename);

    // Access methods
	double getData(addr address) const;
	bool setData(addr address, double data);

    command getCommand(addr address) const;
	
private:
    static const size_t MEMORY_SIZE = 16384;

    frame* m_mem;
};

#endif // _MEMORY_H_
