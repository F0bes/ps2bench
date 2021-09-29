#include "ee.hpp"
#include <kernel.h>
#include <stdlib.h>
// Random stuff that shouldn't really be used to test perf

// SMC abuser, causes constant recompilation
// Usually an interpreter will be faster

u32 killer_instructions[3] =
	{
		0x018d6025, // or t0,t0,t1
		0x03e00008, // jr ra
		0x00000000, // nop
};

void eemisc_recKiller(void)
{
	u32* memoryPtr = (u32*)malloc(sizeof(killer_instructions) * 2);

	printf("Running the recompiler killer at memory address 0x%0x\n",(u32)memoryPtr);

	while (1)
	{
		// Copy the instructions into memory
		memcpy(memoryPtr, &killer_instructions[0], sizeof(killer_instructions));

		// Flush the instruction cache
		FlushCache(2);

		// Execute the instructions
		asm("jalr %0" ::"r"(memoryPtr));

		memoryPtr++;

		// Do the same things below, but at a different memory offset, forcing the recompiler to trash the previous generated block
		memcpy(memoryPtr, &killer_instructions[0], sizeof(killer_instructions));
		FlushCache(2);
		asm("jalr %0" ::"r"(memoryPtr));

		memoryPtr--;

		if (Pad::readButton(Pad::ButtonState::O))
			return;
	}
}
