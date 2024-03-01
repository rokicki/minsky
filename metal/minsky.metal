#include <metal_stdlib>
using namespace metal;
kernel void minsky(device int32_t* xv, device int32_t* yv,
                   device int64_t* cnt,
                   uint index [[thread_position_in_grid]]) {
    int32_t x = xv[index] ;
    int32_t y = yv[index] ;
    int32_t smcnt = 0 ;
    for (int i=0; i<(1<<18); i++) {
       if (y != 0) {
          x -= (y + (y - (y < 0 ? 16 : 0)) / 17) >> 1 ;
          y += (7*x)+(x>>1);
          smcnt++ ;
       }
    }
    xv[index] = x ;
    yv[index] = y ;
    cnt[index] += smcnt ;
}
