#ifndef __STATE_H
#define __STATE_H

typedef enum {IDLE_STATE, CHECKING_STATE, TRANSMIT_STATE} StateType;
#define TIME_TO_IDLE 5

#define str_on  "$1,ON"
#define str_off  "$1,OFF"
#define str_bat "$1BAT,"

#endif
