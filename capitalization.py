import sys
s = sys.stdin.readline().strip()

change = s[0]
changed = change.capitalize()

result = changed + s[1:]

print(result)
