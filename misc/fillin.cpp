#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <vector>
using namespace std ;
#ifndef N
#define N 32
#endif
const int CNT = 1000000 ;
static double start ;
using ll = long long ;
double walltime() {
   struct timeval tv ;
   gettimeofday(&tv, 0) ;
   return tv.tv_sec + 0.000001 * tv.tv_usec ;
}
double duration() {
   double now = walltime() ;
   double r = now - start ;
   start = now ;
   return r ;
}
double s[N], x[N], y[N] ;
ll cnt[N] ;
int t2() {
   for (int j=0; j<CNT; j++) {
      for (int i=0; i<N; i++) {
         x[i] -= floor(y[i] * (9.0/17.0)) ;
         y[i] += floor(x[i] * (15.0/2.0)) ;
      }
      for (int i=0; i<N; i++)
         if (y[i] == 0) {
            return j+1 ;
         }
   }
   return CNT ;
}
vector<ll> todo ;
int modm=1, modi ;
int main(int argc, char *argv[]) {
   ll v ;
   if (argc > 1) {
      modm = atol(argv[1]) ;
      modi = atol(argv[2]) ;
   }
   int seq = 0 ;
   while (cin >> v) {
      if (seq % modm == modi)
         todo.push_back(v) ;
      seq++ ;
   }
   duration() ;
   ll nxt = 0 ;
   for (int i=0; i<N; i++) {
      if (nxt < (int)todo.size()) {
         x[i] = s[i] = todo[nxt] ;
         cnt[i] = 0 ;
         nxt++ ;
      } else {
         exit(10) ;
      }
   }
   while (1) {
      int steps = t2() ;
      for (int i=0; i<N; i++) {
         cnt[i] += steps ;
         if (y[i] == 0) {
            cout << (ll)s[i] << " " << cnt[i] << " " << (ll)x[i] << endl << flush ;
            if (nxt < (int)todo.size()) {
               s[i] = x[i] = todo[nxt] ;
               y[i] = 0 ;
               cnt[i] = 0 ;
               nxt++ ;
            } else {
               exit(10) ;
            }
         }
      }
   }
}
