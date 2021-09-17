# --- Variables
SRCS = PIMain.c DocSignInit.cpp DSEngine.cpp DSHandler.cpp DSSigVal.cpp
OBJS = PIMain.o DocSignInit.o DSEngine.o DSHandler.o DSSigVal.o
PI = DocSign.api

# --- TODO: set the following variables
#KEYPAIR = <path to public/private key pair>
#ENCRYPTEDKEY = <path to encrypted key>

CCFLAGS = $(ACROCCFLAGS) 
INCDIRS = -I$(GTK_DIR1) -I$(GTK_DIR2) -I$(GTK_DIR3) -I$(GTK_DIR4) -I$(GTK_DIR5) -I$(GTK_DIR6) -I$(X11_DIR) -I$(HEADER_DIR_API) -I$(HEADER_DIR_SDK) -I$(HEADER_DIR_ADM)

# --- Linking
$(PI) : $(OBJS)
	@echo "Warning: Plug-in must be enabled to load winthin Acrobat Reader 7.0"
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
#	Plug-ins must be enabled on a platform that is supported
#	by the enabling tools (currently Sparc Solaris).
#	Plug-in can be enabled using the following command
#	makeppi $(PI) $(KEYPAIR) $(ENCRYPTEDKEY)
#	@if [ ! -d $(HOME)/plug_ins ]; then mkdir $(HOME)/plug_ins;fi
#	@cp $(PI) $(HOME)/plug_ins/$(PI)

# --- Compiling
PIMain.o: $(HEADER_DIR_API)/PIMain.c
	$(CC) -c $(INCDIRS) $(CCFLAGS) $(HEADER_DIR_API)/PIMain.c

DocSignInit.o: $(SOURCE_DIR)/DocSignInit.cpp
	$(CC) -c $(INCDIRS) $(CCFLAGS) $(SOURCE_DIR)/DocSignInit.cpp

DSEngine.o: $(SOURCE_DIR)/DSEngine.cpp
	$(CC) -c $(INCDIRS) $(CCFLAGS) $(SOURCE_DIR)/DSEngine.cpp

DSHandler.o: $(SOURCE_DIR)/DSHandler.cpp
	$(CC) -c $(INCDIRS) $(CCFLAGS) $(SOURCE_DIR)/DSHandler.cpp

DSSigVal.o: $(SOURCE_DIR)/DSSigVal.cpp
	$(CC) -c $(INCDIRS) $(CCFLAGS) $(SOURCE_DIR)/DSSigVal.cpp

# --- CleanUp
clean:
	rm -f $(OBJS) $(PI)

