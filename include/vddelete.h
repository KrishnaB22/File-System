#ifndef VDDELETE_H
#define VDDELETE_H

#include"diskdriver.h"
#include"filenode.h"
#include"bitio.h"
#include"diskfunc.h"

int delete_file(char *fname,char *bitmap);

int delete_at_end(char *fname,int delnum,char *bitmap);

#endif