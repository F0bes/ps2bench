#include <tamtypes.h>
#include <kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graph.h>
#include <draw.h>

#include "eePrintf.hpp"
#include "menu.hpp"
#include "pad.hpp"

void print_motd(void)
{
	printf("Oh look, a kitty !\n"
		   "    /\\___/\\		\n"
		   "   /       \		\n"
		   "  l  u   u  l		\n"
		   "--l----*----l--	\n"
		   "   \\   w   /     - You're running version 0.1r2\n"
		   "     ======		\n"
		   "   /       \ __	\n"
		   "   l        l\\ \\	\n"
		   "   l        l/ /	\n"
		   "   l  l l   l /	\n"
		   "   \\ ml lm /_/		\n");
}

s32 main(void)
{
	print_motd();
	printf("[ee] main()\n");
	printf("[ee] Fobesmark 0.1a\n");
	init_pad();
	menu_init();
	printf("[ee] ~main()\n");
	SleepThread();
}
