#ifndef FILENODE_H
#define FILENODE_H

#include"diskdriver.h"
#include<string.h>

int get_empty_node(char *fname);

int get_file_node(char *fname);

void display_meta(file_info file_meta);

#endif