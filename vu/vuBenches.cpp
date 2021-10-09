#include "vu.hpp"
#include <gsKit.h>

/*
	Runs a loop with a lot of arithmetic, (hopefully) doesn't stall
*/
extern u64 vuStress_Start __attribute__((section(".vudata")));
extern u64 vuStress_End __attribute__((section(".vudata")));

void vubench_generic(void)
{
	printf("Starting VU generic benching\n");
	uploadMicroProgram(0, &vuStress_Start, &vuStress_End, 1, 0);

	u32 vp[4] __attribute__((aligned(128)));

	vp[0] = VIFFLUSHE;
	vp[1] = VIFMSCAL(0);
	vp[2] = VIFNOP;
	vp[3] = VIFFLUSHE;

	s32 c = 0;
	while (1)
	{
		c++;
		VIF1MADR = (u32)&vp[0];
		VIF1QWC = 1;

		FlushCache(0);

		VIF1CHCR = 0x101;

		FlushCache(0);

		while (VIF1CHCR == 0x100)
		{
		};

		waitVU1Finish();

		if (c == 3) // Adjust this if you'd like, too low of a value with kill the mtvu ring buffer
			// too high and you'll have the EE waiting on vsyncs too much and the VU will be bored
		{
			if (Pad::readButton(Pad::ButtonState::O))
			{
				//menu_reset_gsKit();
				return;
			}
			gsKit_vsync_wait();
			c = 0;
		}
	}
}


/*
	Runs a loop that uses a lot of registers
	Creates a bunch of register pressure and can be used to
	compare recompiler register usage methods
*/
extern u64 vuRegisterPressure_Start __attribute__((section(".vudata")));
extern u64 vuRegisterPressure_End __attribute__((section(".vudata")));

void vubench_registerPressure(void)
{
	printf("Starting VU register pressure\n");
	uploadMicroProgram(0, &vuRegisterPressure_Start, &vuRegisterPressure_End, 1, 0);

	u32 vp[4] __attribute__((aligned(128)));

	vp[0] = VIFFLUSHE;
	vp[1] = VIFMSCAL(0);
	vp[2] = VIFNOP;
	vp[3] = VIFFLUSHE;

	s32 c = 0;
	while (1)
	{
		c++;
		VIF1MADR = (u32)&vp[0];
		VIF1QWC = 1;

		FlushCache(0);

		VIF1CHCR = 0x101;

		FlushCache(0);

		while (VIF1CHCR == 0x100)
		{
		};

		waitVU1Finish();

		if (c == 3) // Adjust this if you'd like, too low of a value with kill the mtvu ring buffer
			// too high and you'll have the EE waiting on vsyncs too much and the VU will be bored
		{
			if (Pad::readButton(Pad::ButtonState::O))
			{
			//	menu_reset_gsKit();
				return;
			}
			gsKit_vsync_wait();
			c = 0;
		}
	}
}