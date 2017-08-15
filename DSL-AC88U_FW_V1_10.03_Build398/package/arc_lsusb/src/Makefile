BIN_NAME:=$(TARGET)
MAIN_SRC = lsusb.c

all: $(BIN_NAME)

$(BIN_NAME):
	$(CC) $(CFLAGS) $(LDFLAGS) -DVENDOR_ID=$(VID) -o $(BIN_NAME) $(MAIN_SRC) -lusb-1.0 -larcgpl
resetUsb:
	$(CC) $(CFLAGS) $(LDFLAGS) -o resetUsb resetUsb.c
	
clean:
	@rm *.o $(BIN_NAME)
