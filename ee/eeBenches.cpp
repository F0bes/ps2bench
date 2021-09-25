#include "ee.hpp"
#include <stdlib.h> // calloc, free
#include <kernel.h> // flushcache
// Doesn't stress much, just loops, doing some math
void eebench_loopArithmetic(void)
{
	printf("Starting the EE arithmetic test\n");
	while (1)
	{
		asm __volatile__(
			"li $t1, 2\n" // Divide by 2
			"addi $t0, $t0, 1\n" // Increment by one
			"or $t2,$zero,$t0\n" // Store current increment
			"div $t2, $t1\n" // Divide by 2
			"or $t3,$t2,$zero\n" // Store result of division
			::
				: "$t0", "$t1", "$t2", "$t3");

		if (Pad::readButton(Pad::ButtonState::O))
			return;
	}
}

// Just a fun little formula I wrote up
// The single word assembly is commented out there
void eebench_dwArithmetic(void)
{
	printf("Starting the EE doubleword arithmetic test\n");
	// Let's do some fun with math
	// F(x)
	// if x is odd, 3x+1 else,x/2
	// This will always reach the number 1
	u64 iter_num = 2; // Start at 2
	u64 cur_num __attribute__((aligned(32)));
	cur_num = iter_num;
	u64 number_iterations = 0;

	while (1)
	{
		number_iterations++;
		asm __volatile__(

			//"ld $t3, 0(%0)\n"
			"lw $t3, 0(%0)\n" // Load dw

			"li $t1, 1\n" // Find out if the input is odd
			"and $t2, $t3, $t1\n"
			// t2 is 1 if odd, 0 is even

			"bgtz $t2, bigArith_odd\n"
			"nop\n"

			// t2 is even

			"dsrl $t3, $t3, 1\n" // Shift right by 1 bit (div by 2)
			//"li $t0, 2\n"
			//"divu $t3, $t3, $t0\n"

			"b bigArith_end\n"

			"bigArith_odd:\n"
			// t2 is odd
			//"li $t0, 3\n"
			//"multu $t3, $t3, $t0\n"

			"or $t2, $t3, $zero\n" // Copy t3
			"dsll $t3, $t3, 1\n" // Shift t3 to the left by 1 bit (mul by 2)
			"daddu $t3, $t2, $t3\n" // Add t3 to t2 (itself)
			"ori $t2, $zero, 1\n" // Add 1
			"daddu $t3, $t3, $t2\n"

			//"addiu $t3, $t3, 1\n"

			"bigArith_end:\n"
			"sd $t3, 0(%0)\n" // Store dw
			::"r"(&cur_num)
			: "$t0", "$t1", "$t2", "$t3");
		if (Pad::readButton(Pad::ButtonState::O))
			return;
		if (cur_num == 1)
		{
			cur_num = iter_num++;
			number_iterations = 0;
		}
	}
}

u32 SDXLDXJunkOpcodes[7]
{
	0x200d0100, // addi $t1, $zero, 0x100
	0x25ae0200, // addiu $t2, $t1, 0x200
	0x25f00200, // addiu $s0, $t3, 0x200
	0x26110400, // addiu $s1, $s0, 0x400
	0x26320500, // addiu $s2, $s1, 0x500
	0x26530600, // addiu $s3, $s2, 0x600
	0x026da021, // addu $s4, $s3, $t1
};

u32 SDXLDXOpcodes[4]
{
	0x684c0000,
	0xb04c0000,
	0x6c4c0000,
	0xb44c0000,
};

// Developed to bench: https://github.com/PCSX2/pcsx2/pull/4739
// Now with fancy dynacode generation :)
void eebench_SDXLDXFallback(void)
{
	u128 someMemory = (u128)0x123456789abcdef << 64 | (u128)0x123456789abcdef;
	printf("Starting the EE SDL,SDR,LDL,and LDR tests\n");

	u32* our_func = (u32*)calloc(sizeof(u32) * ((4 * 0xF * 7) + 2),32); 

	u32 fi = 0; // Function iter
	for(int i = 0; i < 0xF; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			our_func[fi++] = SDXLDXOpcodes[j] + i;
			for(int k = 0; k < 7; k++)
			{
				our_func[fi++] = SDXLDXJunkOpcodes[k];
			}
		}
	}
	our_func[fi++] = 0x03e00008; // jr $ra
	our_func[fi++] = 0x0;

	printf("Our dynamically generated function is at 0x%0x. We have %d instructions there\n",&our_func[0],fi);
	FlushCache(0);

	while (1)
	{
		if (Pad::readButton(Pad::ButtonState::O))
		{
			free(our_func);
			return;
		}
		
		asm("jalr %0" ::"r"(&our_func[0]));
	}
}