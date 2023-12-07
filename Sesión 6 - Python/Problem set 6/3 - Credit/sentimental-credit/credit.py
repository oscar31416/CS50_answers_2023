while 1:
    try:
        # Get input, and (try to) convert to a number.
        b = int(input("Number: "))
        a = str(b)

        # Negative numbers will NOT be allowed.
        if b < 0:
            continue

        # Calculate first check sum.
        sum1 = 0
        aux1 = 0
        b_copy = b
        while b_copy > 0:

            # Take the current value of b_copy,
            # take the second-to-last digit
            # and multiply it by two.
            aux1 = ((b_copy // 10) % 10) * 2

            # Take aux1's digits and add them to sum1.
            while aux1 > 0:
                sum1 += (aux1 % 10)
                aux1 = aux1 // 10

            # Get rid of b_copy's last two digits and repeat.
            b_copy = b_copy // 100

        # Second check sum.
        del aux1
        b_copy = b
        sum2 = 0
        while b_copy > 0:

            # Take b_copy's last digit and add it to sum2.
            sum2 += (b_copy % 10)

            # Get rid of b_copy's last two digits and repeat.
            b_copy = b_copy // 100

        # If the result of the following operation is NOT zero,
        # the credit card number is not valid.
        sum = (sum1 + sum2) % 10
        if sum != 0:
            print("INVALID")
        elif ((len(a) == 15) and (a[0:2] in ["34", "37"])):
            print("AMEX")
        elif ((a[0] == "4") and (len(a) in [13, 16])):
            print("VISA")
        elif (51 <= int(a[0:2]) and int(a[0:2]) <= 55) and (len(a) == 16):
            print("MASTERCARD")
        else:
            print("INVALID")
        break
    except:
        continue
