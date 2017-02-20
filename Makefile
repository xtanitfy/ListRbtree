#===============================================================================


EXEC_ARM = main


OBJS = main.o
OBJS += ngx_rbtree.o


#CC = $(CROSS_COMPILE)gcc -D _LARGEFILE64_SOURCE -D _FILE_OFFSET_BITS=64 -D _GNU_SOURCE
CC = $(CROSS_COMPILE)gcc $(FALGS)

LDFLAGS = -lpthread -lm $(LIBS)

all: $(EXEC_ARM)
	cp ./$(EXEC_ARM) /opt/app

$(EXEC_ARM): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
#	cp $(PROJECT_PATH)/$(EXEC_ARM) $(IMAGE_PATH)/$(EXEC_ARM)

clean:
	rm -rf *.o $(EXEC_ARM)
	
	
	
	
	
	
	
	
	
	
	
	
	
	
