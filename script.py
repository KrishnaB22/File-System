import os
import random

myfiles = os.listdir('inputs/')

ops = ['add','read','delete']
#ops = ['add','delete']

f = open("commands.txt",'w')

cadd = 0
cread = 0
cdelete = 0

addedlist = []

for i in range(10000):

    action = random.choice(ops)
    outname = random.choice(myfiles)
    fname = outname + '_' + str(i)

    fname = 'inputs/' + fname
    if i == 0:
        action = 'add'

    if action == 'read':
        fname = random.choice(addedlist)
        temp = fname.split('/')
        temp[0] = 'outputs'
        temp[1] = (temp[1].split('_'))[0]
        outname = temp[0] + '/' + temp[1]
        cread += 1
    else:
        outname = 'inputs/' + outname

    if action == 'add':
        cadd += 1
        addedlist.append(fname)
    
    if action == 'delete':
        fname = random.choice(addedlist)
        cdelete += 1

    f.write("{}\n{}\n{}\n".format(action,fname,outname))

f.write('end')
f.close()

print("total operations add = {}, read = {}, delete = {}".format(cadd,cread,cdelete))