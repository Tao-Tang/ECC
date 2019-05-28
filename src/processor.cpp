#include <fstream>
#include <cstdint>
#include <float.h>
#include "ctpl_stl.h"
#include "processor.h"
#define buff_size 1 << 20

vector<string> v1;
vector<string> v2;

FILE *my_fopen(char *file_name, const char *mode)
{
	FILE *fp = fopen(file_name,mode);
	if(fp == NULL)
	{
		printf("error, failed to open file %s\n",file_name);
		exit(0);
	}

	return fp;
}


/**
void Processor::pro(char *input, char *output)
{
	ifstream fin;
	odstream fout;
	fin.seekg(0, is.end);
	fout.open(output,ios::out);
	fin.open(input,ios::out);

} **/


void process_files(const vector<string> &list, int start, int end )
{
	ifstream fin;
	ofstream fout;
	string file_name;
	for(int i = start; i < end; i++)
	{
		file_name = list[i];
		char *buff = new char[buff_size];
		fin.open(file_name.c_str(),ifstream::binary);
		fin.seekg(0, fin.end);
		if( fin.tellg() <= buff_size)
		{
			v1[i] =  string(file_name);
			v2[i] = string(file_name);
		}
		else
		{
			v1[i] = file_name;
			v2[i] = "tmp_" + file_name;
			fin.seekg(0, fin.beg);
			
			fin.read(buff, buff_size);
			fout.open(v2[i].c_str(),ifstream::binary);
			fout.write(buff,buff_size);
			fout.close();
		}
		fin.close();
		delete[] buff;
	}
}

void sketch_files(const vector<string> &list, int start, int end )
{
	mash::CommandSketch *cs = new mash::CommandSketch();

	for(int i = start; i < end; i++)
		cs->run(list[i]);

	delete cs;
}


void Processor::read_src_list(char *src_fp,int thread_number)
{
	char file_name[256];
	vector<string> list;
	FILE *file=my_fopen(src_fp,"r");;
	int file_num = 0;
	while (fscanf(file,"%s", file_name) != EOF)
	{
		list.push_back(string(file_name));
		v1.push_back("");
		v2.push_back("");
		++file_num;
	}
	std::cout << "Number of lines in text file: " << file_num << endl;
	fseek(file, 0, SEEK_SET);

	string suffix = ".msh";
	int count = 0;				 

	thread *threads[thread_number];
	int start = 0, task_number = list.size();
	int end = start;
	int avg_task = task_number/thread_number;
	int moduler = task_number%thread_number;

	for (int i = 0; i < thread_number; i++)
	{
		end +=  i < moduler ? avg_task+1 : avg_task;
		threads[i] = new thread(&process_files,list,start,end);
		start = end;
	}

	for (int i = 0; i < thread_number; i++)
	{
		threads[i]->join();
		delete threads[i];
	}
	printf("begin sketch\n");

	start =0;
	end = start;
	for (int i = 0; i < thread_number; i++)
	{
		end +=  i < moduler ? avg_task+1 : avg_task;
		threads[i] = new thread(&sketch_files,v2,start,end);
		start = end;
	}

	for (int i = 0; i < thread_number; i++)
	{
		threads[i]->join();
		delete threads[i];
	}	
	fclose(file);

	return;
}

vector<string> Processor::generate_list()
{
    return v1;
}

void normalize(vector<vector<double>> &vv)
{
	int i,j,n = vv.size();
	double min = vv[0][0],max = DBL_MIN;
    for(i = 0; i < n; i++)
    {
    	for( j = 0; j < n; j++)
    	{
    		if(min > vv[i][j])
    			min = vv[i][j];
    		if(max < vv[i][j])
    			max = vv[i][j];
    	}
    }

    double interval = max - min;
    for(i = 0; i < n; i++)
    {
    	for( j = 0; j < n; j++)
    		vv[i][j] = min + (vv[i][j] - min)/interval;
    }
}


vector<vector<double>> Processor::generate_similarity_matrix()
{
	string suffix = ".msh";
	vector<vector<double>> vv;
	printf("number of files: %d\n", v1.size());
    int i, j, n = v2.size();
    for( i = 0; i < n; i++)
    	vv.push_back( vector<double>(n,0.0) );
    double dist;
 //   map<string,string>::iterator it = src_map.begin();

    vector<string> v;
    for( i = 0; i < n; i++)
    	v.push_back( v2[i] +suffix );

	mash::CommandDistance *cd = new mash::CommandDistance();
	cd->run(v, vv);
	delete cd;

    
    for(i = 0; i < n; i++) 
   	{
   		if(v1[i]!=v2[i])
			remove( v2[i].c_str() );
   		remove( (v2[i] +suffix).c_str() );
	} 
    normalize(vv);  
	return vv;
}


