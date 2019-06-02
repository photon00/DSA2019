#include "state.hpp"

long long c[110][110];
int graph[15][15];
int N, M;
std::unordered_map<unsigned long, long long> dp;

inline unsigned long to_state(int l1, int r1, int l2, int r2, int k){
    // ********************************
    // k  -> 7 bits  |  6-0(7)
    // r2 -> 4 bits  |  10-7(4)
    // l2 -> 4 bits  |  14-11(4)
    // r1 -> 4 bits  |  18-15(4)
    // l1 -> 4 bits  |  22-19(4)
    // *********************************/
    unsigned long return_value = (unsigned long)(l1 & 0b1111);
    return_value  = (return_value << 4) + (unsigned long)(r1 & 0b1111);
    return_value = (return_value << 4) + (unsigned long)(l2 & 0b1111);
    return_value = (return_value << 4) + (unsigned long)(r2 & 0b1111);
    return_value = (return_value << 7) + (unsigned long)(k & 0b1111111);
    return return_value;
}

inline std::tuple<int, int, int, int, int> to_tuple(unsigned long state_value){
    int k = state_value & 0b1111111;
    int r2 = (state_value >> 7) & 0b1111;
    int l2 = (state_value >> 11) & 0b1111;
    int r1 = (state_value >> 15) & 0b1111;
    int l1 = (state_value >> 19) & 0b1111;
    return std::make_tuple(l1, r1, l2, r2, k);
}

long long C(int n, int m){
    if (c[n][m] > 0) return c[n][m];
    if (n == m || m == 0) return c[n][m] = (long long)1;
    else return c[n][m] = (C(n-1, m) + C(n-1, m-1)) % MOD;
}

long long search(state &s){
    unsigned long state_value = s.getStateValue();
    auto state_tuple = to_tuple(state_value);
    if (dp.find(state_value) != dp.end()) return dp[state_value];
    
    int Lr = std::get<0>(state_tuple), Rr = std::get<1>(state_tuple);
    int Lc = std::get<2>(state_tuple), Rc = std::get<3>(state_tuple);
    int k = std::get<4>(state_tuple);
    if (k == 0){ return dp[state_value] = (long long)1; }
    ////if (Lr > Rr || Rr > 10 || Lc > Rc || Rc > 10){ return dp[state_value] = (long long)-1;}
    ////if (Lr == Rr && Lc == Rc){ return dp[state_value] = (long long)-1;}
    if (Lr > Rr || Rr > 10 || Lc > Rc || Rc > 10){ return dp[state_value] = (long long)0;}
    if (Lr == Rr && Lc == Rc){ return dp[state_value] = (long long)0;}
    
    s.genAvaCut();
    std::vector<int>& available_cut = s.getAvaCut();
    if (k == 1) return dp[state_value] = (long long)available_cut.size();

    long long score = 0;
    int error_count = 0;
    for (int ka=0; ka<k; ++ka){
        for (auto i=available_cut.begin(); i<available_cut.end(); ++i){
            int cut = *i;
            long long va, vb;
            if (cut < 0){  // cut along column
                int col_cut = ~cut;
                ////if ((col_cut-1 < Lc && ka > 0) || (col_cut+1 > Rc && k-1-ka > 0)) va = -1, vb = -1;
                if ((col_cut-1 < Lc && ka > 0) || (col_cut+1 > Rc && k-1-ka > 0)) va = 0, vb = 0;
                else { 
                    state s1(to_state(Lr, Rr, Lc, col_cut-1, ka));
                    state s2(to_state(Lr, Rr, col_cut+1, Rc, k-1-ka));
                    va = search(s1) % MOD;
                    vb = search(s2) % MOD;
                }
            }
            else {  // cut along row
                ////if ((cut-1 < Lr && ka > 0) || (cut+1 > Rr && k-1-ka > 0)) va = -1, vb = -1;
                if ((cut-1 < Lr && ka > 0) || (cut+1 > Rr && k-1-ka > 0)) va = 0, vb = 0;
                else {
                    state s1(to_state(Lr, cut-1, Lc, Rc, ka));
                    state s2(to_state(cut+1, Rr, Lc, Rc, k-1-ka));
                    va = search(s1) % MOD;
                    vb = search(s2) % MOD;
                }
            }
            if (va >= 0 && vb >= 0){
                //if (va == 0) score = (score + vb) % MOD;
                //else if (vb == 0) score = (score + va) % MOD;
                //else score = (score + (((C(k-1, ka)*va) % MOD )*vb) % MOD) % MOD;
                score = (score + (((C(k-1, ka)*va) % MOD )*vb) % MOD) % MOD;
            }
            else {
                error_count += 1;
            }
        }
    }
    ////if (error_count > 0 && score == 0) score = (long long)-1;
    //if (score == 0) score = (long long)-1;
    return dp[state_value] = score;
}

int main(){
    using namespace std;
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> N >> M;
    char c;
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            cin >> c;
            if (c == 'X') graph[i][j] = 1;
        }
    }

    long long total_score = 1;
    int k = 1;
    while (true){
        state s(to_state(0, N-1, 0, M-1, k));
        long long score = search(s);
        //cout << "k = " << k << " -> score: " << score << endl;
        if (score <= 0 || k > 100) break;
        total_score += score;
        total_score %= MOD;
        ++k;
    }
    cout << total_score << '\n';

    return 0;
}