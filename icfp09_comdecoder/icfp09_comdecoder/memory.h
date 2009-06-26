#ifndef _MEMORY_H_
#define _MEMORY_H_

union command {
	struct {
		unsigned r2 : 14;
		unsigned r1 : 14;
		unsigned op : 4;
	} d_command;

	struct {
		unsigned r1 : 14;			
		unsigned imm : 6;
		unsigned cmp_type : 4;
		unsigned op : 4;
		unsigned flag : 4;
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
	Memory(int size);
	~Memory();

	void loadFile(char* filename);

    // Access methods
	double getData(int address) const;
	bool setData(int address, double data);

    command getCommand(int address) const;
	
protected:

private:
    frame* m_mem;
	int m_size;

};

#endif // _MEMORY_H_
