/*i used this code due to the time limit exceeded error in hackerearth ... i will write the initial code below*/
#include <iostream>
using namespace std;
int main() {
    long long L,R;
    cin>>L>>R;
    long long count=(R+1)/2-(L/2);
    if (count%2==0) cout<<"even";
    else cout<<"odd";
}

/*the initial code was*/
/*#include <iostream>
using namespace std;
int main(){
    long long L,R,X=0;
    cin>>L>>R;
    for (long long i=L;i<=R;i++) {
      X^=i;
    }
    if (X%2==0) cout<<"even";
    else cout<<"odd";
}*/