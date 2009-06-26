#include <iostream>
#include "memory.h"

using namespace std;
void decoder(command com)
{
	if(com.s_command.flag)
	{
		switch(com.d_command.op)
		{
		case 1:
			cout<<"add "<<com.d_command.r1<<" "<<com.d_command.r2<<endl;
			break;
		case 2:
			cout<<"sub "<<com.d_command.r1<<" "<<com.d_command.r2<<endl;
			break;
		case 3:
			cout<<"mult "<<com.d_command.r1<<" "<<com.d_command.r2<<endl;
			break;
		case 4:
			cout<<"div "<<com.d_command.r1<<" "<<com.d_command.r2<<endl;
			break;
		case 5:
			cout<<"out "<<com.d_command.r1<<" "<<com.d_command.r2<<endl;
			break;
		case 6:
			cout<<"phi "<<com.d_command.r1<<" "<<com.d_command.r2<<endl;
			break;
		}
	}
	else
	{
		switch(com.s_command.op)
		{
		case 0:
		//	cout<<"noop"<<endl;
			break;
		case 1:
			cout<<"compare "<<com.s_command.r1;

			switch(com.s_command.cmp_type)
			{
			case 0:
				cout<<" <";
				break;
			case 1:
				cout<<" <=";
				break;
			case 2:
				cout<<" =";
				break;
			case 3:
				cout<<" >=";
				break;
			case 4:
				cout<<" >";
				break;
			}

			cout<<" 0"<<endl;
			break;
		case 2:
			cout<<"sqrt "<<com.s_command.r1<<endl;
			break;
		case 3:
			cout<<"copy "<<com.s_command.r1<<endl;
			break;
		case 4:
			cout<<"input "<<com.s_command.r1<<endl;
			break;
		}
	}
};


void main ()
{
	char* filename = "bin1.obf";
	Memory mem(300);

	mem.loadFile(filename);

	for(int i=0; i< 300; i++)
		decoder(mem.getCommand(i));

}