while (1==1):
    x = input("enter the text: ")
    if (x==''): break
    for i in range(len(x)):
        j = ord(x[i])-1
        n = 0
        while (j - 9 >= 0):
            n += 1
            j -= 9
        print(('9' * n + str(j))+'+.1\'',end='',sep='')
    print('\n\n')