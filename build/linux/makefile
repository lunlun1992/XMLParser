SRC_ROOT = ../..
CFLAGS = -g -O2 -Wall
LDFLAGS = -lpthread
CC = gcc

core_SRCS += $(SRC_ROOT)/file_io.c
core_SRCS += $(SRC_ROOT)/parse_event.c
core_SRCS += $(SRC_ROOT)/post_process.c
core_SRCS += $(SRC_ROOT)/divide_event.c
core_OBJS := $(core_SRCS:%.c=%.o)

exec_SRCS += $(SRC_ROOT)/main.c
exec_OBJS := $(exec_SRCS:%.c=%.o)

EXEC := XMLParser
$(EXEC):$(exec_OBJS) $(core_OBJS) 
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f $(core_OBJS) $(EXEC) $(exec_OBJS)

