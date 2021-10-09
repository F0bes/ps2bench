#include "vu.hpp"
#include <gsKit.h>
/*
	Uploads many many micro programs, filling the recompiler cache
	Not realistic, and hard to get a good metric on recompilation speed
	But fun because why not
*/
extern u64 mvuIBit __attribute__((section(".vudata")));
extern u64 mvuIBitEnd __attribute__((section(".vudata")));

// This flavour only changes the value of an I bit value
// This recompilation can be stopped by using the I-bit hack on pcsx2
void vumisc_IBitRecompilation(void)
{
	float f = 0.0f;
	s32 c = 0;
	while (1)
	{
		c++;
		f += 1.0f;
		((u32*)&mvuIBit)[0] = f; // Unsure if this will properly change to a float but whatever
			// we don't care about results, just an ibit change
		uploadMicroProgram(0, &mvuIBit, &mvuIBitEnd, 1, 1);
		waitVU1Finish();

		if (c == 50)
		{
			if (Pad::readButton(Pad::ButtonState::O))
			{
		//		menu_reset_gsKit();
				return;
			}
			gsKit_vsync_wait();
			c = 0;
		}
		if (f >= __FLT_MAX__)
			f = 0.0f;
	}
}