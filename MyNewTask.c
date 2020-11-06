/*
 * MyNewTask.c
 *
 *  Created on: 27 oct. 2020
 *      Author: Marco Antonio Ramírez Zepeda IE708370
 */

#include "MyNewTask.h"

void My_Task( osaTaskParam_t argument );
static void myTaskTimerCallback( void *param );

osaEventId_t mMyEvents;

/*Global variable to store our TimerID*/
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;

/*Handler ID for task*/
osaTaskId_t gMyTaskHandlerID = 0;

/*Local variable to store the current state of the LEDs*/
static ledStates_e ledsState = RED;

/*Local variable to know if the device has been connected, if not, dont allow LED changes */
uint8_t connection_flag = 0;

/*OSA Task Definition*/
OSA_TASK_DEFINE(My_Task, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE);

/*Main custom Task*/
void My_Task( osaTaskParam_t argument )
{
	osaEventFlags_t customEvent;
	myTimerID = TMR_AllocateTimer();

	while(1)
	{
		OSA_EventWait( mMyEvents, osaEventFlagsAll_c, FALSE, osaWaitForever_c, &customEvent );

		if( !gUseRtos_c && !customEvent )
		{
			break;
		}

		/*Depending on the received event */
		switch(customEvent)
		{
			case gMyNewTaskEvent1_c:  //Start timer to count 3 seconds and change the LEDs
				TMR_StartIntervalTimer(myTimerID, /*myTimerID*/3000, /* Timer's Timeout */myTaskTimerCallback, /* pointer to myTaskTimerCallback function */NULL);
				ledsState = BLACK;
				updateLED( ledsState );
				connection_flag = 1;
				break;

			case gMyNewTaskEvent2_c: /* Event called from myTaskTimerCallback */
				/* If counter reaches end of states, reset to RED, the first value and update LEDs */
				ledsState++;
				ledsState = ( ledsState >= BLACK ) ? RED : ledsState;
				updateLED( ledsState );
				break;

			case gMyNewTaskEvent3_c: /* Event to stop the timer */
				ledsState = BLACK;
				updateLED( ledsState );
				TMR_StopTimer(myTimerID);
				break;

			default:
				break;
		}
	}
}

/* Function to init the task */
void MyTask_Init(void)
{
	mMyEvents = OSA_EventCreate(TRUE);/* The instance of the MAC is passed at task creaton*/
	gMyTaskHandlerID = OSA_TaskCreate(OSA_TASK(My_Task), NULL);
}

/* Public function to send an event to start the timer */
void MyTaskTimer_Start(void)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent1_c);
}

/* This is the function called by the Timer each time it expires */
static void myTaskTimerCallback( void *param )
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);
}

/* Public function to send an event to stop the timer */
void MyTaskTimer_Stop(void)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent3_c);
}

/* Update the LED colors on the board */
void updateLED( ledStates_e state )
{
	switch(state)
	{
		case RED:
			TurnOffLeds();
			Led2On();/*Led1On(); */
		break;

		case GREEN:
			TurnOffLeds();
			Led3On();
		break;

		case BLUE:
			TurnOffLeds();
			Led4On();
		break;

		case WHITE://Turn on all LEDs
			 Led2On();
			 Led3On();
			 Led4On();
		break;

		default://BLACK
			TurnOffLeds();
		break;

	}
}

/* Send the counter to the mwa_end_device file */
ledStates_e returnLEDCounter(void)
{
	return ledsState;
}

/* Update the counter, the LED color and reset the timer, this function is called anytime SW3 or SW4 is pressed*/
void updateLEDCounter(ledStates_e counter)
{
	if(connection_flag)//only if the device is connected, allow it to change the colors
	{
		ledsState = counter;
		updateLED( ledsState );
		TMR_StartIntervalTimer(myTimerID, /*myTimerID*/1000, /* Timer's Timeout */myTaskTimerCallback, /* pointer to myTaskTimerCallback function */NULL);
	}
	return;
}












