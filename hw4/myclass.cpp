#include "myclass.hpp"
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