#include <iostream>
using namespace std;
const int Max= 10000001;
int count[Max];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);//only for fast i/o(took help of websites)
    for (int i=2;i<Max;i++) {
        if (count[i]==0) {
            for (int j=i; j<Max;j+=i) {
              count[j]++;
            }
        }
    }
    int t;
    cin>>t;
    while (t--) {
        int n;
        cin>>n;
        cout<<(1<<count[n])<<'\n';
    }
    return 0;
}