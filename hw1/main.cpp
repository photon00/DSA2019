#include "myclass.hpp"

int main(){
	using namespace std;
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m, k;
	cin >> n >> m >> k;
	hybrid myHybrid(k, m);
	int num;
	for (int i=0; i<m; ++i){
		cin >> num;
		myHybrid.append(num);
	}
	cout << myHybrid.get_kth() << '\n';
	for (int i=0; i<n-m; ++i){
		cin >> num;
		myHybrid.slide(num);
		cout << myHybrid.get_kth() << '\n';
	}
	return 0;	
}
