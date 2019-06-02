#include <iostream>
#include <tuple>
#include <vector>
#include <unordered_map>

#define MOD (long long)1000000007

extern long long c[110][110];
extern int graph[15][15];
extern int N, M;
extern std::unordered_map<unsigned long, long long> dp;

extern inline std::tuple<int, int, int, int, int> to_tuple(unsigned long state_value);

class state{
public:
    state(unsigned long state_value): _state_value(state_value){};
    ~state(){};
    void genAvaCut();
    unsigned long getStateValue(){ return _state_value; }
    std::vector<int>& getAvaCut(){ return _available_cut; }

private:
    unsigned long _state_value;
    std::vector<int> _available_cut;
};