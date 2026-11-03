import sys
nums = list(map(int, sys.stdin.read().split()))
s = nums[0] * nums[1]

print(s // 2)