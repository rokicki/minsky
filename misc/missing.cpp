#include <iostream>
#include <cstdio>
#include <vector>
using namespace std ;
using ll = long long ;
template<typename T> struct dsvec {
   T& operator[](ll v) {
      lowindex = min(lowindex, v) ;
      hiindex = max(hiindex, v) ;
      if (v < 0)
         return dolook(neg, -1-v) ;
      else
         return dolook(pos, v) ;
   }
   T& dolook(vector<T> &a, ll v) {
      if ((ll)a.size() <= v)
         a.resize(1000 + v + (v >> 2)) ;
      return a[v] ;
   }
   vector<T> pos, neg ;
   ll lowindex, hiindex ;
} ;
dsvec<char> seen ;
const int BUFSIZE = 8192 ;
char linebuf[BUFSIZE+10] ;
int main(int argc, char *argv[]) {
   for (int fi=1; fi<argc; fi++) {
      FILE *f = fopen(argv[fi], "r") ;
      if (f == 0) {
         cerr << "Failed opening " << argv[fi] << endl ;
         exit(10) ;
      }
      while (fgets(linebuf, BUFSIZE, f)) {
         if (linebuf[strlen(linebuf)-1] != 10)
            continue ;
         if (linebuf[0] != '-' && ('0' > linebuf[0] || linebuf[0] > '9'))
            continue ;
         ll a, b, c ;
         if (sscanf(linebuf, "%lld %lld %lld", &a, &b, &c) != 3) {
            cerr << "Could not get three ints from " << argv[fi] << endl ;
            exit(10) ;
         }
         seen[a] = 1 ;
      }
      fclose(f) ;
   }
   for (ll x=1; ; x++) {
      if (!seen[x])
         cout << x << endl ;
      if (!seen[-x])
         cout << -x << endl ;
   }
}
