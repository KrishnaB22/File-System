#ifndef VDALLOCATE_H
#define VDALLOCATE_H


//----------------------

#include<string.h>

#include"diskdriver.h"
#include"bitio.h"
#include"disk_util.h"
#include"filenode.h"
#include"diskfunc.h"
#include"vdadd.h"
#include"vdread.h"
#include"vddelete.h"

void user_actions(char *ufname,char *bitmap);


#endif