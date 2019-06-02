#include "state.hpp"


void state::genAvaCut(){
    auto state_tuple = to_tuple(_state_value);
    int Lr = std::get<0>(state_tuple), Rr = std::get<1>(state_tuple);
    int Lc = std::get<2>(state_tuple), Rc = std::get<3>(state_tuple);

    if (Lr < Rr)
        for (int i=Lr; i<=Rr; ++i){
            int flag = 0;
            for (int j=Lc; j<=Rc; ++j){ if (graph[i][j]==1){ flag=1; break; } }
            if (flag == 0) _available_cut.push_back(i); 
        }
    
    if (Lc < Rc)
        for (int j=Lc; j<=Rc; ++j){
            int flag = 0;
            for (int i=Lr; i<=Rr; ++i){ if(graph[i][j]==1){ flag=1; break;} }
            if (flag == 0) _available_cut.push_back(~j);
        }

}