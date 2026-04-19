#include<bits/stdc++.h>
using namespace std;
 
string solve (int n, vector<int> arr) {
        sort(arr.rbegin(),arr.rend());
    long long alex=0,bob=0;
    bool turn=true;
    vector<int>a,b;
    for (int i=0;i<n;i++) {
        int val=arr[i];
        bool used=false;
        if (turn) {
            // check in a for duplicates using simple loop
            for (int j=0;j<a.size();j++) {
                if (a[j]==val) {
                    used=true;
                    break;
                }
            }
            // adding value to alex
            if (!used){
                alex+=val;
                a.push_back(val);
                turn = !turn;
            }
        } else {
            // check in b for duplicates using simple loop
            for (int j=0;j<b.size();j++) {
                if (b[j]==val) {
                    used=true;
                    break;
                }
            }
            if (!used){
                bob+=val;
                b.push_back(val);
                turn=!turn;
            }
        }
    }
 
    return (alex > bob) ? "Alex" : "Bob";
}
int main() {
 
    ios::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    for(int t_i = 0; t_i < T; t_i++)
    {
        int n;
        cin >> n;
        vector<int> arr(n);
        for(int i_arr = 0; i_arr < n; i_arr++)
        {
        	cin >> arr[i_arr];
        }
 
        string out_;
        out_ = solve(n, arr);
        cout << out_;
        cout << "\n";
    }
}