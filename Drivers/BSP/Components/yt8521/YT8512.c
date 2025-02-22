/** 
  ******************************************************************************
  * @file    YT8512.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the YT8512
  *          PHY devices.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "YT8512.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */ 
  
/** @defgroup YT8512 YT8512
  * @{
  */   
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup YT8512_Private_Defines YT8512 Private Defines
  * @{
  */
#define YT8512_SW_RESET_TO    ((uint32_t)500U)
#define YT8512_INIT_TO        ((uint32_t)2000U)
#define YT8512_MAX_DEV_ADDR   ((uint32_t)31U)
/**
  * @}
  */
 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup YT8512_Private_Functions YT8512 Private Functions
  * @{
  */
    
/**
  * @brief  Register IO functions to component object
  * @param  pObj: device object  of YT8512_Object_t. 
  * @param  ioctx: holds device IO functions.  
  * @retval YT8512_STATUS_OK  if OK
  *         YT8512_STATUS_ERROR if missing mandatory function
  */
int32_t  YT8512_RegisterBusIO(YT8512_Object_t *pObj, YT8512_IOCtx_t *ioctx)
{
  if(!pObj || !ioctx->ReadReg || !ioctx->WriteReg || !ioctx->GetTick)
  {
    return YT8512_STATUS_ERROR;
  }
  
  pObj->IO.Init = ioctx->Init;
  pObj->IO.DeInit = ioctx->DeInit;
  pObj->IO.ReadReg = ioctx->ReadReg;
  pObj->IO.WriteReg = ioctx->WriteReg;
  pObj->IO.GetTick = ioctx->GetTick;
  
  return YT8512_STATUS_OK;
}

/**
  * @brief  Initialize the YT8512 and configure the needed hardware resources
  * @param  pObj: device object YT8512_Object_t. 
  * @retval YT8512_STATUS_OK  if OK
  *         YT8512_STATUS_ADDRESS_ERROR if cannot find device address
  *         YT8512_STATUS_READ_ERROR if connot read register
  *         YT8512_STATUS_WRITE_ERROR if connot write to register
  *         YT8512_STATUS_RESET_TIMEOUT if cannot perform a software reset
  */
 int32_t YT8512_Init(YT8512_Object_t *pObj)
 {
   uint32_t tickstart = 0, regvalue = 0, addr = 0;
   int32_t status = YT8512_STATUS_OK;
   
   if(pObj->Is_Initialized == 0)
   {
     if(pObj->IO.Init != 0)
     {
       /* GPIO and Clocks initialization */
       pObj->IO.Init();
     }
   
     /* for later check */
     pObj->DevAddr = YT8512_MAX_DEV_ADDR + 1;
   
     /* Get the device address from special mode register */  
     for(addr = 0; addr <= YT8512_MAX_DEV_ADDR; addr ++)
     {
       if(pObj->IO.ReadReg(addr, YT8512_PHYSCR, &regvalue) < 0)
       { 
         status = YT8512_STATUS_READ_ERROR;
         /* Can't read from this device address 
            continue with next address */
         continue;
       }
//			if ((regvalue & 0x001FU) == addr)
//				{
//						pObj->DevAddr = addr;
//						status = YT8512_STATUS_OK;
//						break;
//				}
       if((regvalue & YT8512_PHYSCR_DEFAULT) == YT8512_PHYSCR_DEFAULT)
       {
         pObj->DevAddr = addr;
         status = YT8512_STATUS_OK;
         break;
       }
     }
   
     if(pObj->DevAddr > YT8512_MAX_DEV_ADDR)
     {
       status = YT8512_STATUS_ADDRESS_ERROR;
     }
		
//     pObj->DevAddr = YT8512_ADDR;
//     status = YT8512_STATUS_OK;

     /* if device address is matched */
     if(status == YT8512_STATUS_OK)
     {
       /* set a software reset  */
       if(pObj->IO.WriteReg(pObj->DevAddr, YT8512_BCR, YT8512_BCR_SOFT_RESET) >= 0)
       { 
         /* get software reset status */
         if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BCR, &regvalue) >= 0)
         { 
           tickstart = pObj->IO.GetTick();
           
           /* wait until software reset is done or timeout occured  */
           while(regvalue & YT8512_BCR_SOFT_RESET)
           {
             if((pObj->IO.GetTick() - tickstart) <= YT8512_SW_RESET_TO)
             {
               if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BCR, &regvalue) < 0)
               { 
                 status = YT8512_STATUS_READ_ERROR;
                 break;
               }
             }
             else
             {
               status = YT8512_STATUS_RESET_TIMEOUT;
               break;
             }
           } 
         }
         else
         {
           status = YT8512_STATUS_READ_ERROR;
         }
       }
       else
       {
         status = YT8512_STATUS_WRITE_ERROR;
       }
     }
   }
      
   if(status == YT8512_STATUS_OK)
   {
     tickstart =  pObj->IO.GetTick();
     
     /* Wait for 2s to perform initialization */
     while((pObj->IO.GetTick() - tickstart) <= YT8512_INIT_TO)
     {
     }
     pObj->Is_Initialized = 1;
   }
   
   return status;
 }

/**
  * @brief  De-Initialize the YT8512 and it's hardware resources
  * @param  pObj: device object YT8512_Object_t. 
  * @retval None
  */
int32_t YT8512_DeInit(YT8512_Object_t *pObj)
{
  if(pObj->Is_Initialized)
  {
    if(pObj->IO.DeInit != 0)
    {
      if(pObj->IO.DeInit() < 0)
      {
        return YT8512_STATUS_ERROR;
      }
    }
  
    pObj->Is_Initialized = 0;  
  }
  
  return YT8512_STATUS_OK;
}

/**
  * @brief  Disable the YT8512 power down mode.
  * @param  pObj: device object YT8512_Object_t.  
  * @retval YT8512_STATUS_OK  if OK
  *         YT8512_STATUS_READ_ERROR if connot read register
  *         YT8512_STATUS_WRITE_ERROR if connot write to register
  */
int32_t YT8512_DisablePowerDownMode(YT8512_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = YT8512_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BCR, &readval) >= 0)
  {
    readval &= ~YT8512_BCR_POWER_DOWN;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, YT8512_BCR, readval) < 0)
    {
      status =  YT8512_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = YT8512_STATUS_READ_ERROR;
  }
   
  return status;
}

/**
  * @brief  Enable the YT8512 power down mode.
  * @param  pObj: device object YT8512_Object_t.  
  * @retval YT8512_STATUS_OK  if OK
  *         YT8512_STATUS_READ_ERROR if connot read register
  *         YT8512_STATUS_WRITE_ERROR if connot write to register
  */
int32_t YT8512_EnablePowerDownMode(YT8512_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = YT8512_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BCR, &readval) >= 0)
  {
    readval |= YT8512_BCR_POWER_DOWN;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, YT8512_BCR, readval) < 0)
    {
      status =  YT8512_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = YT8512_STATUS_READ_ERROR;
  }
   
  return status;
}

/**
  * @brief  Start the auto negotiation process.
  * @param  pObj: device object YT8512_Object_t.  
  * @retval YT8512_STATUS_OK  if OK
  *         YT8512_STATUS_READ_ERROR if connot read register
  *         YT8512_STATUS_WRITE_ERROR if connot write to register
  */
int32_t YT8512_StartAutoNego(YT8512_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = YT8512_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BCR, &readval) >= 0)
  {
    readval |= YT8512_BCR_AUTONEGO_EN;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, YT8512_BCR, readval) < 0)
    {
      status =  YT8512_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = YT8512_STATUS_READ_ERROR;
  }
   
  return status;
}

/**
  * @brief  Get the link state of YT8512 device.
  * @param  pObj: Pointer to device object. 
  * @param  pLinkState: Pointer to link state
  * @retval YT8512_STATUS_LINK_DOWN  if link is down
  *         YT8512_STATUS_AUTONEGO_NOTDONE if Auto nego not completed 
  *         YT8512_STATUS_100MBITS_FULLDUPLEX if 100Mb/s FD
  *         YT8512_STATUS_100MBITS_HALFDUPLEX if 100Mb/s HD
  *         YT8512_STATUS_10MBITS_FULLDUPLEX  if 10Mb/s FD
  *         YT8512_STATUS_10MBITS_HALFDUPLEX  if 10Mb/s HD       
  *         YT8512_STATUS_READ_ERROR if connot read register
  *         YT8512_STATUS_WRITE_ERROR if connot write to register
  */
int32_t YT8512_GetLinkState(YT8512_Object_t *pObj)
{
  uint32_t readval = 0;
  
  /* Read Status register  */
  if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BSR, &readval) < 0)
  {
    return YT8512_STATUS_READ_ERROR;
  }
  
  /* Read Status register again */
  if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BSR, &readval) < 0)
  {
    return YT8512_STATUS_READ_ERROR;
  }
  
  if((readval & YT8512_BSR_LINK_STATUS) == 0)
  {
    /* Return Link Down status */
    return YT8512_STATUS_LINK_DOWN;    
  }
	
	if((readval & YT8512_BSR_AUTONEGO_ABILITY) == YT8512_BSR_AUTONEGO_ABILITY)
	{
		if((readval & YT8512_BSR_AUTONEGO_CPLT) == 0)
    {
      return YT8512_STATUS_AUTONEGO_NOTDONE;
    }
		
		if(((readval & YT8512_BSR_100BASE_TX_FD) == YT8512_BSR_100BASE_TX_FD)) 
    {
      return YT8512_STATUS_AUTO_100MBITS_FULLDUPLEX;
    }
    else if ((readval & YT8512_BSR_100BASE_TX_HD) == YT8512_BSR_100BASE_TX_HD)
    {
      return YT8512_STATUS_AUTO_100MBITS_HALFDUPLEX;
    }        
    else if ((readval & YT8512_BSR_10BASE_T_FD) == YT8512_BSR_10BASE_T_FD)
    {
      return YT8512_STATUS_AUTO_10MBITS_FULLDUPLEX;
    }
    else
    {
      return YT8512_STATUS_AUTO_10MBITS_HALFDUPLEX;
    }  		
	}
	else
	{
		if(((readval & YT8512_BSR_100BASE_TX_FD) == YT8512_BSR_100BASE_TX_FD)) 
    {
      return YT8512_STATUS_100MBITS_FULLDUPLEX;
    }
    else if ((readval & YT8512_BSR_100BASE_TX_HD) == YT8512_BSR_100BASE_TX_HD)
    {
      return YT8512_STATUS_100MBITS_HALFDUPLEX;
    }        
    else if ((readval & YT8512_BSR_10BASE_T_FD) == YT8512_BSR_10BASE_T_FD)
    {
      return YT8512_STATUS_10MBITS_FULLDUPLEX;
    }
    else
    {
      return YT8512_STATUS_10MBITS_HALFDUPLEX;
    } 
	}
  
  /* Check Auto negotiaition */
//  if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BCR, &readval) < 0)
//  {
//    return YT8512_STATUS_READ_ERROR;
//  }
//  
//  if((readval & YT8512_BCR_AUTONEGO_EN) != YT8512_BCR_AUTONEGO_EN)
//  {
//    if(((readval & YT8512_BCR_SPEED_SELECT) == YT8512_BCR_SPEED_SELECT) && ((readval & YT8512_BCR_DUPLEX_MODE) == YT8512_BCR_DUPLEX_MODE)) 
//    {
//      return YT8512_STATUS_100MBITS_FULLDUPLEX;
//    }
//    else if ((readval & YT8512_BCR_SPEED_SELECT) == YT8512_BCR_SPEED_SELECT)
//    {
//      return YT8512_STATUS_100MBITS_HALFDUPLEX;
//    }        
//    else if ((readval & YT8512_BCR_DUPLEX_MODE) == YT8512_BCR_DUPLEX_MODE)
//    {
//      return YT8512_STATUS_10MBITS_FULLDUPLEX;
//    }
//    else
//    {
//      return YT8512_STATUS_10MBITS_HALFDUPLEX;
//    }  		
//  }
//  else /* Auto Nego enabled */
//  {
//    if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_PHYSR, &readval) < 0)
//    {
//      return YT8512_STATUS_READ_ERROR;
//    }
//    
//    /* Check if auto nego not done */
//    if((readval & YT8512_PHYSR_AUTONEGO_DONE) == 0)
//    {
//      return YT8512_STATUS_AUTONEGO_NOTDONE;
//    }
//    
//    if((readval & YT8512_PHYSR_HCDSPEEDMASK) == YT8512_PHYSR_100BTX_FD)
//    {
//      return YT8512_STATUS_100MBITS_FULLDUPLEX;
//    }
//    else if ((readval & YT8512_PHYSR_HCDSPEEDMASK) == YT8512_PHYSR_100BTX_HD)
//    {
//      return YT8512_STATUS_100MBITS_HALFDUPLEX;
//    }
//    else if ((readval & YT8512_PHYSR_HCDSPEEDMASK) == YT8512_PHYSR_10BT_FD)
//    {
//      return YT8512_STATUS_10MBITS_FULLDUPLEX;
//    }
//    else
//    {
//      return YT8512_STATUS_10MBITS_HALFDUPLEX;
//    }				
//  }
}

/**
  * @brief  Set the link state of YT8512 device.
  * @param  pObj: Pointer to device object. 
  * @param  pLinkState: link state can be one of the following
  *         YT8512_STATUS_100MBITS_FULLDUPLEX if 100Mb/s FD
  *         YT8512_STATUS_100MBITS_HALFDUPLEX if 100Mb/s HD
  *         YT8512_STATUS_10MBITS_FULLDUPLEX  if 10Mb/s FD
  *         YT8512_STATUS_10MBITS_HALFDUPLEX  if 10Mb/s HD   
  * @retval YT8512_STATUS_OK  if OK
  *         YT8512_STATUS_ERROR  if parameter error  
  *         YT8512_STATUS_READ_ERROR if connot read register
  *         YT8512_STATUS_WRITE_ERROR if connot write to register
  */
int32_t YT8512_SetLinkState(YT8512_Object_t *pObj, uint32_t LinkState)
{
  uint32_t bcrvalue = 0;
  int32_t status = YT8512_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BCR, &bcrvalue) >= 0)
  {
    /* Disable link config (Auto nego, speed and duplex) */
    bcrvalue &= ~(YT8512_BCR_AUTONEGO_EN | YT8512_BCR_SPEED_SELECT | YT8512_BCR_DUPLEX_MODE);
    
    if(LinkState == YT8512_STATUS_100MBITS_FULLDUPLEX)
    {
      bcrvalue |= (YT8512_BCR_SPEED_SELECT | YT8512_BCR_DUPLEX_MODE);
    }
    else if (LinkState == YT8512_STATUS_100MBITS_HALFDUPLEX)
    {
      bcrvalue |= YT8512_BCR_SPEED_SELECT;
    }
    else if (LinkState == YT8512_STATUS_10MBITS_FULLDUPLEX)
    {
      bcrvalue |= YT8512_BCR_DUPLEX_MODE;
    }
    else
    {
      /* Wrong link status parameter */
      status = YT8512_STATUS_ERROR;
    }	
  }
  else
  {
    status = YT8512_STATUS_READ_ERROR;
  }
  
  if(status == YT8512_STATUS_OK)
  {
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, YT8512_BCR, bcrvalue) < 0)
    {
      status = YT8512_STATUS_WRITE_ERROR;
    }
  }
  
  return status;
}

/**
  * @brief  Enable loopback mode.
  * @param  pObj: Pointer to device object. 
  * @retval YT8512_STATUS_OK  if OK
  *         YT8512_STATUS_READ_ERROR if connot read register
  *         YT8512_STATUS_WRITE_ERROR if connot write to register
  */
int32_t YT8512_EnableLoopbackMode(YT8512_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = YT8512_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BCR, &readval) >= 0)
  {
    readval |= YT8512_BCR_LOOPBACK;
    
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, YT8512_BCR, readval) < 0)
    {
      status = YT8512_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = YT8512_STATUS_READ_ERROR;
  }
  
  return status;
}

/**
  * @brief  Disable loopback mode.
  * @param  pObj: Pointer to device object. 
  * @retval YT8512_STATUS_OK  if OK
  *         YT8512_STATUS_READ_ERROR if connot read register
  *         YT8512_STATUS_WRITE_ERROR if connot write to register
  */
int32_t YT8512_DisableLoopbackMode(YT8512_Object_t *pObj)
{
  uint32_t readval = 0;
  int32_t status = YT8512_STATUS_OK;
  
  if(pObj->IO.ReadReg(pObj->DevAddr, YT8512_BCR, &readval) >= 0)
  {
    readval &= ~YT8512_BCR_LOOPBACK;
  
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, YT8512_BCR, readval) < 0)
    {
      status =  YT8512_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = YT8512_STATUS_READ_ERROR;
  }
   
  return status;
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */      
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
