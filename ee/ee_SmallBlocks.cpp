#include "ee.hpp"
#include <kernel.h>
// Stresses the JIT by creating a bunch of small blocks

#define jit_block_count_est 90000 // Estimated amount of JIT blocks this'll produce
u32 Instructions[]
{
	0x218cffff, // addi $t0, $t0, -1
	0x10000001, // b 0x8
	0x218c0001 // addi $t0, $t0, 1
};

u32 instructionBlock[jit_block_count_est * 3 + 2];
void eebench_smallBlocks(void)
{
	printf("Starting the EE smallblocks test\n");

	for(u32 i = 0; i < 3 * jit_block_count_est; i++)
	{
		instructionBlock[i] = Instructions[i % 3];
	}
	instructionBlock[(jit_block_count_est * 3) - 2] = 0x03e00008; // jr ra
	instructionBlock[(jit_block_count_est * 3) - 1] = 0x00000000; // nop
	FlushCache(0);
	while(1)
	{
		asm("jalr %0"::"r"(&instructionBlock[0]));
		if(Pad::readButton(Pad::ButtonState::O))
			return;
	}
}
