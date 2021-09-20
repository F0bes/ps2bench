#include "ee.hpp"

// Doesn't stress much, just loops, doing some math
void eebench_loopArithmetic(void)
{
	eePrintf("Starting the EE arithmetic test\n");
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

		if (pad_do_i_leave())
			return;
	}
}

// Just a fun little formula I wrote up
// The single word assembly is commented out there
void eebench_dwArithmetic(void)
{
	eePrintf("Starting the EE doubleword arithmetic test\n");
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
			"lw $t3, 0(%0)\n"	// Load dw

			"li $t1, 1\n"		// Find out if the input is odd
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
			
			"or $t2, $t3, $zero\n"	// Copy t3
			"dsll $t3, $t3, 1\n"	// Shift t3 to the left by 1 bit (mul by 2)
			"daddu $t3, $t2, $t3\n" // Add t3 to t2 (itself)
			"ori $t2, $zero, 1\n"	// Add 1
			"daddu $t3, $t3, $t2\n"	
			
			//"addiu $t3, $t3, 1\n"

			"bigArith_end:\n"
			"sd $t3, 0(%0)\n"		// Store dw
			::"r"(&cur_num)
				: "$t0", "$t1", "$t2", "$t3");
		if (pad_do_i_leave())
			return;
		if(cur_num == 1)
		{
			cur_num = iter_num++;
			number_iterations = 0;
		}
	}
}

// Developed to bench: https://github.com/PCSX2/pcsx2/pull/4739
void eebench_SDXLDXFallback(void)
{
	u128 someMemory = (u128)0x123456789abcdef << 64 | (u128)0x123456789abcdef;
	eePrintf("Starting the EE SDL,SDR,LDL,and LDR tests");

	while (1)
	{
		for (int i = 0; i < 1800000; i++)
		{
			// TODO: Dyanmic code generation
			if (pad_do_i_leave())
				return;
			asm(
				"LDL $t0, 0(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 0(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 0(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 0(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 1(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 1(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 1(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 1(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 2(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 2(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 2(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 2(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 3(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 3(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 3(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 3(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 4(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 4(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 4(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 4(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 5(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 5(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 5(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 5(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 6(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 6(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 6(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 6(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 7(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 7(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 7(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 7(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 8(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 8(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 8(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 8(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 9(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 9(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 9(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 9(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 10(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 10(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 10(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 10(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 11(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 11(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 11(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 11(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 12(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 12(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 12(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 12(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 13(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 13(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 13(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 13(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 14(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 14(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 14(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDR $t0, 14(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDL $t0, 15(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"SDL $t0, 15(%0)\n"
				"addi $t1, $zero, 0x100\n"
				"addiu $t2, $t1, 0x200\n"
				"addiu $s0, $t3, 0x200\n"
				"addiu $s1, $s0, 0x400\n"
				"addiu $s2, $s1, 0x500\n"
				"addiu $s3, $s2, 0x600\n"
				"addu $s4, $s3, $t1\n"
				"LDR $t0, 15(%0)\n"
				"SDR $t0, 15(%0)\n" ::"r"(&someMemory)
				: "$t0", "$t1", "$t2", "$t3", "$s0", "$s1", "$s2", "$s3", "$s4");
		}
		graph_wait_vsync();
	}
}