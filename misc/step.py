import sys;
x = int(sys.argv[1])
y = int(sys.argv[2])
ox = x
oy = y
cnt = 0
while True:
   if (cnt & (cnt - 1)) == 0 or y == 0 or (x == ox and y == oy):
      print(cnt, x, y)
      sys.stdout.flush()
   if y == 0 and cnt > 0:
      break
   x -= y*9//17
   y += x*15//2
   cnt += 1
