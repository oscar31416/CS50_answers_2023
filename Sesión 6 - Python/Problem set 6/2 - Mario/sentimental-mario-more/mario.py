while 1:
    try:
        a = int(input("Height: "))
        if not ((a < 1) or (a > 8)):
            break
    except:
        continue

for i in range(1, a + 1):
    print((" ") * (a - i), end = "")
    print("#" * i, end = "")
    print("  ", end = "")
    print("#" * i, end = "\n")
