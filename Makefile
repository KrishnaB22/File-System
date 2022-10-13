CC = gcc
FS_DIR = $(shell pwd)
INCLUDE_DIR = $(FS_DIR)/include

CFLAGS = -I$(INCLUDE_DIR)

TARGET = diskop
new = diskop
files := vdallocate.c diskdriver.c filenode.c bitio.c disk_util.c diskfunc.c vdadd.c vdread.c vddelete.c

all:
	$(CC) $(files) -o $(TARGET) $(CFLAGS)


create:
	gcc vdcreate.c -o vdcreate && \
	./vdcreate mydisk 134217728 1024 16384

clean:
	rm -rf mydisk $(TARGET) out.txt vdcreate out.mp4

commit:
	git add . && \
	git commit -m "update" && \
	git push --set-upstream origin main