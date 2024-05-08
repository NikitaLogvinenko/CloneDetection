#include <iostream>
#include "lunch_calculator.h"
using namespace std;
int main(int argc,char *argv[]) {
    if(argc!=5)
    {
        std::cout<<"I need only four argument's: \n1)file name for reading\n2)file name to write the result's\n3)Time begin's\n4)Time end"<<std::endl;
    }else {
        char file_name_in[100];
        char file_name_out[100];
        string time_begin_str=argv[3];
        string time_end_str=argv[4];
        sscanf(argv[1],"%s",file_name_in);
        sscanf(argv[2],"%s",file_name_out);
        std::vector<std::string> vec;
        lunch_calculator lu(file_name_in);
        vec=lu.to_offer_advice(time_begin_str,time_end_str);
        fstream f_out;
        f_out.open(file_name_out,ios::out);
        if(!vec.empty()&&f_out)
        {
            for(auto it:vec)
            {
                f_out<<it;
            }
        }
    }
}