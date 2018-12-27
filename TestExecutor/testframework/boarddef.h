/*
 * myDef.h
 *
 *  Created on: 03.11.2018
 *      Author: nicholas
 */

#ifndef BOARDDEF_H_
#define BOARDDEF_H_

#define onBoard 1 //uncommend if used on board

#ifdef onBoard
#define _malloc pvPortMalloc
#define _free vPortFree
#include <../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h>
#else
#define _malloc malloc
#define _free free

#endif

#endif /* MYDEF_H_ */
