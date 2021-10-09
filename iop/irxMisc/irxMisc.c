#include "irx_imports.h"

IRX_ID("irxMisc", 1, 0);

u32 doSMCTest();

static SifRpcDataQueue_t g_Rpc_Queue __attribute__((aligned(64)));
static SifRpcServerData_t g_Rpc_Server __attribute((aligned(64)));
static u32 g_Rpc_Buffer[1024] __attribute((aligned(64)));

void* rpcCommandHandler(int cmd, void* data, int size)
{
	printf("[IOP] Command handler was issued command %d\n", cmd);
	switch (cmd)
	{
		// Let's call this one SMC
		case 0:
			doSMCTest();
			*(u32*)data = 0xb00b2;
			return data;
			break;
	}
};

void rpcThreadFunc(void* unused)
{
	printf("[IOP] rpcThreadFunc() begin\n");
	SifInitRpc(0);
	SifSetRpcQueue(&g_Rpc_Queue, GetThreadId());

	printf("[IOP] Registering the Rpc server\n");
	SifRegisterRpc(&g_Rpc_Server, 0xB00BA, (void*)rpcCommandHandler, (u32*)&g_Rpc_Buffer, 0, 0, &g_Rpc_Queue);

	printf("[IOP] spinning our thread\n");
	SifRpcLoop(&g_Rpc_Queue);
	// Our thread should be yielding, waiting for an rpc command
}


s32 _start()
{
	printf("[IOP] IRXMisc _start() running!\n");

	iop_thread_t rpcThread;
	rpcThread.attr = TH_C;
	rpcThread.thread = &rpcThreadFunc;
	rpcThread.priority = 40;
	rpcThread.stacksize = 0x1600;

	u32 rpcThreadID = CreateThread(&rpcThread);
	if (rpcThreadID > 0)
	{
		printf("[iop] IOPLink thread created (%d)\n", rpcThreadID);
		StartThread(rpcThreadID, NULL);
		return 0;
	}

	printf("[IOP] I'm done :)\n");
	return 0;
}

/* SMC tester */

const u32 instructionBlock1[5] =
	{
		0x34080064, // li $t0, 100
		0x21090032, // addi $t1, $t0, 50
		0x03e00008, // jr $ra
		0x00000000, // nop
};

const u32 instructionBlock2[4] =
	{
		0x34080064, // li $t0, 100
		0x2109ffce, // addi $t1, $t0, -50
		0x03e00008, // jr $ra
		0x00000000, // nop
};
u32 get_pc()
{
	asm("move $v0, $ra\n"
		"jr $ra\n");
	return 0;
}
u32 ourFunction[5];
u32 doSMCTest()
{
	printf("[IOP] doSMCTest() Hello!\n");

	u32* memory = &ourFunction[0];
	printf("Our memory pointer is at %0x\n", memory);
	printf("we are at %0x", get_pc());
	for (int i = 0; i < 4; i++)
	{
		printf("[IOP] {%d} Writing %0x\n", i, instructionBlock1[i]);
		memory[i] = instructionBlock1[i];
	}

	asm("jalr %0\n" ::"r"(memory)
		: "$t0", "$t1");


	for (int i = 0; i < 4; i++)
	{
		printf("[IOP] {%d} Writing %0x\n", i, instructionBlock2[i]);
		memory[i] = instructionBlock2[i];
	}

	asm("jalr %0\n" ::"r"(memory)
		: "$t0", "$t1");

	asm goto("li $t0, 50\n"
			 "beq $t0, $t1, %0\n"
			 :
			 :
			 : "$t0"
			 : valid);

	printf("[IOP] Test was not successful!\n");
	return 0;

valid:
	printf("[IOP] Test was successful!\n");
	return 1;
}
