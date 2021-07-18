PROGNAME = dewv_serv
# Comment #1
DEW_COMMON_DIR = ../../dew_common
DEW_COMMON_OBJS = dew_hash.o
DEW_COMMON_TMP =$(addprefix $(DEW_COMMON_DIR)/,$(DEW_COMMON_OBJS))
DEW_COMMON_SRCS =$(patsubst %.o, %.c, $(DEW_COMMON_TMP))

# Comment #2
DEWV_COMMON_DIR = ../common

# Comment #3
THIS_SRCS = $(wildcard *.c)
THIS_OBJS = $(patsubst %.c, %.o, $(THIS_SRCS))

CC = gcc
CFLAGS += -Wall -g -I$(DEW_COMMON_DIR) -I$(DEWV_COMMON_DIR)
LDFLAGS = -lpthread -lm -lssl -lcrypto

$(PROGNAME): $(THIS_OBJS) $(DEW_COMMON_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) 

$(DEW_COMMON_OBJS): $(DEW_COMMON_SRCS)
	$(CC) -c $< -o $@

$(THIS_OBJS): $(THIS_SRCS)
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm -f $(DEW_COMMON_OBJS) $(THIS_OBJS) $(PROGNAME) depend
