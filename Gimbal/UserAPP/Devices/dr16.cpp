/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    dr16.cpp
  * @author  Zelong.Xu (8762322@qq.com)
  * @brief   Code for DJI-DR16 driver in embedded software system.
  * @date    2021-04-07
  * @version 3.0
  * @par Change Log��
  * <table>
  * <tr><th>Date        <th>Version  <th>Author    		<th>Description
  * <tr><td>2019-04-00  <td> 2.0     <td>YuyongHu     <td>Creator
  * <tr><td>2019-11-08  <td> 2.1     <td>Zelong.Xu    <td>Re-format for API.
  * <tr><td>2021-04-07  <td> 3.0     <td>M3chD09      <td>Add DR16 prefix for enum API.
  * </table>
  *
  ==============================================================================
                            How to use this driver     
  ==============================================================================
    @note 
      -# ʵ����DR16_Classdef��
      -# ����Check_Link()����DR16���߼��
      -# ͨ�����ڽ���DR16���������ݣ�������DataCapture()����
      -# ��Ҫʱ���ø�ͨ����ȡ������ȡ���ݣ����߻�ȡ���ջ�������״̬

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

/* Includes ------------------------------------------------------------------*/ 
#include "dr16.h"

/* Private function declarations ---------------------------------------------*/
float DeadZone_Process(float num,float DZ_min, float DZ_max, float DZ_num);
/* function prototypes -------------------------------------------------------*/
/**
    * @brief  Initialize DR16 Class
    * @param  None
    * @retval None
    */
DR16_Classdef::DR16_Classdef()
{
    MouseCoefficient = 128;      //���ϵ����ʼ��
    Status = DR16_LOST;    //״̬��ʼ��
    for(short i=0;i<16;i++)      //��ֵ��ʼ��
    {
        Key[i].isPressed=false;
        Key[i].isTriggered=false;
    }
    //ҡ��ֵ��ʼ��
    DataPack.ch0 = 1024;
    DataPack.ch1 = 1024;
    DataPack.ch2 = 1024;
    DataPack.ch3 = 1024;
}

/**
 * @brief  ��ȡ���ݰ���������DR16�������յ������ݰ���ֵ����DR16�ĳ�Ա����DataPack�����м��̰�����������
 * @param  captureData:�յ������ݰ�ָ��
 * @return None.
 */
void DR16_Classdef::DataCapture(DR16_DataPack_Typedef* captureData)
{
    DataPack = *captureData; 

    /*�������ߣ���ʼ�ٴμ������*/
    last_check_time = 0;
    Status = DR16_ESTABLISHED;

    /*�������Ӱ�ͨ��ֵ��һ������*/
    RX_Norm = DeadZone_Process((float)(DataPack.ch0-1024)/660.0f,-Ignore_Limit,Ignore_Limit,0);
    RY_Norm = DeadZone_Process((float)(DataPack.ch1-1024)/660.0f,-Ignore_Limit,Ignore_Limit,0);
    LX_Norm = DeadZone_Process((float)(DataPack.ch2-1024)/660.0f,-Ignore_Limit,Ignore_Limit,0);
    LY_Norm = DeadZone_Process((float)(DataPack.ch3-1024)/660.0f,-Ignore_Limit,Ignore_Limit,0);

    float temp;

    temp = MouseCoefficient*((float)DataPack.x)/32768.0f;
    temp=temp>1?1:temp;
    temp=temp<-1?-1:temp;
    MouseX_Norm = temp;
    
    temp = MouseCoefficient*((float)DataPack.y)/32768.0f;
    temp=temp>1?1:temp;
    temp=temp<-1?-1:temp;
    MouseY_Norm = temp;

    
    temp = MouseCoefficient*((float)DataPack.z)/32768.0f;
    temp=temp>1?1:temp;
    temp=temp<-1?-1:temp;
    MouseZ_Norm = temp;

    /*��������*/
    Key_Process();
}

/**
 * @brief  �������� Key Process
 * @param  None
 * @return None
 */
void DR16_Classdef::Key_Process(void)
{
    for(short i=0;i<16;i++)
    {
        //��⵽��Ӧ�������¾���key�ṹ�������λ
        if(DataPack.key & (0x01<<i)) 
            Key[i].isPressed = true;
        else
        {
            Key[i].isPressed = false;
            Key[i].isTriggered = false;
        }
    }
    //������Ҽ�����
    if(DataPack.press_l == 0x01)
        Key[DR16_MOUSE_L].isPressed = true;
    else
    {
        Key[DR16_MOUSE_L].isPressed = false;
        Key[DR16_MOUSE_L].isTriggered = false;
    }
    if(DataPack.press_r == 0x01)
        Key[DR16_MOUSE_R].isPressed = true;
    else
    {
        Key[DR16_MOUSE_R].isPressed = false;
        Key[DR16_MOUSE_R].isTriggered = false;
    }
}

/**
 * @brief  ע�ᰴ���ص�����
 * @param  _Key ���µİ���
 * @param  Fun_Ptr ��Ӧ����Ҫ�ص��ĺ���
 * @return None
 */
void DR16_Classdef::Register_Click_Fun(int _Key, CLICK_EXCE Fun_Ptr)
{
    Click_Fun[_Key] = Fun_Ptr;
}

/**
 * @brief  �����ص�����
 * @param  None
 * @return None
 */
void DR16_Classdef::Exce_Click_Fun()
{
    for(size_t i = 0; i < 18; i++)
    {
        if(Click_Fun[i]!= NULL &&IsKeyPress(i)&&!IsKeyPress(DR16_KEY_CTRL))
        {
            if(!Key[i].isTriggered)
            {
            Key[i].isTriggered = 1;
            Click_Fun[i]();
            }
        }
    }
}

/**
 * @brief  ����Getxxx�������ܶ��ǻ�����ݰ��е�Get�������ݵ�ֵ��
 * @param  None
 * @return Get��������ݵ�ֵ
 */
uint64_t DR16_Classdef::GetCh0(void)
{
    return DataPack.ch0;
}

uint64_t DR16_Classdef::GetCh1(void)
{
    return DataPack.ch1;
}


uint64_t DR16_Classdef::GetCh2(void)
{
    return DataPack.ch2;
}


uint64_t DR16_Classdef::GetCh3(void)
{
    return DataPack.ch3;
}

SW_Status_Typedef DR16_Classdef::GetS2(void)
{
    return (SW_Status_Typedef)DataPack.s2;
}
SW_Status_Typedef DR16_Classdef::GetS1(void)
{
    return (SW_Status_Typedef)DataPack.s1;
}
int64_t DR16_Classdef::GetMouseX(void)
{
    return DataPack.x;
}
int64_t DR16_Classdef::GetMouseY(void)
{
    return DataPack.y;
}
int64_t DR16_Classdef::GetMouseZ(void)
{
    return DataPack.z;
}
uint64_t DR16_Classdef::GetPress_L(void)
{
    return DataPack.press_l;
}
uint64_t DR16_Classdef::GetPress_R(void)
{
	return DataPack.press_r;
}
uint64_t DR16_Classdef::Getkey(void)
{
    return DataPack.key;
}
/**
 * @brief  ��һ�����ͨ��0123�����XYZֵ(Left_X_Axis,Right_Y_Axis,balabala)
 * @param  None
 * @retval -1~1֮���ͨ��ֵ
 */
float DR16_Classdef::Get_RX_Norm(void)
{
    return RX_Norm;
}
float DR16_Classdef::Get_RY_Norm(void)
{
    return RY_Norm;
}
float DR16_Classdef::Get_LX_Norm(void)
{
    return LX_Norm;
}
float DR16_Classdef::Get_LY_Norm(void)
{
    return LY_Norm;
}
float DR16_Classdef::Get_MouseX_Norm(void)
{
    return MouseX_Norm;
}
float DR16_Classdef::Get_MouseY_Norm(void)
{
    return MouseY_Norm;
}
float DR16_Classdef::Get_MouseZ_Norm(void)
{
    return MouseZ_Norm;
}

/**
 * @brief  �����ж�ĳ�������Ƿ���,��ϼ������ж�
 * @param  _key ͷ�ļ��к궨���key������_w,_s��
 * @retval ����Ϊture��û����Ϊfalse
 */
bool DR16_Classdef::IsKeyPress(int _key)
{
    return Key[_key].isPressed;
}

/**
 * @brief   �õ�DR16��Ա����status��ֵ���������ж�DR16�Ƿ�����
 * @param   None
 * @return  Connection_Lost         DR16����  
 * @return  Connection_Established  DR16����
 */
LinkageStatus_Typedef DR16_Classdef::GetStatus(void)
{
  return Status;
}


/**
* @brief  ����ȷ�ϣ������豸������״̬��ÿ100ms��û�е���DataCapture()��
*         ��������ģʽ��
* @param  current_check_time ��ǰϵͳʱ�䣨���룩.
* @return None
*/
void DR16_Classdef::Check_Link(uint32_t current_check_time)
{
  static uint32_t dt = 0;

  /*��ʼ���*/
  if(last_check_time == 0)
  {
    last_check_time = current_check_time;
  }
  else
  {
      dt = current_check_time - last_check_time;
      if (dt > 100)
      {
          /*ʱ�Ӽ�ʱ���*/
          if (dt > 1000)
              last_check_time = 0;
          /*ÿ100ms����λ����Ϊ����*/
          else
              Status = DR16_LOST;

          /*���¿�ʼ���*/
          last_check_time = 0;
      }
      else {}
  }
}

/**
 * @brief  ��������������������㸽����΢С���
 * @param  num:Ҫ�������; DZ_min,DZ_max:������Χ;DZ_num:����������ʱ���ص�ֵ
 * @return �����Ľ��
 */
float DeadZone_Process(float num,float DZ_min, float DZ_max, float DZ_num)
{
    //�����������򷵻�����ֵ
    if(num<DZ_max&&num>DZ_min)
    {
        return DZ_num;
    }
    else
        return num;
}


/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
