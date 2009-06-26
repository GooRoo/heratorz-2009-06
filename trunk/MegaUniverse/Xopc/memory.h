#ifndef _MEMORY_H_
#define _MEMORY_H_

union command {
	struct {
		unsigned int r2 : 14;
		unsigned int r1 : 14;
		unsigned int op : 4;
	} d_command;

	struct {
		unsigned int r1 : 14;			
		unsigned int imm : 6;
		unsigned int cmp_type : 4;
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
