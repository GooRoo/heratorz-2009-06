#include <iostream>
using namespace std;

union command{
		struct {
			unsigned r2 : 14;
			unsigned r1 : 14;
			unsigned op : 4;
		} d_command;

		struct {
			unsigned r1 : 14;			
			unsigned imm : 10;
			unsigned op : 4;
			unsigned flag : 4;
		} s_command;

	};

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
			//cout<<"noop"<<endl;
			break;
		case 1:
			cout<<"cmpz "<<com.s_command.imm<<" "<<com.s_command.r1<<endl;
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

	
	command com;

	double data;

	FILE *f = fopen( filename, "r" );
	int frame_cnt = 0;

	if(f)
	{
		while(!feof(f))
		{
			if(frame_cnt % 2)
			{
				fread(&com, sizeof(com), 1, f);
				fread(&data, sizeof(data), 1, f);
			}
			else
			{
				fread(&data, sizeof(data), 1, f);
				fread(&com, sizeof(com), 1, f);
			}
			
			decoder(com);

			frame_cnt++;

		}
		fclose( f );
	}
}