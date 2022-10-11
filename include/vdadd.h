#ifndef VDADD_H
#define VDADD_H

#include"diskdriver.h"
#include"filenode.h"
#include"bitio.h"
#include"diskfunc.h"
#include<string.h>

int add_file(char *fname,char *outname,char *bitmap);


int insert_at_end(char *fname,char *fname1,char *bitmap);

#endif