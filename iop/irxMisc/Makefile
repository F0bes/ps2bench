IOP_BIN = irxMisc.irx

IOP_OBJS = irxMisc.o imports.o

all: $(IOP_BIN)

clean:
	rm -f $(IOP_BIN) $(IOP_OBJS)
	rmdir obj

reset:
	ps2client reset

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal