#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <tuple>

class Hasher{
public:
    Hasher(std::string str, int k);
    ~Hasher(){};

    unsigned long get_hash(int idx, int length);
    int get_strLength(){ return _strLength; }
    std::unordered_map<unsigned long, int> hash_map;
private:
    int                             _k;
    int                             _strLength;
    std::vector<unsigned long> _kList;
    std::vector<unsigned long> _H;

};
#define CHAR_OFFSET 0x60

Hasher::Hasher(std::string str, int k): _k(k) {
    _strLength = str.length();
    _H.reserve(_strLength);
    _kList.reserve(_strLength);
    _H.push_back(str[0] - CHAR_OFFSET);
    _kList.push_back(_k);
    for (int i=1; i<_strLength; ++i){
        _H.push_back( ((_H[i-1] * (unsigned long)_k) + (str[i] - CHAR_OFFSET)) );
        _kList.push_back((_kList[i-1] * _k));
    }
}

unsigned long Hasher::get_hash(int idx, int length){
    if (idx == 0) return _H[length-1];
    unsigned long second_term = (_H[idx-1] * _kList[length-1]);
    return (_H[length+idx-1] - second_term);
}


std::tuple<int, int> binary_search_mtimes(Hasher &hasher, int m){
    int str_length = hasher.get_strLength();
    int target_length, min_length = 1, max_length = str_length - m + 1;
    while (min_length <= max_length){  // iterative binary search over avaliable length
        target_length = (min_length + max_length) / 2;
        bool match_flag = false;
        for (int i=0; i<str_length-target_length+1; ++i){  // scan over the string and count the hash value
            unsigned long hash_value = hasher.get_hash(i, target_length);
            if (hasher.hash_map.find(hash_value) == hasher.hash_map.end()){
                hasher.hash_map[hash_value] = 1;
            }
            else {
                hasher.hash_map[hash_value] += 1;
            }
            if (hasher.hash_map[hash_value] >= m){
                match_flag = true;
                break;
            }
        }
        if (match_flag){  // meets requirement, update lower bound
            min_length = target_length+1;
        }
        else {  // don't meet, update lower bound
            max_length = target_length-1;
        }
        hasher.hash_map.clear();
    }
    int return_length, return_idx = -1;
    for (int i=0; i<str_length-max_length+1; ++i){
        unsigned long hash_value = hasher.get_hash(i, max_length);
        if (hasher.hash_map.find(hash_value) == hasher.hash_map.end()){
            hasher.hash_map[hash_value] = 1;
        }
        else {
            hasher.hash_map[hash_value] += 1;
        }
        if (hasher.hash_map[hash_value] >= m){
            return_length = max_length, return_idx = i;
        }
    }
    hasher.hash_map.clear();
    return std::make_tuple(return_length, return_idx);
}

int binary_search_prepost(Hasher &hasher){
    int str_length = hasher.get_strLength();
    std::vector< std::tuple<int, unsigned long> > available_lengths;  // [(length, hash_value), ...]
    available_lengths.reserve(str_length);
    for (int l=1; l<str_length-1; ++l){
        unsigned long pre_hash_value = hasher.get_hash(0, l);
        unsigned long post_hash_value = hasher.get_hash(str_length-l, l);
        if (pre_hash_value == post_hash_value){
            available_lengths.push_back(std::make_tuple(l, pre_hash_value));
        }
    }

    int target_length_idx, min_length_idx = 0, max_length_idx = available_lengths.size()-1;
    while (min_length_idx <= max_length_idx){  // iterative binary search over avaliable length
        target_length_idx = (min_length_idx + max_length_idx) / 2;
        int target_length = std::get<0>(available_lengths[target_length_idx]);
        unsigned long target_hash_value = std::get<1>(available_lengths[target_length_idx]);
        bool match_flag = false;
        for (int i=1; i<str_length-target_length; ++i){  // scan over the string and count the hash value
            unsigned long hash_value = hasher.get_hash(i, target_length);
            if (hash_value == target_hash_value){
                match_flag = true;
                break;
            }
        }
        if (match_flag){  // meets requirement, update lower bound
            min_length_idx = target_length_idx+1;
        }
        else {  // don't meet, update lower bound
            max_length_idx = target_length_idx-1;
        }
    }
    if (max_length_idx == -1) return -1;
    return std::get<0>(available_lengths[max_length_idx]);
}

int main(){
    using namespace std;
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int m;
    string str;
    cin >> m;
    while (m > 0){
        cin >> str;
        Hasher myhasher(str, 31);
        auto match_tuple = binary_search_mtimes(myhasher, m);
        if (get<1>(match_tuple) < 0) cout << "none\n";
        else cout << get<0>(match_tuple) << ' ' << get<1>(match_tuple) << '\n';

        int biggest_length = binary_search_prepost(myhasher);
        if (biggest_length < 0) cout << "Just a legend\n";
        else cout << str.substr(0, biggest_length) << '\n';
        cin >> m;
    }
}