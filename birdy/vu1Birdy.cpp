#include "birdy.hpp"
#include <gif_tags.h>

extern u64 VU1BirdyInit __attribute__((section(".vudata")));
extern u64 VU1BirdyInitEnd __attribute__((section(".vudata")));
extern u64 VU1BirdyFrame __attribute__((section(".vudata")));
extern u64 VU1BirdyFrameEnd __attribute__((section(".vudata")));

void Birdy::VU1::Initialize()
{
	u32 vp[255] __attribute__((aligned(128))), vpi = 0;

	vp[vpi++] = VIFFLUSHE;

	qword_t gifPacket[255] __attribute__((aligned(128)));
	qword_t* q = &gifPacket[0];
	{
		q->sw[0] = 0x44ed8000; // Start coord of X (1900)
		q->sw[1] = 0x450c8000; // Start coord of y (2148)
		q->sw[2] = 0x0; // Z is not preserved in the micro program!!!
		q++;

		PACK_GIFTAG(q, GIF_SET_TAG(1, 1, GIF_PRE_ENABLE, GIF_PRIM_TRIANGLE, GIF_FLG_PACKED, 4),
			GIF_REG_RGBAQ | (GIF_REG_XYZ2 << 4) | (GIF_REG_XYZ2 << 8) | (GIF_REG_XYZ2 << 12));
		q++;
		//RGBAQ (Q will default to 1.0f on new tag)
		q->dw[0] = (u64)((0xcc) | ((u64)0x00 << 32));
		q->dw[1] = (u64)((0x00) | ((u64)0x80 << 32));

		q++;
		gifPacket[0].sw[3] = ((((u32)q - (u32)&gifPacket[0]) / 16)); // Store the XYZ2 offset for the VU init program
		/* Alloc space for our coordinates */
		q += 3;

		// Finish tag
		PACK_GIFTAG(q, GIF_SET_TAG(1, 1, 0, 0, GIF_FLG_PACKED, 1), GIF_REG_AD);
		q++;
		PACK_GIFTAG(q, 1, 0x61); // GS_REG_FINISH
		q++;
	}

	// Set up our UNPACK
	u32 giftag_qwords = ((u32)q - (u32)&gifPacket[0]) / 16;
	vp[vpi++] = VIFUNPACK(0b01100, (u32)giftag_qwords, 0, 1, 0);
	for (u32 i = 0; i < giftag_qwords; i++)
	{
		vp[vpi++] = gifPacket[i].sw[0];
		vp[vpi++] = gifPacket[i].sw[1];
		vp[vpi++] = gifPacket[i].sw[2];
		vp[vpi++] = gifPacket[i].sw[3];
	}

	s32 needPad = vpi % 4;
	for (; needPad > 0; needPad--)
		vp[vpi++] = VIFNOP;

	VIF1MADR = (u32)&vp;
	VIF1QWC = (vpi / 4); // qwd in vif packet
	FlushCache(0);
	VIF1CHCR = 0x101;
	FlushCache(0);
	while (VIF1CHCR & 0x100)
	{
	}

	// Once that's done, call the init micro program

	uploadMicroProgram(0, &VU1BirdyInit, &VU1BirdyInitEnd, 1, 1);
	waitVU1Finish();

	uploadMicroProgram(0, &VU1BirdyFrame, &VU1BirdyFrameEnd, 1, 1);
}

void Birdy::VU1::Frame()
{
	u32 execPacket[4];
	execPacket[0] = VIFFLUSHE;
	execPacket[1] = VIFMSCAL(0);
	execPacket[0] = VIFFLUSHE;
	execPacket[3] = VIFNOP;

	VIF1MADR = (u32)&execPacket[0];
	VIF1QWC = 1;
	FlushCache(0);
	VIF1CHCR = 0x101;
	FlushCache(0);
	while (VIF1CHCR & 0x100)
	{
	}

	waitVU1Finish();
}

// Todo: not rely on gsKit?
// I mean, I expose the Frame() function already right?
// Also, tests show that the menu doesn't affect perf _at all_
// so this is actually kind of useless, because you'll get the same number in the menu.
// But I'll keep it.
void Birdy::VU1::Bench(gsGlobal* gsGlobal)
{
	Initialize();
	gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
	gsKit_clear(gsGlobal, 0x0);
	while (!Pad::readButton(Pad::ButtonState::O))
	{
		gsKit_queue_exec(gsGlobal);
		Frame();
		gsKit_sync_flip(gsGlobal);
	}
	gsKit_mode_switch(gsGlobal, GS_ONESHOT); // Assume that gsKit was in oneshot mode when we start ?
}
