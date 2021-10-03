#include <tamtypes.h>
#include <kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graph.h>
#include <draw.h>

#include "menu.hpp"
#include "pad.hpp"
#include "birdy/birdy.hpp"

void print_motd(void)
{
	printf(R"ART(
			     ,                                                                                                            
     Et           :                                                                                               
     E#t         t#,                      ,;         .                                                    G:      
     E##t       ;##W.   .               f#i         ;W                                         j.         E#,    :
     E#W#t     :#L:WE   Ef.           .E#t         f#E                 ..       :           .. EW,        E#t  .GE
     E#tfL.   .KG  ,#D  E#Wi         i#W,        .E#f                 ,W,     .Et          ;W, E##j       E#t j#K;
     E#t      EE    ;#f E#K#D:      L#D.        iWW;                 t##,    ,W#t         j##, E###D.     E#GK#f  
  ,ffW#Dffj. f#.     t#iE#t,E#f.  :K#Wfff;     L##Lffi              L###,   j###t        G###, E#jG#W;    E##D.   
   ;LW#ELLLf.:#G     GK E#WEE##Wt i##WLLLLt   tLLG##L             .E#j##,  G#fE#t      :E####, E#t t##f   E##Wi   
     E#t      ;#L   LW. E##Ei;;;;. .E#L         ,W#i             ;WW; ##,:K#i E#t     ;W#DG##, E#t  :K#E: E#jL#D: 
     E#t       t#f f#:  E#DWWt       f#E:      j#E.             j#E.  ##f#W,  E#t    j###DW##, E#KDDDD###iE#t ,K#j
     E#t        f#D#;   E#t f#K;      ,WW;   .D#j             .D#L    ###K:   E#t   G##i,,G##, E#f,t#Wi,,,E#t   jD
     E#t         G#t    E#Dfff##E,     .D#; ,WK,             :K#t     ##D.    E#t :K#K:   L##, E#t  ;#W:  j#t     
     E#t          t     jLLLLLLLLL;      tt EG.              ...      #G      .. ;##D.    L##, DWi   ,KK:  ,;     
     ;#t                                    ,                         j          ,,,      .,,                     
      :;                                                                                                          
			)ART");
}

s32 main(void)
{
	print_motd();

	Pad::init();
	Menu::Load();
	Menu::InitGS();
	
	Menu::Loop();

	printf("[ee] ~main()\n");
	SleepThread();
}
