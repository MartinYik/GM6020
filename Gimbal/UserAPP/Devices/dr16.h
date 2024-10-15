/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    dr16.h
  * @author  Zelong.Xu 8762322@qq.com
  * @brief   Code for DJI-DR16 driver in embedded software system.
  ******************************************************************************
  * @attention
  *
  * if you had modified this file, please make sure your code does not have any
  * bugs, update the version Number, write dowm your name and the date. The most
  * important thing is make sure the users will have clear and definite under-
  * standing through your new brief.
  *
  * <h2><center>&copy; Copyright (c) 2019 - ~, SCUT-RobotLab Development Team.
  * All rights reserved.</center></h2>
  ******************************************************************************
  */
#ifndef _DR16_H_
#define _DR16_H_

#ifdef __cplusplus
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>

/* Private macros ------------------------------------------------------------*/
/* ��λ���� */
enum
{
  DR16_KEY_W = 0,
  DR16_KEY_S,
  DR16_KEY_A,
  DR16_KEY_D,
  DR16_KEY_SHIFT,
  DR16_KEY_CTRL,
  DR16_KEY_Q,
  DR16_KEY_E,
  DR16_KEY_R,
  DR16_KEY_F,
  DR16_KEY_G,
  DR16_KEY_Z,
  DR16_KEY_X,
  DR16_KEY_C,
  DR16_KEY_V,
  DR16_KEY_B,
  DR16_MOUSE_L,
  DR16_MOUSE_R,
};

/* Private type --------------------------------------------------------------*/
/** 
  @brief DR16���ݰ�����
*/
#pragma pack(1)
struct DR16_DataPack_Typedef
{
  uint64_t ch0:11;
  uint64_t ch1:11;
  uint64_t ch2:11;
  uint64_t ch3:11;
  uint64_t s2:2;
  uint64_t s1:2;
  int64_t x:16;
  int64_t y:16;
  int64_t z:16;
  uint64_t press_l:8;
  uint64_t press_r:8;
  uint64_t key:16;
};
#pragma pack()
/**
  @brief �ֱ���������λ����״̬
*/
enum SW_Status_Typedef
{
  DR16_SW_NONE = 0,
  DR16_SW_UP = 1,
  DR16_SW_MID = 3,
  DR16_SW_DOWN = 2,
};


/**
  @brief �������Ͷ���
*/
struct Key_Typedef
{
    bool isPressed;   /*<! �Ƿ��� */
    bool isTriggered; /*<! �Ƿ񴥷�������������ִ�е���¼� */
};

/**
  @brief ����״̬
*/
#ifndef __LinkageStatus_DEFINED
#define __LinkageStatus_DEFINED
enum LinkageStatus_Typedef
{
  DR16_LOST = 0U,
  DR16_ESTABLISHED,
};
#endif

typedef void(*CLICK_EXCE)(void);  /*<! ����ִ�к��� */
/* Exported macros -----------------------------------------------------------*/
#define Ignore_Limit 0.05         /*<! ��������,�ֱ������Ĺ�һ����ľ���ֵС�ڴ�ֵʱ�Զ���Ϊ0 */

/* Exported types ------------------------------------------------------------*/
/* DR16���� */
class DR16_Classdef
{
    private:
        LinkageStatus_Typedef Status;   /*<! ����״̬ */
        uint8_t  detecting_flag;        /*<! ��������־λ*/
		    uint32_t last_check_time;       /*<! ��һ�����߼��ʱ��*/
        DR16_DataPack_Typedef DataPack; /*<! ���ݰ�*/
        float RX_Norm,RY_Norm,LX_Norm,LY_Norm,MouseX_Norm,MouseY_Norm,MouseZ_Norm;
                                        /*<! ����ҡ���ĸ�������������������ٶȹ�һ�����ֵ*/
        Key_Typedef Key[18];            /*<! 16�����������Ϣ*/
		    CLICK_EXCE Click_Fun[18];       /*<! ����ִ�к��� */
        float MouseCoefficient;         /*<! ��궯���˵�ϵ��*/
        void Key_Process(void);         /*<! ��������*/

    public:
/* Exported function declarations --------------------------------------------*/
        DR16_Classdef();
        void DataCapture(DR16_DataPack_Typedef* captureData);

        uint64_t GetCh0(void);
        uint64_t GetCh1(void);
        uint64_t GetCh2(void);
        uint64_t GetCh3(void);
        SW_Status_Typedef GetS2(void);
        SW_Status_Typedef GetS1(void);
        int64_t GetMouseX(void);
        int64_t GetMouseY(void);
        int64_t GetMouseZ(void);
        uint64_t GetPress_L(void);
        uint64_t GetPress_R(void);
        uint64_t Getkey(void);

        /*��һ�����ͨ��0123�����XYZֵ*/
        float Get_RX_Norm(void);
        float Get_RY_Norm(void);
        float Get_LX_Norm(void);
        float Get_LY_Norm(void);
        float Get_MouseX_Norm(void);
        float Get_MouseY_Norm(void);
        float Get_MouseZ_Norm(void);

        /*�����ж�ĳ�������Ƿ���,����֮��Ļص�����*/
        bool IsKeyPress(int _key);
        void Register_Click_Fun(int _Key, CLICK_EXCE Fun_Ptr);
        void Exce_Click_Fun();

        /*����״̬��ز���*/
        void Check_Link(uint32_t current_check_time);
		    void SetStatus(LinkageStatus_Typedef para_status);
        LinkageStatus_Typedef GetStatus(void);
};

#endif
#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
