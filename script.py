import os
import random

myfiles = os.listdir('inputs/')

ops = ['add','read','delete']

f = open("commands.txt",'w')


for i in range(100):

    action = random.choice(ops)
    outname = random.choice(myfiles)
    fname = outname + '_' + str(i)

    fname = 'inputs/' + fname

    if action == 'read':
        outname = 'outputs/' + outname
    else:
        outname = 'inputs/' + outname


    f.write("{}\n{}\n{}\n".format(action,fname,outname))

f.write('end')
f.close()

