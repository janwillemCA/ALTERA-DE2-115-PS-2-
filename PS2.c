#include <stdio.h>
#include "includes.h"
#include <altera_up_avalon_ps2.h>
#include <altera_up_ps2_keyboard.h>

/* Definition of Task Stacks */
#define         TASK_STACKSIZE                          2048
OS_STK taskKeyboard_stk                                [TASK_STACKSIZE];

#define         taskKeyboard_PRIORITY                   1

void taskKeyboard(void* pdata)
{
  INT8U err;
  alt_up_ps2_dev *ps2;
  KB_CODE_TYPE *decode_mode;
  alt_u8 buf;
  char ascii;
  char buffer[MAX_BUFFER];

  ps2 = alt_up_ps2_open_dev(PS2_PORT_NAME);
  alt_up_ps2_init(ps2);
  while (1) {
      decode_scancode(ps2,decode_mode,&buf,&ascii);
      if (*decode_mode != 6) {
          if (*decode_mode == 1) {
              //alt_printf("KEY1: %c\n",ascii);
              sprintf(buffer, "Pressed key: %c", ascii);
              displayTextLCD(buffer);
              err = OSQPost(KeyboardQueue, ascii);
            }
        } else {
          OSTimeDlyHMSM(0, 0, 0, 100);
        }
    }
}

/* The main function creates two task and starts multi-tasking */
int main(void)
{
  OSTaskCreateExt(taskKeyboard,         NULL,   (void *)&taskKeyboard_stk[TASK_STACKSIZE-1],            taskKeyboard_PRIORITY,          taskKeyboard_PRIORITY,          taskKeyboard_stk,       TASK_STACKSIZE, NULL,   0);

  OSStart();
  return 0;
}
