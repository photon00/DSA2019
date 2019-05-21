#include "myclass.hpp"

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