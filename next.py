import sys
lst = [word for line in sys.stdin for word in line.split()]
std = int(lst[int(lst[1])+1])
realrank = lst[2:]

t = 0

for i in realrank:
    if int(i) >= std and int(i) > 0:
        t+=1

print(t)