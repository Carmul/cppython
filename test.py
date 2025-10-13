

num = 1
while num < 100:
    i = 2
    flag = num != 1
    while i <= num / 2 and flag:
        if num % i == 0:
            flag = False
        i = i + 1
     
    if flag:
        println(num, " is prime")
    else:
        println(num, " is not prime")
    num = num + 1
