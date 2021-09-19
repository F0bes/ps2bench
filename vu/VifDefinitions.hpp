// Don't worry about the interrupt (i) bit >:)

// NO-OP used for alignment
#define VIFNOP 0x00000000
// Transfers the microprogram, size is in 64 bit units (128 bits = 2) defaults to 256
#define VIFMPG(size, pos) ((0x4A000000) | (size << 16) | pos)
// Wait for micro program (vu0 / vu1) to end
#define VIFFLUSHE 0x10000000
// Waits for micro program and GIF transfers to end
#define VIFFLUSH 0x11000000
// Waits for micro program and GIF P1,P2,P3 transfers to end
#define VIFFLUSHA 0x13000000
// Activates the miro program at specified address
#define VIFMSCAL(execaddr) (0x14000000 | execaddr)
// Activates the micro program
#define VIFMSCNT 0x17000000
// Activates the micro program, waits for GIF P1 & P2 before doing so
#define VIFFMSCALF(execaddr) (0x15000000 | execaddr)

#define VIFUNPACK(mode, size, flg, usn, addr) ((0b011 << 29) | (mode << 24) | (size << 16) | (flg << 15) | (usn << 14) | addr)
