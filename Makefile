CC = gcc

CFLAGS = -lpthread
 
# The build target 
TARGET = prod_cons
 
all: $(TARGET)
 
$(TARGET): $(TARGET).c
		$(CC) $(TARGET).c -o $(TARGET) $(CFLAGS)
 
clean:
		$(RM) $(TARGET)
