CC = gcc
TARGET = main 
OBJS = main.o utils.o

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) -c $< -o $@
-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) $(OBJS:.o=.d)
