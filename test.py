
def main():

    num = 1
    while num < 99999:
        if is_prime(num):
            println(num, " is Prime")
        num = num + 1
     

def is_prime(n):
    if n < 2:
        return False
    i = 2
    while i <= n / 2:
        if n % i == 0:
            return False
        i = i + 1
    return True


main()