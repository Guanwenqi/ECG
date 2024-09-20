#include "Key.h"
unsigned char state2;
unsigned char Cnt2;
extern float_t xtran;
extern uint8_t cntx;
//uint8_t h;
extern uint16_t data2[240];
void  Key2_Run(void)
{
   switch(state2)
   {
   case WAIT2_DOWN:
       Cnt2=0;
	   if(Key2_Low)
	     state2=CONFIRM2_DOWN;
	   break;
   case CONFIRM2_DOWN:
	     if(Cnt2==1)
		    state2=X_TRAN;
		else if(Key2_High)
		     state2=WAIT2_DOWN;
		else
		    Cnt2++;
		break;
		case X_TRAN:
		  if(cntx<3)
				xtran-=0.1;
			else if(cntx==3)
				xtran=1;
			else if(cntx>3&&cntx<6)
				xtran+=0.1;
			else
			{
				cntx=0;
				xtran=1;
			}
			cntx++;
		   state2=WAIT2_UP;
		   break;
		case WAIT2_UP:
		   Cnt2=0;
		      if(Key2_High)
			     state2=CONFIRM2_UP;
			  break;
		case CONFIRM2_UP:
	      if(Cnt2==1)
		    state2=WAIT2_DOWN;
		 else if(Key2_Low)
		     state2=WAIT2_UP;
		  else
		    Cnt2++;
		  break;	  
			
		default:
		  state2=WAIT2_DOWN;
			break;
   }
}