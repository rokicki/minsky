#include <x86intrin.h>
#include <iostream>
#include <math.h>
#include <sys/time.h>
using namespace std ;
static double start ;
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
struct pos {
   double startpos ;
   double x, y ;
   long long starttime ;
   int online() {
      return y > 0 && x * 34 == y * 7 ;
   }
} ;
struct worker {
   pos p[16] ;
   long long cnt ;
   worker() {
      cnt = 0 ;
   }
   void newjob(int at, double pos) {
      p[at].x = 7.0 * pos ;
      p[at].y = 34.0 * pos ;
      p[at].starttime = cnt ;
      p[at].startpos = pos ;
   }
   void advance() ;
} ;
union unpacker {
   double a[4] ;
   __m256d p ;
} ;
void unpack(__m256d v, double &v0, double &v1, double &v2, double &v3) {
   unpacker u ;
   u.p = v ;
   v0 = u.a[3] ;
   v1 = u.a[2] ;
   v2 = u.a[1] ;
   v3 = u.a[0] ;
}
/*
 *   Do 1M advances from f(v1), f(v1+1), f(v1+2), f(v1+3)
 *   where f(v1) = (7*v1, 34*v1).
 */
void worker::advance() {
   __m256d mulx = _mm256_set1_pd(15.0/2.0) ;
   __m256d muly = _mm256_set1_pd(9.0/17.0) ;
   __m256d mulxx = _mm256_set1_pd(34.0) ;
   __m256d mulyy = _mm256_set1_pd(7.0) ;
   __m256d x0 = _mm256_set_pd(p[0].x, p[1].x, p[2].x, p[3].x) ;
   __m256d y0 = _mm256_set_pd(p[0].y, p[1].y, p[2].y, p[3].y) ;
   __m256d x1 = _mm256_set_pd(p[4].x, p[5].x, p[6].x, p[7].x) ;
   __m256d y1 = _mm256_set_pd(p[4].y, p[5].y, p[6].y, p[7].y) ;
   __m256d x2 = _mm256_set_pd(p[8].x, p[9].x, p[10].x, p[11].x) ;
   __m256d y2 = _mm256_set_pd(p[8].y, p[9].y, p[10].y, p[11].y) ;
   __m256d x3 = _mm256_set_pd(p[12].x, p[13].x, p[14].x, p[15].x) ;
   __m256d y3 = _mm256_set_pd(p[12].y, p[13].y, p[14].y, p[15].y) ;
   int working = 1 ;
   long long tick = cnt ;
   while (working) {
      __m256d t0 = _mm256_floor_pd(_mm256_mul_pd(y0, muly)) ;
      x0 = _mm256_sub_pd(x0, t0) ;
      t0 = _mm256_floor_pd(_mm256_mul_pd(x0, mulx)) ;
      y0 = _mm256_add_pd(y0, t0) ;
      t0 = _mm256_cmp_pd(_mm256_mul_pd(x0, mulxx),
                          _mm256_mul_pd(y0, mulyy), _CMP_EQ_OQ) ;
      working &= _mm256_testz_pd(t0, t0) ;
      __m256d t1 = _mm256_floor_pd(_mm256_mul_pd(y1, muly)) ;
      x1 = _mm256_sub_pd(x1, t1) ;
      t1 = _mm256_floor_pd(_mm256_mul_pd(x1, mulx)) ;
      y1 = _mm256_add_pd(y1, t1) ;
      t1 = _mm256_cmp_pd(_mm256_mul_pd(x1, mulxx),
                          _mm256_mul_pd(y1, mulyy), _CMP_EQ_OQ) ;
      working &= _mm256_testz_pd(t1, t1) ;
      __m256d t2 = _mm256_floor_pd(_mm256_mul_pd(y2, muly)) ;
      x2 = _mm256_sub_pd(x2, t2) ;
      t2 = _mm256_floor_pd(_mm256_mul_pd(x2, mulx)) ;
      y2 = _mm256_add_pd(y2, t2) ;
      t2 = _mm256_cmp_pd(_mm256_mul_pd(x2, mulxx),
                          _mm256_mul_pd(y2, mulyy), _CMP_EQ_OQ) ;
      working &= _mm256_testz_pd(t2, t2) ;
      __m256d t3 = _mm256_floor_pd(_mm256_mul_pd(y3, muly)) ;
      x3 = _mm256_sub_pd(x3, t3) ;
      t3 = _mm256_floor_pd(_mm256_mul_pd(x3, mulx)) ;
      y3 = _mm256_add_pd(y3, t3) ;
      t3 = _mm256_cmp_pd(_mm256_mul_pd(x3, mulxx),
                          _mm256_mul_pd(y3, mulyy), _CMP_EQ_OQ) ;
      working &= _mm256_testz_pd(t3, t3) ;
      tick++ ;
   }
   unpack(x0, p[0].x, p[1].x, p[2].x, p[3].x) ;
   unpack(y0, p[0].y, p[1].y, p[2].y, p[3].y) ;
   unpack(x1, p[4].x, p[5].x, p[6].x, p[7].x) ;
   unpack(y1, p[4].y, p[5].y, p[6].y, p[7].y) ;
   unpack(x2, p[8].x, p[9].x, p[10].x, p[11].x) ;
   unpack(y2, p[8].y, p[9].y, p[10].y, p[11].y) ;
   unpack(x3, p[12].x, p[13].x, p[14].x, p[15].x) ;
   unpack(y3, p[12].y, p[13].y, p[14].y, p[15].y) ;
   cnt = tick ;
}
int main(int argc, char *argv[]) {
   duration() ;
   double todo = 1 ;
   if (argc > 1)
      todo = atof(argv[1]) ;
   double maxv = 2000000000 ;
   if (argc > 2)
      maxv = atof(argv[2]) ;
   double needed = maxv - todo ;
   worker wk ;
   for (int i=0; i<16; i++)
      wk.newjob(i, todo++) ;
   long long target = 0 ;
   double st = walltime() ;
   while (needed) {
      wk.advance() ;
      if (wk.cnt > target) {
         double dur = walltime() - st ;
         double rate = 16*wk.cnt / dur ;
         cout << "Scanned " << (16*wk.cnt) << " in " << dur << " rate "
              << rate << endl ;
         target += 62500000 ;
      }
      for (int i=0; i<16; i++)
         if (wk.p[i].online()) {
            if (wk.p[i].startpos < maxv) {
               needed-- ;
               cout << "From " << (long long)wk.p[i].startpos << " "
                    << (long long)(wk.p[i].y/34.0) << " "
                    << (wk.cnt-wk.p[i].starttime) << endl << flush ;
            }
            wk.newjob(i, todo++) ;
         }
   }
}
