#include "ee.hpp"
// Stresses the JIT by creating a bunch of small blocks

// This heavily increases compile time, maybe I'll generate this at runtime instead

// Currently there are 2158 for loops and I get around 120 max on 
// my i7-9750h, linux 64 bit PCSX2 release mode with LTO

#define smallLoop(x) \
for(int x = 0; x < 1; x++) \
	volatileValue = !volatileValue;

#define smallLoops \
smallLoop(a){smallLoop(b){smallLoop(c){smallLoop(d){smallLoop(e){smallLoop(f){	\
smallLoop(g){smallLoop(h){smallLoop(i){smallLoop(j){smallLoop(k){smallLoop(l){	\
smallLoop(m){smallLoop(n){smallLoop(o){smallLoop(p){smallLoop(q){smallLoop(r){	\
smallLoop(s){smallLoop(t){smallLoop(u){smallLoop(v){smallLoop(w){smallLoop(x){	\
smallLoop(y){smallLoop(z){}}}}}}}}}}}}}}}}}}}}}}}}}}

void eebench_smallBlocks(void)
{
	printf("Starting the EE smallblocks test\n");

	// Write to a volatile variable so the compiler wont optimize us out and
	// to give the EE some work 
	volatile u32 volatileValue = 0;

	while(1)
	{
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops smallLoops
		if(pad_do_i_leave())
			return;
	}
}
