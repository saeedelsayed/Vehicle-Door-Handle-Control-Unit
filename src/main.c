#include "timer.h"
#include "Gpio.h"
#include "Rcc.h"

#define LED_ONE 0
#define LED_TWO 1
#define LED_THREE 2

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 762

void Delay_MS(unsigned long long n)
{
  volatile unsigned long long count = 0;
  while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

int main()
{
  
  Rcc_Init();
  Rcc_Enable(RCC_GPIOA);
  Rcc_Enable(RCC_GPIOB);
  
  GPT_Init();
  
  Gpio_ConfigPin(GPIO_A, 0, GPIO_INPUT, GPIO_PULL_UP);
  Gpio_ConfigPin(GPIO_A, 1, GPIO_INPUT, GPIO_PULL_UP);
  
  int i;
  for (i = LED_ONE; i <= LED_THREE; i++)
  {
    Gpio_ConfigPin(GPIO_B, i, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_WritePin(GPIO_B, i, LOW);
  }
  
  
  volatile uint8 isDoorLocked = 1;
  volatile uint8 isDoorClosed = 1;
  
  volatile uint32 ticks = 0;
  volatile uint8 isTimerOn = 0;
  
  volatile uint8 caseOne = 0;
  volatile uint8 caseTwo = 0;
  volatile uint8 caseThree = 0;
  
  volatile uint8 keyOnePressed = 0;
  volatile uint8 keyTwoPressed = 0;
  
  while (1)
  {
    
    if(!keyOnePressed){
      if(!Gpio_ReadPin(GPIO_A, 0) && isDoorLocked && isDoorClosed){
        Delay_MS(30);
        
        if(!Gpio_ReadPin(GPIO_A, 0)){
          caseOne = 1;
          keyOnePressed = 1;
        }
      }
    }
    
    
    
    if(!keyTwoPressed){
      if(!Gpio_ReadPin(GPIO_A, 1) && !isDoorClosed && !isDoorLocked){
        Delay_MS(30);
        
        if(!Gpio_ReadPin(GPIO_A, 1)){
          caseTwo = 1;
          keyTwoPressed = 1;
        }
      }
    }
    
    if(!keyOnePressed){
      if(!Gpio_ReadPin(GPIO_A, 0) && isDoorClosed && !isDoorLocked){
        Delay_MS(30);
        
        if(!Gpio_ReadPin(GPIO_A, 0)){
          caseOne = 0;
          caseThree = 1;
          keyOnePressed = 1;
          
          isTimerOn = 0;
        }
      }
    }
    
    if(Gpio_ReadPin(GPIO_A, 0)){
      keyOnePressed = 0;
    }
    
    if(Gpio_ReadPin(GPIO_A, 1)){
      keyTwoPressed = 0;
    }
    
    if(caseOne){
      
      isDoorLocked = 0;
      
      if(!isTimerOn)
      {
        
        
        Gpio_WritePin(GPIO_B, LED_ONE, HIGH);
        Gpio_WritePin(GPIO_B, LED_TWO, HIGH);
        Gpio_WritePin(GPIO_B, LED_THREE, HIGH);
        
        GPT_StartTimer(500);
        isTimerOn = 1;
        ticks = 0;
      }
      
      if(GPT_CheckTimeIsElapsed()){
        ticks++;
      }
      
      if(ticks == 1){
        Gpio_WritePin(GPIO_B, LED_TWO, LOW);
      }else if(ticks == 4 && isDoorClosed){
        Gpio_WritePin(GPIO_B, LED_THREE, LOW);
      }else if(ticks >= 20 && isDoorClosed){
        Gpio_WritePin(GPIO_B, LED_ONE, LOW);
        Gpio_WritePin(GPIO_B, LED_THREE, LOW);
        
        if(ticks == 21 || ticks == 23){
          Gpio_WritePin(GPIO_B, LED_TWO, LOW);
          
          if(ticks == 23){
            isDoorLocked = 1;
            isDoorClosed = 1;
            isTimerOn = 0;
            caseOne = 0;
          }               
        }else if(ticks == 20 || ticks == 22){
          Gpio_WritePin(GPIO_B, LED_TWO, HIGH);  
        }       
      }
      
      if(!keyTwoPressed){
        if(!Gpio_ReadPin(GPIO_A, 1) && isDoorClosed){
          Delay_MS(30);
          if(!Gpio_ReadPin(GPIO_A, 1)){
            
            Gpio_WritePin(GPIO_B, LED_THREE, HIGH);
            
            isDoorClosed = 0;
            isTimerOn = 0;
            caseOne = 0;
            
            keyTwoPressed = 1;
          }        
        } 
      }
    }
    
    if(caseTwo){
      
      if(!isTimerOn){
        
        Gpio_WritePin(GPIO_B, LED_ONE, LOW);
        Gpio_WritePin(GPIO_B, LED_TWO, LOW);
        Gpio_WritePin(GPIO_B, LED_THREE, LOW);
        
        GPT_StartTimer(1000);
        isTimerOn = 1;
        ticks = 0;
      }
      
      if(GPT_CheckTimeIsElapsed()){
        ticks++;
      }
      
      if(ticks == 1){
        Gpio_WritePin(GPIO_B, LED_THREE, HIGH);  
      }else if(ticks == 2){
        Gpio_WritePin(GPIO_B, LED_THREE, LOW);
        
        isDoorClosed = 1;
        isTimerOn = 0;
        caseTwo = 0;
      }  
    } 
    
    if(caseThree){
      
      if(!isTimerOn){
        
        Gpio_WritePin(GPIO_B, LED_ONE, LOW);
        Gpio_WritePin(GPIO_B, LED_TWO, HIGH);
        Gpio_WritePin(GPIO_B, LED_THREE, LOW);
        
        GPT_StartTimer(500);
        isTimerOn = 1;
        ticks = 0;
      }
      
      if(GPT_CheckTimeIsElapsed()){
        ticks++;
      }
      
      if(ticks == 1 || ticks == 3){
        Gpio_WritePin(GPIO_B, LED_TWO, LOW);
        
        
        if(ticks == 3){
          
          isDoorLocked = 1;
          isDoorClosed = 1;
          isTimerOn = 0;
          caseThree = 0;
        }
      }else if(ticks == 2){
        Gpio_WritePin(GPIO_B, LED_TWO, HIGH);   
      }
    }
  }
  
  return 0;
}

