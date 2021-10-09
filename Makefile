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
# IOP stuff
EE_OBJS += iop/iopMisc.o
EE_BIN = ps2bench.elf
EE_INCS = -I$(GSKIT)/include
EE_LIBS = -lkernel -lgraph -L$(GSKIT)/lib -lgskit -ldmakit -lpad -lpatches -lstdc++
EE_DVP = dvp-as
EE_VCL = vcl

all: $(EE_BIN) 
	make -C iop/irxMisc
	cp iop/irxMisc/irxMisc.irx ./irx 
	bin2c irx/padman.irx irx/padman.h padman
	bin2c irx/sio2man.irx irx/sio2man.h sio2man
	bin2c irx/irxMisc.irx irx/irxMisc.h irxMisc

%.o: %.vsm
	$(EE_DVP) $< -o $@

clean:
	rm -f $(EE_BIN) $(EE_OBJS)
	make -C iop/irxMisc clean

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

emu: $(EE_BIN)
	~/PCSX2/bin/PCSX2 --elf="$(shell pwd)/$(EE_BIN)"

reset:
	ps2client reset
	ps2client netdump

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
