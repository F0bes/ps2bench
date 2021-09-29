#include "vu.hpp"

void uploadMicroProgram(const u32 offset, const u64* start, const u64* end, u32 vu1, u32 hush)
{
	u32 vp[0xFFF] __attribute__((aligned(128)));
	u32 vpi = 0;

	u32 progInstructions = (end - start);
	if (progInstructions + offset > 512 && !vu1) // oopsiess
		printf("[ee] !!!!!!!!!! YOUR LAST BLOCK IS GOING TO WRAP TO 0 DUDE\n	!!!!!!!!!! VU0 CAN ONLY HANDLE SO MUCH :(\n");
	// TODO: Check for VU1 code overflow? Eh, I can't write that much assembly and stay sane so it'll probably be useless

	u32 mpgBlocks = 0;
	vp[vpi++] = VIFFLUSHE;
	for (u32 instruction = 0; instruction < progInstructions; instruction++)
	{
		if (!(instruction % 256))
		{
			u32 size = progInstructions - (mpgBlocks * 256); // Get the remaining amount of instructions to send
			if (size >= 256) // If the remaining instructions to be sent to VIF is >= 256
				size = 0; // Then send the maximum amount in this mpg (0, but is interpreted as 256)

			// If we are in our first mpg block then set the offset to be zero
			vp[vpi++] = VIFMPG((size + offset), (mpgBlocks == 0 ? 0 : (mpgBlocks * 256)));

			if (!hush)
				printf("[ee] VU%d MPG BLOCK\n	Offset is %d * 8\n	Size is %d 64 bit units. (0 = 256)\n", vu1, (mpgBlocks == 0 ? 0 : (mpgBlocks * 256)), size);

			mpgBlocks++;
		}
		vp[vpi++] = start[instruction];
		vp[vpi++] = start[instruction] >> 32;
	}

	vp[vpi++] = VIFMSCAL(0); // Call the micro program, forces recompilation
	vp[vpi++] = VIFFLUSHE;

	while (vpi % 4) // Align the packet
		vp[vpi++] = VIFNOP;

	if (vu1)
	{
		VIF1MADR = (u32)&vp[0];
		VIF1QWC = vpi / 4;
	}
	else
	{
		VIF0MADR = (u32)&vp[0];
		VIF0QWC = vpi / 4;
	}

	FlushCache(0);

	if (vu1)
		VIF1CHCR = 0x101;
	else
		VIF0CHCR = 0x101;

	FlushCache(0);

	while (vu1 ? VIF1CHCR == 0x100 : VIF0CHCR == 0x100)
	{
	}

	return;
}

void waitVU0Finish(void)
{
	asm(
		"vu0_wait%=:\n"
		"cfc2 $t0, $vi29\n" // VPU-STAT
		"andi $t0,$t0,1\n"
		"bgtz $t0, vu0_wait%=\n" ::
			: "$t0");
}

void waitVU1Finish(void)
{
	asm(
		"vu1_active_%=:\n"
		"bc2t vu1_active_%=\n"
		"nop\n"
		:);
}