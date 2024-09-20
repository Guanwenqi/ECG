#include "key.h"

float multi=1.5;

enum STATUS{
	WAIT_DOWN,
	CONFIRM_DOWN,
	ADJUST,
	WAIT_UP,
	CONFIRM_UP,
};

unsigned char state1;
unsigned char state2;
unsigned char Cnt1;
unsigned char Cnt2;

void KEY1_RUN(float* width_limit)
{
	switch(state1)
	{
		case WAIT_DOWN:
			Cnt1=0;
			if(KEY1_DOWN)
				state1=CONFIRM_DOWN;
			break;
		
		case CONFIRM_DOWN:
			if(Cnt1==5)
				state1=ADJUST;
			else if(KEY1_UP)
				state1=WAIT_DOWN;
			else
				Cnt1++;
			break;
			
		case ADJUST:
			(*width_limit)/=multi;
			state1=WAIT_UP;
			break;
		
		case WAIT_UP:
			Cnt1=0;
			if(KEY1_UP)
				state1=CONFIRM_UP;
			break;
			
		case CONFIRM_UP:
			if(Cnt1==5)
				state1=WAIT_DOWN;
			else if(KEY1_DOWN)
				state1=WAIT_UP;
			else
				Cnt1++;
			break;
	}
}

void KEY2_RUN(float* width_limit)
{
	switch(state2)
	{
		case WAIT_DOWN:
			Cnt2=0;
			if(KEY2_DOWN)
				state2=CONFIRM_DOWN;
			break;
		
		case CONFIRM_DOWN:
			if(Cnt2==5)
				state2=ADJUST;
			else if(KEY2_UP)
				state2=WAIT_DOWN;
			else
				Cnt2++;
			break;
			
		case ADJUST:
			(*width_limit)*=multi;
			state2=WAIT_UP;
			break;
		
		case WAIT_UP:
			Cnt2=0;
			if(KEY2_UP)
				state2=CONFIRM_UP;
			break;
			
		case CONFIRM_UP:
			if(Cnt2==5)
				state2=WAIT_DOWN;
			else if(KEY2_DOWN)
				state2=WAIT_UP;
			else
				Cnt2++;
			break;
	}
}