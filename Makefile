# Main stuff
EE_OBJS = main.o pad.o menu.o menuDraw.o
# EE stuff
EE_OBJS += ee/eeBenches.o ee/ee_SmallBlocks.o ee/eeMisc.o
# Micro Program stuff
EE_OBJS += vu/microprograms/ibit.o vu/microprograms/vuRegisterPressure.o vu/microprograms/vuStress.o vu/microprograms/vu1Birdy.o
# VU stuff
EE_OBJS += vu/vuBenches.o vu/vuMisc.o vu/vu.o
# Birdy stuff
EE_OBJS += birdy/vu1Birdy.o
# GS stuff
EE_OBJS += gs/gs.o
# Module stuff 
EE_OBJS += padman.o sio2man.o

EE_BIN = ps2bench.elf
EE_INCS = -I$(GSKIT)/include
EE_LIBS = -lkernel -lgraph -L$(GSKIT)/lib -lgskit -ldmakit -lpad -lpatches -lstdc++ -ldma -lpacket
EE_DVP = dvp-as
EE_VCL = vcl

BIN2C = $(PS2SDK)/bin/bin2c

all: $(EE_BIN)

%.o: %.vsm
	$(EE_DVP) $< -o $@

padman.c: $(PS2SDK)/iop/irx/padman.irx
	$(BIN2C) $< $@ padman_irx

sio2man.c: $(PS2SDK)/iop/irx/sio2man.irx
	$(BIN2C) $< $@ sio2man_irx

clean:
	rm -f $(EE_BIN) $(EE_OBJS) 

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

emu: $(EE_BIN)
	$(PCSX2) --elf="$(shell pwd)/$(EE_BIN)"

wsl: $(EE_BIN)
	$(PCSX2) --elf="$(shell wslpath -w $(shell pwd))/$(EE_BIN)"


reset:
	ps2client reset
	ps2client netdump

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
