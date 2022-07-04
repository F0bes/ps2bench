#include "gs.hpp"
#include <kernel.h>
#include <stdlib.h>
#include <graph.h>
#include <draw.h>
#include <gs_gp.h>
#include <gif_tags.h>
#include <gif_registers.h>
#include <dma.h>
#include <gsKit.h>
#include <packet.h>

void gs_mpps_sprite()
{
	packet_t* packet = packet_init(13, PACKET_NORMAL);
	qword_t* q = packet->data;

	{ // Clear the screen with red
		PACK_GIFTAG(q, GIF_SET_TAG(5, 1, GIF_PRE_ENABLE, GS_SET_PRIM(GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0), GIF_FLG_PACKED, 2),
			GIF_REG_XYZ2 | (GIF_REG_XYZ2 << 4));
		q++;
		// More than 5 of these giant sprites is just too much
		for(int i = 0; i < 5; i++)
		{
		// XYZ2
		q->dw[0] = (u64)((((1000 << 4)) | (((u64)(1000 << 4)) << 32)));
		q->dw[1] = (u64)(1);
		q++;
		// XYZ2
		q->dw[0] = (u64)((((3000 << 4)) | (((u64)(3000 << 4)) << 32)));
		q->dw[1] = (u64)(1);
		q++;
		}
	}

	FlushCache(0);
	while (1)
	{
		dma_channel_send_normal(DMA_CHANNEL_GIF, packet->data, q - packet->data, 0, 0);
		if (Pad::readButton(Pad::ButtonState::O))
			break;
	}
	packet_free(packet);
}

void gs_maxpoly(bool deadly)
{
	packet_t* packet = packet_init(800, PACKET_NORMAL);
	qword_t* q = packet->data;

	{ // Clear the screen with red
		PACK_GIFTAG(q, GIF_SET_TAG(800, 1, GIF_PRE_ENABLE, GS_SET_PRIM(GS_PRIM_POINT, 0, 0, 0, 0, 0, 0, 0, 0), GIF_FLG_PACKED, 1),
			GIF_REG_XYZ2);
		q++;
		for (int i = 0; i < 800; i++)
		{
			// XYZ2
			q->dw[0] = (u64)(((((2304) << 4)) | (((u64)((2050) << 4)) << 32)));
			q->dw[1] = (u64)(1);
			q++;
		}
	}

	FlushCache(0);
	dma_channel_fast_waits(DMA_CHANNEL_GIF);
	if(deadly)
	{
		while (true)
		{
			dma_channel_send_normal(DMA_CHANNEL_GIF, packet->data, q - packet->data, 0, 0);
		}
	}
	else
	{
		while (true)
		{
			dma_channel_send_normal(DMA_CHANNEL_GIF, packet->data, q - packet->data, 0, 0);
			if (Pad::readButton(Pad::ButtonState::O))
				break;
		}
	}
	packet_free(packet);
}
