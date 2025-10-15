
def main():
    num = 1
    while num < 1000:
        print_if_prime(num)
        num = num + 1
    add("Carmel is ", "Smart")

def print_if_prime(n):
    ff()
    if n > 1:
        i = 2
        flag = True
        while i <= n / 2 and flag:
            if n % i == 0:
                flag = False
            i = i + 1
        if flag:
            println(n, " is prime")

def add(a, b):
    println(a + b)

main()