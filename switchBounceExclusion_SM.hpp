//switchBounceExclusion_SM.hpp

#ifndef __SWITCH_BOUNCE_EXCLUSION_SM   
#define __SWITCH_BOUNCE_EXCLUSION_SM

#include "variables.h"

#include "FreeRTOS.h"

#include "task.h"
#include "digitalIn.h"


#define TIMER_DFLT_PERIOD 5

extern U8 g_digChCnt;
// This is a synchronous Mealy finite state machine
// The finite state machine must be turned every 0.01  s
// The firnware polls the digital input. External interrupts are not used.


// switchBounceExclusion SM events
// Inputs for the switch Bounce Exclusion state machime

typedef enum
{
 DEBOUNCE_EV_READ_0,    //0 
 DEBOUNCE_EV_READ_1,    //1
 DEBOUNCE_EV_TIME_OUT,  //2
 
 DEBOUNCE_CNT_EVENTS    //3
}enDEBOUNCE_event;


// Hysteresis states of the Hysteresis state machine
typedef enum
{
  DEBOUNCE_ST_DOWN,      //0
  DEBOUNCE_ST_WAITUP,   //1
  DEBOUNCE_ST_UP,        //2
  DEBOUNCE_ST_WAITDONW, //3

  DEBOUNCE_CNT_STATES    //4
}enDEBOUNCE_state;


// DEBOUNCE actions outputs for the DEBOUNCE state machime
typedef enum
{
 DEBOUNCE_ACT_DOWN_0_DOWN,         //0  DOWN_0_DOWN
 DEBOUNCE_ACT_WAITUP_0_DOWN,       //1  WAITUP_0_DOWN
 DEBOUNCE_ACT_UP_0_WAITDOWN,       //2  UP_0_WAITDOWN
 DEBOUNCE_ACT_WAITDOWN_0_WAITDOWN, //3  WAITDOWN_0_WAITDOWN
 DEBOUNCE_ACT_DOWN_1_WAITUP,       //4  DOWN_1_WAITUP
 DEBOUNCE_ACT_WAITUP_1_WAITUP,     //5  WAITUP_1_WAITUP
 DEBOUNCE_ACT_UP_1_UP,             //6  UP_1_UP
 DEBOUNCE_ACT_WAITDOWN_1_UP,       //7  WAITDOWN_1_UP
 DEBOUNCE_ACT_DOWN_TIMO_DOWN,      //8  DOWN_TIMO_DOWN
 DEBOUNCE_ACT_WAITUP_TIMO_UP,      //9  WAITUP_TIMO_UP
 DEBOUNCE_ACT_UP_TIMO_UP,          //10 UP_TIMO_UP
 DEBOUNCE_ACT_WAITDOWN_TIMO_DOWN,  //11 WAITDOWN_TIMO_DOWN


 DEBOUNCE_CNT_ACTIONS   //12
}enDEBOUNCE_action;

#define DEBOUNCE_QNT_EVENTS  (DEBOUNCE_CNT_EVENTS )
#define DEBOUNCE_QNT_STATES  (DEBOUNCE_CNT_STATES )
#define DEBOUNCE_QNT_ACTIONS (DEBOUNCE_CNT_ACTIONS)



class SwitchBounceExclusion
{
  static const enDEBOUNCE_state DEBOUNCE_tableState[DEBOUNCE_QNT_EVENTS][DEBOUNCE_QNT_STATES];
  static const enDEBOUNCE_action DEBOUNCE_tableAction[DEBOUNCE_QNT_EVENTS][DEBOUNCE_QNT_STATES];

  enDEBOUNCE_event m_inEvent;
  enDEBOUNCE_state m_currState;
  enDIGITAL m_digitalChan;
  U8 m_timer;
  U8 m_timerPeriod;
  U8 m_timerFlag;

public:
  I8 m_signal ;
  SwitchBounceExclusion( );
  enDEBOUNCE_event GetEvent();
  void DoAction();
  enDEBOUNCE_state GetState();
  void DoState();
  
  I8 GetSensorVal();

  void Reset();
  void decrementTimer();
};

extern SwitchBounceExclusion DigitalSensor[2];

void DigitalSensorProcess();

#endif // __SWITCH_BOUNCE_EXCLUSION_SM
