//switchBounceExclusion_SM.cpp

#include "switchBounceExclusion_SM.hpp"
#include "Utils_API.hpp"

U8 g_digChCnt;
SwitchBounceExclusion DigitalSensor[2];

SwitchBounceExclusion::SwitchBounceExclusion()
{
  m_timerPeriod = TIMER_DFLT_PERIOD;
  m_timer       = m_timerPeriod;
  m_timerFlag   = 0;
  m_currState   = DEBOUNCE_ST_DOWN;
  m_inEvent     = DEBOUNCE_EV_READ_0;
  m_signal      = 0;
  m_digitalChan = (enDIGITAL) g_digChCnt;
  g_digChCnt++;
}

void SwitchBounceExclusion::Reset()
{
  g_digChCnt=0;
  m_timerPeriod = TIMER_DFLT_PERIOD;
  m_timer       = m_timerPeriod;
  m_timerFlag   = 0;
  m_currState   = DEBOUNCE_ST_DOWN;
  m_inEvent     = DEBOUNCE_EV_READ_0;
  m_signal      = 0;
}

enDEBOUNCE_event SwitchBounceExclusion::GetEvent()
{
  if(m_timerFlag)
  {
    m_inEvent=DEBOUNCE_EV_TIME_OUT;
    m_timerFlag=0;
  }
  else
  {
    U8 digitalIn;
    digitalIn= readDigitalIn(m_digitalChan );
  
    if(0==digitalIn)
    {
      m_inEvent=DEBOUNCE_EV_READ_0;
    }
    else if(1==digitalIn)
    {
      m_inEvent=DEBOUNCE_EV_READ_1;
    }
  }
  return m_inEvent;
}




enDEBOUNCE_state SwitchBounceExclusion::GetState()
{
  if ((m_inEvent<DEBOUNCE_CNT_EVENTS) && (m_currState< DEBOUNCE_CNT_STATES))
  {
    m_currState = DEBOUNCE_tableState[m_inEvent][m_currState];
  }
  else
  {
    Reset();
  }
  return m_currState;
}


void SwitchBounceExclusion::DoAction()
{
  enDEBOUNCE_action action;
  
  if ( (m_inEvent<DEBOUNCE_CNT_EVENTS) && (m_currState< DEBOUNCE_CNT_STATES) )
  {
    action = DEBOUNCE_tableAction[m_inEvent][m_currState];
    
    switch(action)
    {
    case DEBOUNCE_ACT_DOWN_0_DOWN: 
    
    break;
    
    case DEBOUNCE_ACT_WAITUP_0_DOWN: 
      m_timer       = m_timerPeriod;
    break;
    
    case DEBOUNCE_ACT_UP_0_WAITDOWN:
      m_timer       = m_timerPeriod;
    break;
    
    case DEBOUNCE_ACT_WAITDOWN_0_WAITDOWN: 
    break;
    
    case DEBOUNCE_ACT_DOWN_1_WAITUP:
      m_timer       = m_timerPeriod;	
    break;
    
    case DEBOUNCE_ACT_WAITUP_1_WAITUP: 
    break;
    
    case DEBOUNCE_ACT_UP_1_UP: 
    break;
    
    case DEBOUNCE_ACT_WAITDOWN_1_UP: 
    break;
    
    case DEBOUNCE_ACT_DOWN_TIMO_DOWN: 
      m_timer  = m_timerPeriod;
      m_signal = 0;
    break;
    
    case DEBOUNCE_ACT_WAITUP_TIMO_UP: 
      m_timer  = m_timerPeriod;
      m_signal = 1;
    break;
    
    case DEBOUNCE_ACT_UP_TIMO_UP: 
      m_timer  = m_timerPeriod;
      m_signal = 1;
    break;
    
    case DEBOUNCE_ACT_WAITDOWN_TIMO_DOWN: 
      m_timer  = m_timerPeriod;
      m_signal = 0;
    break;
    
    default:
    break;
    }
  }
  else
  {
    Reset();
  }
}

I8 SwitchBounceExclusion::GetSensorVal()
{
  return m_signal;
}



void SwitchBounceExclusion::DoState()
{
  //decrement the timer, if it has been launched
  decrementTimer();
}



void SwitchBounceExclusion::decrementTimer()
{
  if( m_timer )
  {
    m_timer--;
    if( !m_timer  )
    {
      // generate input for the reset state machine
      m_inEvent   = DEBOUNCE_EV_TIME_OUT;
      m_timerFlag = 1;
    }
  }
}


const enDEBOUNCE_state SwitchBounceExclusion::DEBOUNCE_tableState[DEBOUNCE_QNT_EVENTS][DEBOUNCE_QNT_STATES] =
{
// 0                     1                     2                      3                                                        
// DEBOUNCE_ST_DOWN      DEBOUNCE_ST_WAIT_UP   DEBOUNCE_ST_UP         DEBOUNCE_ST_WAIT_DONW                          
  { DEBOUNCE_ST_DOWN,    DEBOUNCE_ST_DOWN,     DEBOUNCE_ST_WAITDONW,  DEBOUNCE_ST_WAITDONW  },  // DEBOUNCE_EV_READ_0
  { DEBOUNCE_ST_WAITUP,  DEBOUNCE_ST_WAITUP,   DEBOUNCE_ST_UP,        DEBOUNCE_ST_UP        },  // DEBOUNCE_EV_READ_1
  { DEBOUNCE_ST_DOWN,    DEBOUNCE_ST_UP,       DEBOUNCE_ST_UP,        DEBOUNCE_ST_DOWN      }   // DEBOUNCE_EV_TIME_OUT           
};

const enDEBOUNCE_action SwitchBounceExclusion::DEBOUNCE_tableAction[DEBOUNCE_QNT_EVENTS][DEBOUNCE_QNT_STATES] =
{
// 0                             1                             2                           3          
// DEBOUNCE_ST_DOWN              DEBOUNCE_ST_WAIT_UP           DEBOUNCE_ST_UP              DEBOUNCE_ST_WAIT_DONW            
  { DEBOUNCE_ACT_DOWN_0_DOWN,    DEBOUNCE_ACT_WAITUP_0_DOWN,   DEBOUNCE_ACT_UP_0_WAITDOWN, DEBOUNCE_ACT_WAITDOWN_0_WAITDOWN   }, // DEBOUNCE_EV_READ_0
  { DEBOUNCE_ACT_DOWN_1_WAITUP,  DEBOUNCE_ACT_WAITUP_1_WAITUP, DEBOUNCE_ACT_UP_1_UP,       DEBOUNCE_ACT_WAITDOWN_1_UP         }, // DEBOUNCE_EV_READ_1
  { DEBOUNCE_ACT_DOWN_TIMO_DOWN, DEBOUNCE_ACT_WAITUP_TIMO_UP,  DEBOUNCE_ACT_UP_TIMO_UP,    DEBOUNCE_ACT_WAITDOWN_TIMO_DOWN    }  // DEBOUNCE_EV_TIME_OUT
};


void DigitalSensorProcess()
{

  
  
  DigitalSensor[DSENS1].GetEvent();
  DigitalSensor[DSENS1].DoAction();
  DigitalSensor[DSENS1].GetState();
  DigitalSensor[DSENS1].DoState();
  
  DigitalSensor[DSENS2].GetEvent();
  DigitalSensor[DSENS2].DoAction();
  DigitalSensor[DSENS2].GetState();
  DigitalSensor[DSENS2].DoState();
	


}
