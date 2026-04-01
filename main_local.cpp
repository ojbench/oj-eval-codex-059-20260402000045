#include <bits/stdc++.h>
#include "src.hpp"
using namespace std;

static string get_data_local(istream &cin) {
    string ret="", line;
    string first;
    getline(cin, first); // consume leftover newline
    while (getline(cin, line)) {
        if (line.size()==0) continue;
        if (line == "#####") break;
        ret += line + "\n";
    }
    return ret;
}

int main(){
    // Use the README driver logic approximated
    string judger_name; if(!(cin>>judger_name)) return 0;
    size_t time_limit, mem_limit; cin>>time_limit>>mem_limit;
    BaseJudger *judger=nullptr; string tmp;
    if (judger_name=="OIJudger"){
        tmp = get_data_local(cin);
        judger = new OIJudger(time_limit, mem_limit, tmp.c_str());
    } else if (judger_name=="ICPCJudger"){
        tmp = get_data_local(cin);
        judger = new ICPCJudger(time_limit, mem_limit, tmp.c_str());
    } else if (judger_name=="SpacialJudger"){
        size_t fst, fsm; cin>>fst>>fsm;
        tmp = get_data_local(cin);
        judger = new SpacialJudger(time_limit, mem_limit, fst, fsm, tmp.c_str());
    } else { cout<<"Unknown Judger type"<<endl; return 0; }

    size_t submit_time, submit_mem;
    while (cin>>submit_time>>submit_mem){
        string out = get_data_local(cin);
        judger->Submit(submit_time, submit_mem, out.c_str());
    }
    cout<<judger->GetScore()<<"\n";
    delete judger; return 0;
}
