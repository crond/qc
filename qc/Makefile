# Makefile - Generic Serial Module


CC=	gcc
INCLUDE= -I/root/quaddrocopter/include
LIBRARY= -L../
CFLAGS_TGT=  -DEMBED_TGT=1
#LIBRARY= -L../pclib -lpc
#LIBRARY= -L../../pc_gsm/gsmlib -lpc
OBJS= main.o \
		sock.o \
		serial.o\
		i2c.o	\
		camera.o \
		processMsg.o
			

TARGET=qc

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) 
#	$(CC) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS_TGT) -c $^ $(INCLUDE)
#	$(CC) $(CFLAGS_HOST) -c $^ $(INCLUDE)

clean:
	rm $(OBJS) $(TARGET)
