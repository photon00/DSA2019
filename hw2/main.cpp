#include "myclass.hpp"

int main(){
    using namespace std;
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, parentI;
    char c;
    cin >> n;
    Container myContainer(n);
    for (int i=0; i<n; ++i){
        cin >> parentI >> c;
        myContainer.insert(parentI, c);
    }
    myContainer.init();
    myContainer.showTries();
    myContainer.showAll();
    myContainer.showAncestors();

    cin >> n;
    int a, b;
    for (int i=0; i<n; ++i){
        cin >> a >> b;
        cout << myContainer.getLCP(a, b) << '\n';
    }
    return 0;
}
