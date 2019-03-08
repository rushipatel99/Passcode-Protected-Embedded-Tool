#include <stdio.h>
#include "utilities.h"
#include "io.hpp"
#include "gpio.hpp"
#include "lpc_pwm.hpp"

void light(int lselector, int *limPtr); //interface with light
void buzz(int lselector, int *limPtr); //interface with buzzer
void passFind(int *loopPtr, int *codePtr, int *pass); //get password
int select(int codeEntered, int selector, int *pass); //make selection
int main()
{
    int loopBreak=0;
    int codeEntered=0;
    int selector=0;
    int pass[4]={4,3,2,1}; //set passcode
    int limiter=0;
    LD.setNumber(0);



    while(1)
    {
        loopBreak=0;

        selector=select(codeEntered, selector, pass);

        if((1==codeEntered) && (0!=selector))
        {
            switch(selector)
            {
                case 1:
                    buzz(selector, &limiter);
                    break;
                case 2:
                    light(selector, &limiter);
                    break;
                case 3:
                    buzz(selector, &limiter);
                    light(selector, &limiter);
                    break;
                case 4:
                    light(selector, &limiter);
                    break;
                default:
                    break;
            }
        }
        else if(0==codeEntered)
        {
            LE.off(1);
            LD.setNumber(0);
            limiter=0;
            selector=0;
            buzz(selector, &limiter);
            light(selector, &limiter);
        }
        passFind(&loopBreak, &codeEntered, pass);
    }
  return 0;
}

void light(int lselector, int *limPtr)
{
    int limiter;
    limiter=*limPtr;
    GPIO pin20(P1_20);
    pin20.setAsOutput();
    if(0==lselector)
    {
        pin20.setLow();
    }
    else if ((2==lselector) && (1>limiter))
    {
        pin20.setHigh();
        delay_ms (5000);
        pin20.setLow();
        limiter++;
        *limPtr=limiter;
    }
    else if (((3==lselector) || (4==lselector)) && (2>limiter))
    {
            for(size_t i=0; i<5; i++)
            {
               pin20.setHigh();
               delay_ms(1000);
               pin20.setLow();
            }
            limiter++;
            *limPtr=limiter;
    }

}

void buzz(int lselector, int *limPtr)
{
    int limiter;
    limiter=*limPtr;
    GPIO buzzer28(P1_28);
    buzzer28.setAsOutput();
        if(0==lselector)
        {
            buzzer28.setLow();
        }
        else if ((1==lselector) && (2>limiter))
        {
            for( size_t i=0; i<4; i++)
            {
                buzzer28.setHigh();
                delay_ms(1000);
                buzzer28.setLow();
            }
            limiter++;
            *limPtr=limiter;

        }
        else if ((3==lselector) && (0==limiter))
        {
            buzzer28.setHigh();
            delay_ms(5000);
            buzzer28.setLow();
            limiter++;
            *limPtr=limiter;
        }
}
void passFind(int *loopPtr, int *codePtr, int * pass)
{
        int loopBreak=*loopPtr;
        int codeEntered=*codePtr;
        if(0!=SW.getSwitch(pass[0]))
        {
            while(0!=SW.getSwitch(pass[0])){} //deliberately empty loop
            delay_ms(10);
            while(1)
            {
                if(1==loopBreak)
                {
                    break;
                }
                else if(0!=SW.getSwitch(pass[0]) || 0!=SW.getSwitch(pass[2]) || 0!=SW.getSwitch(pass[3]))
                {
                    loopBreak=1;
                }
                else if(0!=SW.getSwitch(pass[1]))
                {
                    while(0!=SW.getSwitch(pass[1])){} //deliberate loop
                    delay_ms(10);
                    while(1)
                    {
                        if(1==loopBreak)
                        {
                            break;
                        }
                        else if(0!=SW.getSwitch(pass[0]) || 0!=SW.getSwitch(pass[1]) || 0!=SW.getSwitch(pass[3]))
                        {
                        loopBreak=1;
                        }
                        else if(0!=SW.getSwitch(pass[2]))
                        {
                            while(0!=SW.getSwitch(pass[2])){} //deliberately empty loop
                            delay_ms(10);
                            while(1)
                            {
                                if(1==loopBreak)
                                {
                                    break;
                                }
                                else if(0!=SW.getSwitch(pass[0]) || 0!=SW.getSwitch(pass[1]) || 0!=SW.getSwitch(pass[2]))
                                {
                                loopBreak=1;
                                }
                                else if(0!=SW.getSwitch(pass[3]) && 0==codeEntered)
                                {
                                    codeEntered=1;
                                    loopBreak=1;
                                    *codePtr=codeEntered;
                                    LE.on(1);
                                }
                                else if(0!=SW.getSwitch(pass[3]) && 1==codeEntered)
                                {
                                    codeEntered=0;
                                    loopBreak=1;
                                    *codePtr=codeEntered;
                                }
                            }
                        }
                    }
                }
            }
        }
        delay_ms(10);
}
int select(int codeEntered, int selector, int* pass)
{
    if((1==codeEntered) && (0==selector))
    {
       while(0!=SW.getSwitch(pass[3])){}
       while(0==selector)
       {
              if(0!=SW.getSwitch(1))
              {
                  selector=1;
                  LD.setNumber(1);
              }
              else if(0!=SW.getSwitch(2))
              {
                  selector=2;
                  LD.setNumber(2);
              }
              else if(0!=SW.getSwitch(3))
              {
                  selector=3;
                  LD.setNumber(3);
              }
              else if (0!=SW.getSwitch(4))
              {
                  selector=4;
                  LD.setNumber(4);
              }
          }
       }
       return selector;
}
