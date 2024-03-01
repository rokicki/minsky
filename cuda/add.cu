#include <iostream>
#include <cuda_runtime.h>
#include <sys/time.h>
using namespace std ;
const int SPIN = 1LL<<19 ;
using ll = long long ;
static double start ;
int n ;
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
__global__ void add(int *a, int *b, int *c, int n)
{
    int index = blockDim.x * blockIdx.x + threadIdx.x;
    if (index < n) {
       int x = a[index] ;
       int y = b[index] ;
       int smcnt = 0 ;
       for (int i=0; i<SPIN; i++) {
          if (y != 0) {
             x -= (y + (y - (y < 0 ? 16 : 0)) / 17) >> 1 ;
             y += (7*x)+(x>>1);
             smcnt++ ;
          }
       }
       a[index] = x ;
       b[index] = y ;
       c[index] = smcnt ;
    }
}
int *a, *b, *c, *s ;
ll *cnt, totwork ;
void getwork(int i, int &nxtval) {
   a[i] = nxtval;
   b[i] = 7*a[i] + (a[i]>>1);
   c[i] = 0;
   s[i] = a[i];
   cnt[i] = 1;
   nxtval-- ;
}
int main(int argc, char *argv[]) {
    double firsttime = walltime() ;
    int asize = atol(argv[1]) ;
    int bsize = atol(argv[2]) ;
    n = asize * bsize ;
    int *d_a, *d_b, *d_c;
    int size = n * sizeof(int);
    int nxtval = -11000000 ;
    duration() ;
    a = (int *)malloc(size);
    b = (int *)malloc(size);
    c = (int *)malloc(size);
    s = (int *)malloc(size);
    cnt = (ll *)malloc(size * 2);
    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);
    cout << "Alloced in " << duration() << endl ;
    for (int i = 0; i < n; i++)
       getwork(i, nxtval);
    cout << "Starting . . ." << endl ;
    for (ll iter=1; ; iter++) {
       cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
       cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);
       cout << "Copied in " << duration() << endl ;
       add<<<asize, bsize>>>(d_a, d_b, d_c, n);
       cout << "Executed in " << duration() << endl ;
       cudaMemcpy(a, d_a, size, cudaMemcpyDeviceToHost);
       cudaMemcpy(b, d_b, size, cudaMemcpyDeviceToHost);
       cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
       cout << "Copied back in " << duration() << endl ;
       for (int i=0; i<n; i++) {
	  if (c[i] == 0) {
             cout << "No work at " << i << endl ;
	     exit(10) ;
          }
          cnt[i] += c[i];
	  if (b[i] == 0) {
	     cout << s[i] << " " << cnt[i] << " " << a[i] << endl ;
	     totwork += cnt[i] ;
	     getwork(i, nxtval);
	  }
       }
       double rate = iter * (double)SPIN * n / (walltime() - firsttime) ;
       cout << "Reviewed in " << duration() << " rate " << rate << endl ;
    }
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(a);
    free(b);
    free(c);
    cout << "Freed in " << duration() << endl ;
    return 0;
}
