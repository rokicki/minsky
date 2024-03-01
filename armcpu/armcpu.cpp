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
int main(int argc, char *argv[]) {
   duration() ;
   ll start = atoll(argv[1]) ;
   ll end = atoll(argv[2]) ;
   ll nxt = start ;
   vector<ll> res(end-start, -1) ;
   vector<ll> endp(end-start, -1) ;
   for (int i=0; i<N; i++) {
      x[i] = s[i] = start + i ;
      cnt[i] = 0 ;
   }
   nxt += N ;
   int need = end - start ;
   ll rep = start ;
   while (need > 0) {
      int steps = t2() ;
      for (int i=0; i<N; i++) {
         cnt[i] += steps ;
         if (y[i] == 0) {
            if (s[i] < end) {
               need-- ;
               res[s[i]-start] = cnt[i] ;
               endp[s[i]-start] = x[i] ;
               while (rep < end && res[rep-start] > 0) {
                  cout << rep << " " << res[rep-start] << " " << endp[rep-start] << endl << flush ;
                  rep++ ;
               }
            }
            s[i] = x[i] = nxt++ ;
            y[i] = 0 ;
            cnt[i] = 0 ;
         }
      }
   }
   cout << "Finished in " << duration() << endl ;
}
