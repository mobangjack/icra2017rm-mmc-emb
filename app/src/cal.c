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
 
#include "cal.h"
#include <stdio.h>

/*****************************************/
/*              Calibration              */
/*****************************************/

static CalFlag_t calFlag = 0;

static Maf_t maf;
static float buf[CAL_GM_MAF_BUF_LEN];

static uint32_t gm_startup_delay_cnt = 0;

void Cal_Init(void)
{
	calFlag = 0;
	gm_startup_delay_cnt = 0;
	Maf_Init(&maf, buf, CAL_GM_MAF_BUF_LEN);
}

void Cal_SetGph(void)
{
	GM_CMD(0, 0);
	Maf_Reset(&maf); // Reset maf
	gm_startup_delay_cnt = 0;
	cfg.pos.eh = odo.gp.e;
	Flag_Set(&calFlag, CAL_FLAG_GPH);
}

void Cal_SetGpl(void)
{
	GM_CMD(0, 0);
	Maf_Reset(&maf); // Reset maf
	gm_startup_delay_cnt = 0;
	cfg.pos.el = odo.gp.e;
	Flag_Set(&calFlag, CAL_FLAG_GPL);
}

void Cal_Proc(void)
{
	if (!Cal_HasFlag(CAL_FLAG_GPH)) {
		if (!KEY_H_IS_PRESSED()) {
			GM_CMD(0, CAL_GM_DRV_CURRENT * CAL_GM_UP_DIR);
			Maf_Proc(&maf, ABSVAL(odo.gv.e));
			if (gm_startup_delay_cnt < CAL_GM_START_UP_DELAY) {
				gm_startup_delay_cnt++;
			} 
			// Bang detection
			else if (ABSVAL(maf.avg) <= CAL_GM_BANG_VEL_DET) {
				Cal_SetGph();
				//printf("up_bang detected, gph=%f\n", cfg.pos.eh);
			}
			//printf("gv=%f,maf=%f\n", odo.gv.e, maf.avg);
		} else {
			Cal_SetGph();
			//printf("key_h detected, gph=%f\n", cfg.pos.eh);
		}
	}
	else if (!Cal_HasFlag(CAL_FLAG_GPL)) {
		if (!KEY_L_IS_PRESSED()) {
			GM_CMD(0, CAL_GM_DRV_CURRENT * (-CAL_GM_UP_DIR));
			Maf_Proc(&maf, ABSVAL(odo.gv.e));
			if (gm_startup_delay_cnt < CAL_GM_START_UP_DELAY) {
				gm_startup_delay_cnt++;
			} 
			// Bang detection
			else if (ABSVAL(maf.avg) <= CAL_GM_BANG_VEL_DET) {
				Cal_SetGpl();
				//printf("dn_bang detected, gpl=%f\n", cfg.pos.el);
			}
			//printf("gv=%f,maf=%f\n", odo.gv.e, maf.avg);
		} else {
			Cal_SetGpl();
			//printf("key_l detected, gpl=%f\n", cfg.pos.el);
		}
	}
}

CalFlag_t Cal_GetFlag(void)
{
	return calFlag;
}

void Cal_SetFlag(CalFlag_t flag)
{
	calFlag |= flag;
}

CalFlag_t Cal_HasFlag(CalFlag_t mask)
{
	return calFlag & mask;
}

CalFlag_t Cal_HitFlag(CalFlag_t mask)
{
	return (calFlag & mask) == mask;
}

CalFlag_t Cal_IsDone(void)
{
	//return Cal_HitFlag(CAL_FLAG_GIM); // DBUG
	return 1;
}



