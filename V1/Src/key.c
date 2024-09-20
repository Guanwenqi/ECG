#include "Key.h"
unsigned char state;
unsigned char Cnt;
extern float_t ytran;
extern uint8_t cnty;
//uint8_t j;
extern uint16_t data2[240];
void  Key_Run(void)
{
   switch(state)
   {
   case WAIT1_DOWN:
       Cnt=0;
	   if(Key1_Low)
	     state=CONFIRM1_DOWN;
	   break;
   case CONFIRM1_DOWN:
	     if(Cnt==1)
		    state=Y_TRAN;
		else if(Key1_High)
		     state=WAIT1_DOWN;
		else
		    Cnt++;
		break;
		case Y_TRAN:
			if(cnty<3)
				ytran-=0.2;
			else if(cnty==3)
				ytran=1;
			else if(cnty>3&&cnty<=5)
				ytran+=0.2;
			else
			{
				cnty=0;
				ytran=1;
			}
			cnty++;
		   state=WAIT1_UP;
		   break;
		case WAIT1_UP:
		   Cnt=0;
		      if(Key1_High)
			     state=CONFIRM1_UP;
			  break;
		case CONFIRM1_UP:
	      if(Cnt==1)
		    state=WAIT1_DOWN;
		 else if(Key1_Low)
		     state=WAIT1_UP;
		  else
		    Cnt++;
		  break;	  
			
		default:
		  state=WAIT1_DOWN;
			break;
   }
}
