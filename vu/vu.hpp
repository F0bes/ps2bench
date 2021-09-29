#pragma once

#include <tamtypes.h>
#include <kernel.h>

#include "pad.hpp"

#include "menu.hpp"

#include "MemoryDefines.hpp"

// Benches
void vubench_generic(void);
void vubench_registerPressure(void);

// Stupid
void vumisc_IBitRecompilation(void);


/* Definitions used for VIF packet generation */
#define VIFNOP 0x00000000

#define VIFMPG(size, pos) ((0x4A000000) | (size << 16) | pos)

#define VIFFLUSHE 0x10000000
#define VIFFLUSH 0x11000000
#define VIFFLUSHA 0x13000000
#define VIFMSCAL(execaddr) (0x14000000 | execaddr)
#define VIFMSCNT 0x17000000
#define VIFFMSCALF(execaddr) (0x15000000 | execaddr)

#define VIFUNPACK(mode, size, flg, usn, addr) ((0b011 << 29) | (mode << 24) | (size << 16) | (flg << 15) | (usn << 14) | addr)


/* Helper functions for vpu interfacing */

// Uploads a micro program to offset 0 and executes it
// Micro program might need to be aligned, idk
void uploadMicroProgram(const u32 offset, const u64* start, const u64* end, u32 vu1, u32 hush);

void waitVU0Finish(void);

void waitVU1Finish(void);