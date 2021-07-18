# Comment #1
DEW_COMMON_DIR = $(PWD)/../../dew_common
DEW_COMMON_FILES = dew_ksock.c dew_hash.c
DEW_COMMON_OBJS = $(patsubst %.c, %.o, $(DEW_COMMON_FILES))

# Comment #2
DEWV_COMMON_DIR = $(PWD)/../common

# Comment #3
obj-m += dewv_lkm.o
dewv_lkm-objs := dewvl_filter.o dew_ksock.o $(DEW_COMMON_OBJS)

EXTRA_CFLAGS += -I$(DEW_COMMON_DIR) -I$(DEWV_COMMON_DIR) -DDEBUG

all: 
	for f in $(DEW_COMMON_FILES); do cp $(DEW_COMMON_DIR)/$$f ./; done
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	rm -f $(DEW_COMMON_FILES)
	rm -f $(DEWV_COMMON_FILES)
clean:
	rm -f $(DEW_COMMON_FILES)
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
