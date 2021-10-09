#include "iop.hpp"

#include "irx/irxMisc.h"

#include <loadfile.h>
#include <stdio.h>
#include <kernel.h>
#include <sifrpc.h>

void IOP::Misc::SMC()
{
	// So, pad should have enabled the load module buffer patch
	u32 irxMiscID;
	if ((irxMiscID = SifExecModuleBuffer(irxMisc, size_irxMisc, 0, NULL, NULL)) < 0)
	{
		printf("Failed to load irxMisc module (%d)\n", irxMiscID);
		SleepThread();
	}

	SifRpcClientData_t rpcClient __attribute((aligned(128)));
	u32 count;
	while (1)
	{
		SifBindRpc(&rpcClient, 0xB00BA, 0);
		if (rpcClient.server != NULL)
			break;
		if (count++ == 1000000)
		{
			printf("[EE] Waiting for RPC server\n");
			count = 0;
		}
	}

	u32 value __attribute__((aligned(128))) = 0;
	SifCallRpc(&rpcClient, 0, 0, &value, 0, &value, 32, NULL, NULL);

	printf("[EE] RPC function returned %0x\n", value);
	// Be good and unload :)
	SifUnloadModule(irxMiscID);
	SleepThread();
}