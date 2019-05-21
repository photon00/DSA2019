#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <tuple>

class Hasher{
public:
    Hasher(std::string &str, int k1, int k2);
    ~Hasher(){};

    std::tuple<unsigned long long, unsigned long long> get_hash(int idx, int length);
    int get_strLength(){ return _strLength; }
    int update_map(std::tuple<unsigned long long, unsigned long long> &tup, int idx, int target_length);
    void clear_map(){ _hashMap1.clear(); _hashMap2.clear(); }
    
    //std::unordered_map<int, std::tuple<int, int> > length_max_idx;
private:
    int _strLength;
    int _k1, _k2;
    std::vector<unsigned long long> _k1List, _k2List;
    std::vector<unsigned long long> _H1, _H2;
    std::unordered_map<unsigned long long, std::vector<std::tuple<int, int> > > _hashMap1, _hashMap2;
};
