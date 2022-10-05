num = 1
end = 100000
with open("inputs/input2.txt",'w') as f:
    for _ in range(0,end):
        s = str(num) + '\n'
        f.write(s)
        num += 1
