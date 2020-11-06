/*
 * MyNewTask.h
 *
 *  Created on: 27 oct. 2020
 *      Author: marco
 */

#ifndef MYNEWTASK_H_
#define MYNEWTASK_H_

/* Fwk*/
#include"TimersManager.h"
#include"FunctionLib.h"
#include"LED.h"

/* KSDK */
#include"fsl_common.h"
#include"EmbeddedTypes.h"
#include"fsl_os_abstraction.h"

/* Define the available Task's Events */
#define gMyNewTaskEvent1_c (1 << 0)
#define gMyNewTaskEvent2_c (1 << 1)
#define gMyNewTaskEvent3_c (1 << 2)

#define gMyTaskPriority_c 3
#define gMyTaskStackSize_c 400

typedef enum{
	RED = 0,
	GREEN = 1,
	BLUE = 2,
	WHITE = 3,
	BLACK = 4
}ledStates_e;

void MyTaskTimer_Start(void);
void MyTaskTimer_Stop(void);
void MyTask_Init(void);
void updateLED( ledStates_e state );
ledStates_e returnLEDCounter(void);
void updateLEDCounter(ledStates_e counter);

#endif /* MYNEWTASK_H_ */
