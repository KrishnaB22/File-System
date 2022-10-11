#ifndef VDREAD_H
#define VDREAD_H

#include"diskdriver.h"
#include"filenode.h"
#include"bitio.h"
#include"diskfunc.h"
#include<string.h>

void read_file(char *fname,char *outname);

int read_bytes(char *fname,char *outname, int startnum,int tnum,char *out);


#endif