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
#define CHAR_OFFSET 0x60
#define LARGE_PRIME (((unsigned long long)1<<32)-(unsigned long long)5)


Hasher::Hasher(std::string &str, int k1, int k2): _k1(k1), _k2(k2){
    _strLength = str.length();
    _H1.reserve(_strLength);
    _H2.reserve(_strLength);
    _k1List.reserve(_strLength);
    _k2List.reserve(_strLength);

    _H1.push_back(str[0] - CHAR_OFFSET);
    _H2.push_back(str[0] - CHAR_OFFSET);
    _k1List.push_back(_k1);
    _k2List.push_back(_k2);
    for (int i=1; i<_strLength; ++i){
        _H1.push_back( ((_H1[i-1] * (unsigned long long)_k1) + (str[i] - CHAR_OFFSET))%LARGE_PRIME );
        _H2.push_back( ((_H2[i-1] * (unsigned long long)_k2) + (str[i] - CHAR_OFFSET))%LARGE_PRIME );
        _k1List.push_back((_k1List[i-1] * _k1)%LARGE_PRIME);
        _k2List.push_back((_k2List[i-1] * _k2)%LARGE_PRIME);
    }
}

std::tuple<unsigned long long, unsigned long long> Hasher::get_hash(int idx, int length){
    if (idx == 0) return std::make_tuple(_H1[length-1], _H2[length-1]);
    unsigned long long return_hash1, second_term1 = (_H1[idx-1] * _k1List[length-1]) % LARGE_PRIME;
    if (second_term1 > _H1[length+idx-1])
        return_hash1 = (_H1[length+idx-1] + (LARGE_PRIME - second_term1)) % LARGE_PRIME;
    else
        return_hash1 = (_H1[length+idx-1] - second_term1) % LARGE_PRIME;

    unsigned long long return_hash2, second_term2 = (_H2[idx-1] * _k2List[length-1]) % LARGE_PRIME;
    if (second_term2 > _H2[length+idx-1])
        return_hash2 = (_H2[length+idx-1] + (LARGE_PRIME - second_term2)) % LARGE_PRIME;
    else
        return_hash2 = (_H2[length+idx-1] - second_term2) % LARGE_PRIME;
    return std::make_tuple(return_hash1, return_hash2);
}

int Hasher::update_map(std::tuple<unsigned long long, unsigned long long> &tup, int idx, int length){
    unsigned long long hash1 = std::get<0>(tup);
    unsigned long long hash2 = std::get<1>(tup);
    int return_count = 1;
    if (_hashMap1.find(hash1) != _hashMap1.end() && _hashMap2.find(hash2) != _hashMap2.end()){
        bool match_flag = false;
        for (auto i = _hashMap1[hash1].begin(); i != _hashMap1[hash1].end(); ++i){
            for (auto j = _hashMap2[hash2].begin(); j != _hashMap2[hash2].end(); ++j){
                if ((*i) == (*j)){
                    std::get<0>(*i) += 1; std::get<0>(*j) += 1;
                    std::get<1>(*i) = idx; std::get<1>(*j) = idx;
                    return_count = std::get<0>(*i);
                    /*if (return_count >= std::get<0>(length_max_idx[length]))
                        length_max_idx[length] = std::make_tuple(return_count, idx);*/
                    match_flag = true;
                    break;
                }
            }
            if (match_flag) break;
        }
        if (!match_flag){
            _hashMap1[hash1].push_back(std::make_tuple(1, idx));
            _hashMap2[hash2].push_back(std::make_tuple(1, idx)); 
            /*if (length_max_idx.find(length) == length_max_idx.end())
                length_max_idx[length] = std::make_tuple(1, idx);*/
        }
    }
    else {
        _hashMap1[hash1].push_back(std::make_tuple(1, idx));
        _hashMap2[hash2].push_back(std::make_tuple(1, idx));
        /*if (length_max_idx.find(length) == length_max_idx.end())
            length_max_idx[length] = std::make_tuple(1, idx);*/
    }
    return return_count;
}
template <class T>
inline bool compare_tuple(std::tuple<T, T> &a, std::tuple<T, T>&b){
    if (std::get<0>(a) == std::get<0>(b) && std::get<1>(a) == std::get<1>(b))
        return true;
    return false;
}

std::tuple<int, int> binary_search_mtimes(Hasher &hasher, int m){
    int str_length = hasher.get_strLength();
    int target_length, min_length = 1, max_length = str_length - m + 1;
    while (min_length <= max_length){  // iterative binary search over avaliable length
        target_length = (min_length + max_length) / 2;
        bool match_flag = false;
        //for (int i=0; i<str_length-target_length+1; ++i){  // scan over the string and count the hash value
        for (int i=str_length-target_length; i>=0; --i){  // scan over the string and count the hash value
            auto hash_tuple = hasher.get_hash(i, target_length);
            int count = hasher.update_map(hash_tuple, i, target_length);
            if (count >= m){
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
        hasher.clear_map();
    }
    int return_length = max_length, return_idx = -1;
    for (int i=0; i<str_length-max_length+1; ++i){
        auto hash_tuple = hasher.get_hash(i, max_length);
        int count = hasher.update_map(hash_tuple, i, max_length);
        if (count >= m){
            return_idx = i;
        }
    }
    //hasher.clear_map();
    /*if (hasher.length_max_idx.find(max_length) !=  hasher.length_max_idx.end() && 
            std::get<0>(hasher.length_max_idx[max_length]) >= m)
        return_idx = std::get<1>(hasher.length_max_idx[max_length]);
    */
    return std::make_tuple(return_length, return_idx);
}

int binary_search_prepost(Hasher &hasher){
    int str_length = hasher.get_strLength();
    std::vector< std::tuple<int, std::tuple<unsigned long long, unsigned long long> > > available_lengths;  // [(length, (hash_value1, hash_value2)), ...]
    available_lengths.reserve(str_length);
    for (int l=1; l<str_length-1; ++l){
        auto pre_hash_tuple = hasher.get_hash(0, l);
        auto post_hash_tuple = hasher.get_hash(str_length-l, l);
        if (compare_tuple<unsigned long long>(pre_hash_tuple, post_hash_tuple)){
            available_lengths.push_back(std::make_tuple(l, pre_hash_tuple));
        }
    }

    int target_length_idx, min_length_idx = 0, max_length_idx = available_lengths.size()-1;
    while (min_length_idx <= max_length_idx){  // iterative binary search over avaliable length
        target_length_idx = (min_length_idx + max_length_idx) / 2;
        int target_length = std::get<0>(available_lengths[target_length_idx]);
        auto target_hash_tuple = std::get<1>(available_lengths[target_length_idx]);
        bool match_flag = false;
        //for (int i=1; i<str_length-target_length; ++i){  // scan over the string and count the hash value
        for (int i=str_length-target_length-1; i>0; --i){  // scan over the string and count the hash value
            auto hash_tuple = hasher.get_hash(i, target_length);
            if (compare_tuple<unsigned long long>(hash_tuple, target_hash_tuple)){
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
        Hasher myhasher(str, 31, 29);
        auto match_tuple = binary_search_mtimes(myhasher, m);
        if (get<1>(match_tuple) < 0) cout << "none\n";
        else cout << get<0>(match_tuple) << ' ' << get<1>(match_tuple) << '\n';

        int biggest_length = binary_search_prepost(myhasher);
        if (biggest_length < 0) cout << "Just a legend\n";
        else cout << str.substr(0, biggest_length) << '\n';
        cin >> m;
    }
}