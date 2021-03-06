/**
 * Copyright (c) 2011-2016, Jack Mo (mobangjack@foxmail.com).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "main.h"

static void vLedThreadFun(void* pvParam)
{
	while (1) {
		LED_GREEN_TOG();
		vTaskDelay(500);
	}
}

static void vSrsThreadFun(void* pvParam)
{
	while (1) {
		//Srs_Proc();
		vTaskDelay(20000);
	}
}

static void vDnlThreadFun(void* pvParam)
{
	while (1) {
		//Dnl_Proc();
		vTaskDelay(10000);
	}
}

static void vUplThreadFun(void* pvParam)
{
	while (1) {
		//Upl_Proc();
		vTaskDelay(10000);
	}
}

static void vDbgThreadFun(void* pvParam)
{
	//static uint32_t tick = 0;
	while (1) {
		//dbi.Print("Hello, Jack\r\n");
		//printf("Hello, Jack -> %d\r\n", ++tick);
		vTaskDelay(10000);
	}
}

static void vImuThreadFun(void* pvParam)
{
	static uint32_t last_tick = 0;
	static MPU_Data_t imu_data;
	while (1) {
		if (Clk_GetUsTick() - last_tick > 1e4) {
			last_tick = Clk_GetUsTick();
			if (MPU6500_Read(&imu_data)) {
				printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n", imu_data.ax, imu_data.ay, imu_data.az, imu_data.temp, imu_data.gx, imu_data.gy, imu_data.gz, imu_data.mx, imu_data.my, imu_data.mz);
			}
		}
	}
}

static TaskHandle_t xLedTaskHandle = NULL;
static TaskHandle_t xSrsTaskHandle = NULL;
static TaskHandle_t xDnlTaskHandle = NULL;
static TaskHandle_t xUplTaskHandle = NULL;
static TaskHandle_t xDbgTaskHandle = NULL;
static TaskHandle_t xImuTaskHandle = NULL;

static void vAppTaskCreate(void)
{
	xTaskCreate(vLedThreadFun, "vLedThreadFun", 512, NULL, 3, &xLedTaskHandle);
	xTaskCreate(vSrsThreadFun, "vSrsThreadFun", 512, NULL, 3, &xSrsTaskHandle);
	xTaskCreate(vDnlThreadFun, "vDnlThreadFun", 512, NULL, 3, &xDnlTaskHandle);
	xTaskCreate(vUplThreadFun, "vUplThreadFun", 512, NULL, 3, &xUplTaskHandle);
	xTaskCreate(vDbgThreadFun, "vDbgThreadFun", 512, NULL, 3, &xDbgTaskHandle);
	xTaskCreate(vImuThreadFun, "vImuThreadFun", 512, NULL, 3, &xImuTaskHandle);
}

//MPU_Data_t imu_data;

//static uint32_t last_time = 0;
int main()
{
	//__set_PRIMASK(1); // Close global interrupt
	
	// Boot KOS
	KOS_Boot();
	
	// Play startup music
	Snd_Play();

	// Wait for startup music
	//Delay_Ms(350);
	//Delay_Ms(1000);

	Clk_DelayUs(7e5); // For more precise delay (us)

	// Stop startup music
	Snd_Stop();
	
	vAppTaskCreate();
	vTaskStartScheduler();
	
	while(1)
	{
		//Srs_Proc();
		//Dnl_Proc();
		//if (Clk_GetUsTick() % 2000 == 0) {
		//	Upl_Proc();
		//}
		
		
		//if (Clk_GetUsTick() % 2000 == 0) {
			//printf("m:\t%f\t%f\t%f\t%f\n", odo.mp.w1, odo.mp.w2, odo.mp.w3, odo.mp.w4);
			//printf("m:\t%d\t%d\t%d\t%d\n", motor[0].angle_filtered, motor[1].angle_filtered, motor[2].angle_filtered, motor[3].angle_filtered);
			//printf("m:\t%f\t%f\t%f\t%f\n", motor[0].angle_rad, motor[1].angle_rad, motor[2].angle_rad, motor[3].angle_rad);
			//printf("m1: %d\t%d\t%d\n", motor[0].rate_raw, motor[0].rate_filtered, motor[0].rate_filtered - motor[0].rate_raw);
			//Dbg_Cfg();
			//printf("ex:\t%f\t%f\t%f\n", cmd.cp.x, odo.cp.x, (cmd.cp.x - odo.cp.x));
			//printf("ey:\t%f\t%f\t%f\n", cmd.cp.y, odo.cp.y, (cmd.cp.y - odo.cp.y));
			//printf("ez:\t%f\t%f\t%f\n", cmd.cp.z, odo.cp.z, (cmd.cp.z - odo.cp.z));
			//printf("%f\n", cfg.vel.x);
			//frame_id = motor[0].frame_cnt;
		
		//if (Clk_GetUsTick() - last_time > 10000) {
		//	last_time = Clk_GetUsTick();
		//	if (MPU6500_Read(&imu_data)) {
		//		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", imu_data.ax, imu_data.ay, imu_data.az, imu_data.temp, imu_data.gx, imu_data.gy, imu_data.gz);
		//	}
		//}
		
  }
}

