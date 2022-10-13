import os
import random

myfiles = os.listdir('inputs/')

ops = ['add','read','delete']

f = open("mycommands.txt",'w')


for i in range(100):

    action = random.choice(ops)
    outname = random.choice(myfiles)
    fname = outname + '_' + str(i)

    fname = 'inputs/' + fname
    outname = 'inputs/' + outname

    f.write("{}\n{}\n{}\n".format(action,fname,outname))

f.write('end')
f.close()

