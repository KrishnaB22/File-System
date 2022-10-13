import os
import random

myfiles = os.listdir('inputs/')

ops = ['add','read','delete']

f = open("commands.txt",'w')
addedlist = []

for i in range(1000):

    action = random.choice(ops)
    outname = random.choice(myfiles)
    fname = outname + '_' + str(i)

    fname = 'inputs/' + fname

    if action == 'read':
        fname = random.choice(addedlist)
        temp = fname.split('/')
        temp[0] = 'outputs'
        temp[1] = (temp[1].split('_'))[0]
        outname = temp[0] + '/' + temp[1]
    else:
        outname = 'inputs/' + outname

    if action == 'add':
        addedlist.append(fname)


    f.write("{}\n{}\n{}\n".format(action,fname,outname))

f.write('end')
f.close()

