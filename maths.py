s = input()

nums = list(map(int, s.split("+")))

a = nums.count(1)
b = nums.count(2)
c = nums.count(3)

result = ["1"] * a + ["2"] * b + ["3"] * c

print("+".join(result))