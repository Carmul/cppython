x = 3
flag = False

if x < 10:
    print("x is less than 10")
elif x > 10:
    print("x is greater than 10")
else:
    print("x is equal to 10")
    flag = True

if flag:
    print("Flag is True")
else:
    print("Flag is set to False")

print("End of the program")

if x:
    print("x is non-zero")


num = 100
while num > 0:
    print(num)
    num = num - 5