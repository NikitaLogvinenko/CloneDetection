#include <iostream>
#include "parsing.h"
int main(int argc,char *argv[]) {
    if(argc!=3)
    {
        std::cout<<"I need only two argument's: file name for reading and file name to write the result's."<<std::endl;
    }else
    {
        char file_name_in[100];
        char file_name_out[100];
        sscanf(argv[1],"%s",file_name_in);
        sscanf(argv[2],"%s",file_name_out);
        work_with_file(file_name_in,file_name_out);
        //вызов парсера
    }
    return 0;
}