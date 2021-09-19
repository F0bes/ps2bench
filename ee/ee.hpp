#pragma once
#include <tamtypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <graph.h>

#include "pad.hpp"
#include "eePrintf.hpp"

// Benches
void eebench_loopArithmetic(void);
void eebench_SDXLDXFallback(void);
void eebench_smallBlocks(void);

// Stupid
void eestupid_recKiller(void);
