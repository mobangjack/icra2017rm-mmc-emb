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
 
#ifndef __MSG_H__
#define __MSG_H__

/*********************************************/
/*             KylinBot Msg Type             */
/* Basic frame structure:                    */
/*      |id:8|length:8|token:16|data~|       */
/*      |______________________|_____|       */
/*                head          body~        */
/*                                           */
/*********************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "cbus.h"
#include "dbus.h"
#include "fifo.h"
#include "crc16.h"
	
/* Message length minimum/maximum limit */
#define MSG_LEN_MIN 4u
#define MSG_LEN_MAX 256u

/* Message head union typedef */
typedef union MsgHead_t
{
	uint32_t value; // Message head value in 32bit
	struct
	{
		uint8_t id : 8; // Message ID
		uint8_t length : 8; // Message length (unit: byte)
		uint16_t token : 16; // Message CRC token
	}attr; // Message head attributes
}MsgHead_t; // Message head union typedef

typedef Rcp_t VirtualRC_t;
typedef Hcp_t VirtualHC_t;
typedef DBUS_t VirtualDBUS_t;
typedef CBUS_t VirtualCBUS_t;

#define ZGYRO_ANGLE_RECIP 1e-2f // To scale zgyro angle to deg
#define ZGYRO_RATE_RECIP 1e-5f // To scale zgyro rate to deg/s
typedef struct
{
	int32_t angle; // = (deg*100)
	int16_t rate; // = delta(angle)/1ms
}ZGyroMsg_t;

#define MOTOR1_ID 0
#define MOTOR2_ID 1
#define MOTOR3_ID 2
#define MOTOR4_ID 3
#define MOTOR5_ID 4
#define MOTOR6_ID 5
#define MOTOR_ECD_ANGLE_MAX 8191
#define MOTOR_ECD_ANGLE_MAX 8191
#define MOTOR_ESC_CURRENT_MAX 13000
typedef struct
{
	uint8_t id; // 0~5
	uint16_t ecd_angle; // Encoder angle, range from 0~8191
	int32_t angle; // Continuous angle, infinite
	int16_t rate; // Rate in ecd_diff/1ms
	int32_t current; // Motor current, max to 13000
}MotorMsg_t;

typedef struct
{
	int16_t ax;
	int16_t ay;
	int16_t az;
	int16_t gx;
	int16_t gy;
	int16_t gz;
	int16_t mx;
	int16_t my;
	int16_t mz;
}IMU9XMsg_t;

typedef struct
{
	int32_t px; // Bot position (linear) in x-axis, unit: mm
	int32_t py; // Bot position (linear) in y-axis, unit: mm
	int32_t pz; // Bot position (angular) in z-axis, unit: mm
	int16_t vx; // Bot velocity (linear) in x-axis, unit: mm
	int16_t vy; // Bot velocity (linear) in y-axis, unit: mm
	int16_t vz; // Bot velocity (angular) in z-axis, unit: mm
}OdomeMsg_t;

typedef struct
{
	int16_t pe; // Elevator position
	int16_t pc; // Claw position
}GraspMsg_t;

#define PID_CALIB_TYPE_CHASSIS_VELOCITY 0x01
#define PID_CALIB_TYPE_GRABBER_VELOCITY 0x02
#define PID_CALIB_TYPE_GRABBER_POSITION 0x03
#define PID_CALIB_VALUE_RECIP 0.01f
typedef struct
{
	uint8_t type;
	uint16_t kp;
	uint16_t ki;
	uint16_t kd;
	uint16_t it;
	uint16_t Emax;
	uint16_t Pmax;
	uint16_t Imax;
	uint16_t Dmax;
	uint16_t Omax;
}PIDCalib_t; // PID Calibration

#define IMU_CALIB_VALUE_RECIP 1.0f
typedef struct
{
	int16_t ax_offset;
	int16_t ay_offset;
	int16_t az_offset;
	int16_t gx_offset;
	int16_t gy_offset;
	int16_t gz_offset;
}IMUCalib_t; // IMU offset Calibration

#define MAG_CALIB_VALUE_RECIP 1.0f
typedef struct
{
	int16_t mx_offset;
	int16_t my_offset;
	int16_t mz_offset;
}MagCalib_t; // Mag offset Calibration

#define VEL_CALIB_VALUE_RECIP 0.001f
typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t z;
	uint16_t e;
	uint16_t c;
}VelCalib_t; // Velocity Calibration

#define MEC_CALIB_VALUE_RECIP 0.001f
typedef struct
{
	uint16_t lx; // mm
	uint16_t ly; // mm
	uint16_t r1; // mm
	uint16_t r2; // mm
}MecCalib_t; // Mecanum Wheel Calibration

#define POS_CALIB_VALUE_RECIP 0.001f
typedef struct
{
	int16_t el; // unit: mm
	int16_t eh; // unit: mm
	int16_t cl; // unit: 1000*rad
	int16_t ch; // unit: 1000*rad
}PosCalib_t; // Position Calibration

#define CALIB_FLAG_BIT_IMU (1u<<0)
#define CALIB_FLAG_BIT_MAG (1u<<1)
#define CALIB_FLAG_BIT_POS (1u<<2)
typedef struct
{
	uint32_t auto_cali_flag; // Auto calibration control bits
}CalibMsg_t;

#define WDG_ERR_BIT_RCV			   (1u<<0)
#define WDG_ERR_BIT_TTY			   (1u<<1)
#define WDG_ERR_BIT_BTM			   (1u<<2)
#define WDG_ERR_BIT_DBI			   (1u<<3)
#define WDG_ERR_BIT_IMU			   (1u<<4)
#define WDG_ERR_BIT_VRC			   (1u<<5)
#define WDG_ERR_BIT_VHC			   (1u<<6)
#define WDG_ERR_BIT_VDBUS			 (1u<<7)
#define WDG_ERR_BIT_VCBUS			 (1u<<8)
#define WDG_ERR_BIT_CALIB		   (1u<<9)
#define WDG_ERR_BIT_ZGYRO		   (1u<<10)
#define WDG_ERR_BIT_MOTOR1		 (1u<<11)
#define WDG_ERR_BIT_MOTOR2		 (1u<<12)
#define WDG_ERR_BIT_MOTOR3		 (1u<<13)
#define WDG_ERR_BIT_MOTOR4		 (1u<<14)
#define WDG_ERR_BIT_MOTOR5		 (1u<<15)
#define WDG_ERR_BIT_MOTOR6		 (1u<<16)

#define INI_FLAG_BIT_ZGYRO      (1u<<0)
#define INI_FLAG_BIT_MOTOR1     (1u<<1)
#define INI_FLAG_BIT_MOTOR2     (1u<<2)
#define INI_FLAG_BIT_MOTOR3     (1u<<3)
#define INI_FLAG_BIT_MOTOR4     (1u<<4)
#define INI_FLAG_BIT_MOTOR5     (1u<<5)
#define INI_FLAG_BIT_MOTOR6     (1u<<6)

typedef struct
{
	uint32_t wdg; // Watchdog
	uint32_t ini; // Initialization status
}StatuMsg_t;

#define WRAP_U8(V) ((uint8_t)V)
#define WRAP_U16(V) ((uint16_t)V)
#define WRAP_U32(V) ((uint32_t)V)

#define MSG_ID_VRC WRAP_U8(0x01)
#define MSG_ID_VHC WRAP_U8(0x02)
#define MSG_ID_VDBUS WRAP_U8(0x03)
#define MSG_ID_VCBUS WRAP_U8(0x04)
#define MSG_ID_ZGYRO WRAP_U8(0x05)
#define MSG_ID_MOTOR WRAP_U8(0x06)
#define MSG_ID_ODOME WRAP_U8(0x07)
#define MSG_ID_GRASP WRAP_U8(0x08)
#define MSG_ID_STATU WRAP_U8(0x09)
#define MSG_ID_CALIB WRAP_U8(0x0a)

#define MSG_LEN_VRC sizeof(VirtualRC_t)
#define MSG_LEN_VHC sizeof(VirtualHC_t)
#define MSG_LEN_VDBUS sizeof(VirtualDBUS_t)
#define MSG_LEN_VCBUS sizeof(VirtualCBUS_t)
#define MSG_LEN_ZGYRO sizeof(ZGyroMsg_t)
#define MSG_LEN_MOTOR sizeof(MotorMsg_t)
#define MSG_LEN_ODOME sizeof(OdomeMsg_t)
#define MSG_LEN_GRASP sizeof(GraspMsg_t)
#define MSG_LEN_STATU sizeof(StatuMsg_t)
#define MSG_LEN_CALIB sizeof(CalibMsg_t)
	
#define MSG_TOKEN_VRC WRAP_U16(0x1234)
#define MSG_TOKEN_VHC WRAP_U16(0x2345)
#define MSG_TOKEN_VDBUS WRAP_U16(0x3456)
#define MSG_TOKEN_VCBUS WRAP_U16(0x4567)
#define MSG_TOKEN_ZGYRO WRAP_U16(0x5678)
#define MSG_TOKEN_MOTOR WRAP_U16(0x6789)
#define MSG_TOKEN_ODOME WRAP_U16(0x789a)
#define MSG_TOKEN_GRASP WRAP_U16(0x89ab)
#define MSG_TOKEN_STATU WRAP_U16(0x9abc)
#define MSG_TOKEN_CALIB WRAP_U16(0xabcd)

#define MSG_HEAD_VALUE(ID,LEN,TOKEN) ((WRAP_U32(TOKEN)<<16) | (WRAP_U32(LEN)<<8) | WRAP_U32(ID))
#define MSG_HEAD_VALUE_OF(NAME) MSG_HEAD_VALUE(MSG_ID_##NAME,MSG_LEN_##NAME,MSG_TOKEN_##NAME)

#define MSG_HEAD_VALUE_VRC MSG_HEAD_VALUE_OF(VRC)
#define MSG_HEAD_VALUE_VHC MSG_HEAD_VALUE_OF(VHC)
#define MSG_HEAD_VALUE_VDBUS MSG_HEAD_VALUE_OF(VDBUS)
#define MSG_HEAD_VALUE_VCBUS MSG_HEAD_VALUE_OF(VCBUS)
#define MSG_HEAD_VALUE_ZGYRO MSG_HEAD_VALUE_OF(ZGYRO)
#define MSG_HEAD_VALUE_MOTOR MSG_HEAD_VALUE_OF(MOTOR)
#define MSG_HEAD_VALUE_ODOME MSG_HEAD_VALUE_OF(ODOME)
#define MSG_HEAD_VALUE_GRASP MSG_HEAD_VALUE_OF(GRASP)
#define MSG_HEAD_VALUE_STATU MSG_HEAD_VALUE_OF(STATU)
#define MSG_HEAD_VALUE_CALIB MSG_HEAD_VALUE_OF(CALIB)

#define MSG_HEAD_VRC { MSG_HEAD_VALUE_VRC }
#define MSG_HEAD_VHC { MSG_HEAD_VALUE_VHC }
#define MSG_HEAD_VDBUS { MSG_HEAD_VALUE_VDBUS }
#define MSG_HEAD_VCBUS { MSG_HEAD_VALUE_VCBUS }
#define MSG_HEAD_ZGYRO { MSG_HEAD_VALUE_ZGYRO }
#define MSG_HEAD_MOTOR { MSG_HEAD_VALUE_MOTOR }
#define MSG_HEAD_ODOME { MSG_HEAD_VALUE_ODOME }
#define MSG_HEAD_GRASP { MSG_HEAD_VALUE_GRASP }
#define MSG_HEAD_STATU { MSG_HEAD_VALUE_STATU }
#define MSG_HEAD_CALIB { MSG_HEAD_VALUE_CALIB }

typedef enum
{
	MSG_TYPE_VRC = 0x00,
	MSG_TYPE_VHC = 0x01,
	MSG_TYPE_VDBUS = 0x02,
	MSG_TYPE_VCBUS = 0x03,
	MSG_TYPE_ZGYRO = 0x04,
	MSG_TYPE_MOTOR = 0x05,
	MSG_TYPE_ODOME = 0x06,
	MSG_TYPE_GRASP = 0x07,
	MSG_TYPE_STATU = 0x08,
	MSG_TYPE_CALIB = 0x09,
}MsgType_t;

/**
 * Brief: Push a single message to message fifo. 
 * @arg fifo Message fifo
 * @arg head Message head
 * @arg body Message body
 * @ret Message length (num of bytes)
 */
uint32_t Msg_Push(FIFO_t* fifo, const void* head, const void* body);

/**
 * Brief: Pop a single message from message fifo. 
 * @arg fifo Message fifo
 * @arg head Message head
 * @arg body Message body
 * @ret Message length (num of bytes)
 */
uint32_t Msg_Pop(FIFO_t* fifo, const void* head, void* body);

extern const MsgHead_t msg_head_vrc;
extern const MsgHead_t msg_head_vhc;
extern const MsgHead_t msg_head_vdbus;
extern const MsgHead_t msg_head_vcbus;
extern const MsgHead_t msg_head_zgyro;
extern const MsgHead_t msg_head_motor;
extern const MsgHead_t msg_head_odome;
extern const MsgHead_t msg_head_grasp;
extern const MsgHead_t msg_head_statu;
extern const MsgHead_t msg_head_calib;

#ifdef __cplusplus
}
#endif

#endif


