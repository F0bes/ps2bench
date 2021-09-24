#pragma once
#include <tamtypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <graph.h>

#include "pad.hpp"

// Benches
void eebench_loopArithmetic(void);
void eebench_SDXLDXFallback(void);
void eebench_smallBlocks(void);
void eebench_dwArithmetic(void);

// Misc
void eemisc_recKiller(void);
