#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_D"
#include "bits/stdc++.h"

using namespace std;


#include "../lib/classes/sparsetable.cpp"
#include "../lib/classes/lineartimesparsetable.cpp"
#include "../lib/classes/substrmatching.cpp"


signed main(){
    string s, t;
    cin >> s;
    SubstrMatching p(s);
    int q;
    cin >> q;
    for(int i = 0; i < q; ++i){
        cin >> t;
        printf("%d\n", p.contains(t));
    }
}

