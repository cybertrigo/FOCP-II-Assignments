#include <iostream>
using namespace std;

int main() {
    int n,k;
    cin>>n>>k;
    string a;
    cin>>a;
    int off=1, on=n, ans=n;
    while (off<=on) {
        int mid=(off+on)/2;
        int ops=0;
        for (int i=0;i<n;i++) {
            if (a[i]=='1') {
                ops++;
                i += mid - 1; // skip next mid bulbs
            }
        }
        if (ops<=k) {
            ans=mid;
            on=mid-1;
        } else {
            off=mid+1;
        }
    }
    cout<<ans;
}