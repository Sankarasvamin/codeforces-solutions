user_name = input()

char = set(user_name)
num = len(char)

if num % 2 == 0:
    print("CHAT WITH HER!")
else:
    print("IGNORE HIM!")
