# build an executable for UsbMX

CC = g++
CFLAGS = -g -Wall -I../../include

HDIR = ../../include
SDIR = ../../src
ODIR = ../../src/obj

UMX = UsbMX
TARGET = wheel

all: $(TARGET)

$(TARGET): $(TARGET).o $(UMX).o
	$(CC) $(CFLAGS) $< $(ODIR)/$(UMX).o -o $@
		
$(TARGET).o: $(TARGET).cpp
	$(CC) $(CFLAGS) -c $< -o $@
	
$(UMX).o: $(SDIR)/$(UMX).cpp $(HDIR)/$(UMX).h
	$(CC) $(CFLAGS) -c $< -o $(ODIR)/$@


target: $(TARGET)

$(TARGET): $(TARGET).o $(UMX).o
	$(CC) $(CFLAGS) $< $(ODIR)/$(UMX).o -o $@

clean:
	$(RM) -f core *.o $(TARGET)

cleanall:
		$(RM) -f core *.o $(ODIR)/*.o $(TARGET) $(SDIR)/*.cpp~ *.cpp~ $(HDIR)/*.h~ 
