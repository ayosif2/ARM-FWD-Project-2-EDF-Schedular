/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  main.c
 *        \brief  the main c file for the project
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "GPIO.h"
#include "bit_math.h"	
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "serial.h"

extern BaseType_t xTaskCreatePeriodic( TaskFunction_t pxTaskCode,
                            const char * const pcName, 
                            const configSTACK_DEPTH_TYPE usStackDepth,
                            void * const pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * const pxCreatedTask,
														TickType_t period);
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
	TaskHandle_t TSK_1 = NULL;
	TaskHandle_t TSK_2 = NULL;
	TaskHandle_t TSK_3 = NULL;
	TaskHandle_t TSK_4 = NULL;
	TaskHandle_t TSK_5 = NULL;
	TaskHandle_t TSK_6 = NULL;
//	TaskHandle_t TSK_7 = NULL;
	QueueHandle_t xQueue1;
	xComPortHandle SERHAN;
	char state_Buffer[400];

	void TASK_1(){
		int flag;
		TickType_t xLastWakeTime;
		char Consumer_send1[64] = "Btn1PE ";
		char Consumer_send2[64] = "Btn1NE ";
		const TickType_t xFrequency = 50;
		vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			if (GPIO_read(PORT_1, PIN1)==PIN_IS_HIGH){
				if (flag==0){
					xQueueSendToBack( xQueue1,( void * ) &Consumer_send1,( TickType_t ) 20 );		
				}
				flag=1;
			}else if (GPIO_read(PORT_1, PIN1)==PIN_IS_LOW){
				if (flag==1){
					xQueueSendToBack( xQueue1,( void * ) &Consumer_send2,( TickType_t ) 20 );				
				}
				flag=0;
			}	
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}
	void TASK_2(){
		int flag;
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 50;
		char Consumer_send1[32] = "Btn2_PE ";
		char Consumer_send2[32] = "Btn2_NE ";
		vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );
		xLastWakeTime=xTaskGetTickCount();	
		while(1){
			if (GPIO_read(PORT_1, PIN2)==PIN_IS_HIGH){
				if (flag==0){
					xQueueSendToBack( xQueue1,( void * ) &Consumer_send1,( TickType_t ) 20 );		
				}
				flag=1;
			}else if (GPIO_read(PORT_1, PIN2)==PIN_IS_LOW){
				if (flag==1){
					xQueueSendToBack( xQueue1,( void * ) &Consumer_send2,( TickType_t ) 20 );				
				}
				flag=0;
			}			
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}
	void TASK_3(){
		char Consumer_send1[64] = "Periodc";
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 100;
		vTaskSetApplicationTaskTag( NULL, ( void * ) 3 );
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			xQueueSendToBack( xQueue1,( void * ) &Consumer_send1,( TickType_t ) 10 );
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}
	void TASK_4(){
		char Consumer_recieve[64];
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 10;
		vTaskSetApplicationTaskTag( NULL, ( void * ) 4 );
		xLastWakeTime=xTaskGetTickCount();
		while(1){

			if(xQueueReceive( xQueue1,&( Consumer_recieve ),( TickType_t ) 10 )==pdPASS){
			vSerialPutString(SERHAN,Consumer_recieve,64);
			xSerialPutChar(SERHAN,'\n',( TickType_t ) 10);
			}
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}
	void TASK_5(){
		int i=0;
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 10;
		vTaskSetApplicationTaskTag( NULL, ( void * ) 5 );
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			for(i=1;i<33186;i++){ //this number corrosponds to 5 ms excecution time 
			i=i;
			}
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}
	void TASK_6(){
		int i=0;
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 100;
		vTaskSetApplicationTaskTag( NULL, ( void * ) 6 );
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			for(i=1;i<79646;i++){ //this aamount corrosponds to 12 ms
			i=i;
			}
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}
	/*void TASK_7(){
		int i=0;
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 20;
		vTaskSetApplicationTaskTag( NULL, ( void * ) 7 );
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			vTaskGetRunTimeStats(state_Buffer);
			vSerialPutString(SERHAN,state_Buffer  ,400);	
			xSerialPutChar(SERHAN,'\n',( TickType_t ) 10);
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}*/
	void Task_Init(){
		xTaskCreatePeriodic( TASK_1,"Button_1_Monitor",100,NULL,1,&TSK_1,50);
		xTaskCreatePeriodic( TASK_2,"Button_2_Monitor",100,NULL,1,&TSK_2,50);
		xTaskCreatePeriodic( TASK_3,"Periodic_Transmitter",100,NULL,1,&TSK_3,100);
		xTaskCreatePeriodic( TASK_4,"Uart_Receiver",100,NULL,1,&TSK_4,10);
		xTaskCreatePeriodic( TASK_5,"Load_1_Simulation",100,NULL,1,&TSK_5,10);
		xTaskCreatePeriodic( TASK_6,"Load_2_Simulation",100,NULL,1,&TSK_6,100);
	//	xTaskCreatePeriodic( TASK_7,"Trace",100,NULL,3,&TSK_7,20);

	}
	void vApplicationIdleHook( void ){
	GPIO_write(PORT_0,PIN7,PIN_IS_HIGH);

	};
	void vApplicationTickHook( void ){
			GPIO_write(PORT_0,PIN6,PIN_IS_HIGH);
			GPIO_write(PORT_0,PIN6,PIN_IS_LOW);

	};

	static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}
	int main(){
	configTimer1();
	GPIO_init();
	xSerialPortInitMinimal(  115200 ,400 );
	//vSerialPutString(SERHAN,"TEST",5);
	Task_Init();	
	xQueue1 = xQueueCreate( 200, sizeof( unsigned long long ) );
	vTaskStartScheduler();
	while(1){}
	}
