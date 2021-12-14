

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "common.h"

typedef uint32_t ticktime_t;

void Init_Systick();
ticktime_t Now();
void Reset_Timer();
ticktime_t Get_Timer();

#endif /* SYSTICK_H_ */
