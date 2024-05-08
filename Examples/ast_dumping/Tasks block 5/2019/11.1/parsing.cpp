#include "parsing.h"

using namespace std;

vector<string> get_node_from_file(istream &stream_in, string &string_name) {
    string line;
    size_t pos;
    getline(stream_in, line);
    pos = line.find_first_of('=', 0);
    if (pos == string::npos) return vector<string>();
    string_name = line.substr(0, pos-1);
    line = line.substr(pos +2);
    string _new;
    vector<string> dependences;
    for (pos = 0;;) {
        pos = line.find_first_not_of(" \t", pos);
        if (pos == string::npos)
            break;
        size_t pos1=line.find_first_of(',',pos);
        size_t len = (pos1 == string::npos) ? string::npos : pos1 - pos;
        dependences.push_back(line.substr(pos, len));
        if (pos1 == string::npos)
            break;
        pos = pos1+1;
    }
    return dependences;
}

void work_with_file(const string &file_in,const string &file_out)
{
    fstream file;
    file.open(file_in);
    if(!file) return;
    string string_name;
    dependence_graph graph;
    vector<string> dependences;
    while(!file.eof())
    {
        dependences=get_node_from_file(file,string_name);
        if(dependences.size()!=0)
        graph.add_knot(string_name,dependences);
    }
    file.close();
    file.open(file_out,ios::out);
    dependences=graph.topological_sort_by_Tarjan();
    for(auto it:dependences)
    {
       file<<it<<"\n";
    }
}
