#ifndef __YT8512_H__
#define __YT8512_H__


#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Component
  * @{
  */
    
/** @defgroup YT8512
  * @{
  */    
/* Exported constants --------------------------------------------------------*/
/** @defgroup YT8512_Exported_Constants YT8512 Exported Constants
  * @{
  */ 
  
/** @defgroup YT8512_Registers_Mapping YT8512 Registers Mapping
  * @{
  */ 
#define YT8512_BCR      ((uint16_t)0x0000U)
#define YT8512_BSR      ((uint16_t)0x0001U)
#define YT8512_PHYI1R   ((uint16_t)0x0002U)
#define YT8512_PHYI2R   ((uint16_t)0x0003U)
#define YT8512_ANAR     ((uint16_t)0x0004U)
#define YT8512_ANLPAR   ((uint16_t)0x0005U)
#define YT8512_ANER     ((uint16_t)0x0006U)
#define YT8512_ANNPTR   ((uint16_t)0x0007U)
#define YT8512_ANNPRR   ((uint16_t)0x0008U)
#define YT8512_MMDACR   ((uint16_t)0x000DU)
#define YT8512_MMDAADR  ((uint16_t)0x000EU)


#define YT8512_PHYSCR	  ((uint16_t)0x0010U)
#define YT8512_PHYSR  	((uint16_t)0x0011U)
#define YT8512_IMR      ((uint16_t)0x0012U)		
#define YT8512_ISFR     ((uint16_t)0x0013U)		
#define YT8512_SMR      ((uint16_t)0x0012U)		

/**
  * @}
  */

/** @defgroup YT8512_BCR_Bit_Definition YT8512 BCR Bit Definition
  * @{
  */    
#define YT8512_BCR_SOFT_RESET         ((uint16_t)0x8000U)
#define YT8512_BCR_LOOPBACK           ((uint16_t)0x4000U)
#define YT8512_BCR_SPEED_SELECT       ((uint16_t)0x2000U)
#define YT8512_BCR_AUTONEGO_EN        ((uint16_t)0x1000U)
#define YT8512_BCR_POWER_DOWN         ((uint16_t)0x0800U)
#define YT8512_BCR_ISOLATE            ((uint16_t)0x0400U)
#define YT8512_BCR_RESTART_AUTONEGO   ((uint16_t)0x0200U)
#define YT8512_BCR_DUPLEX_MODE        ((uint16_t)0x0100U) 
/**
  * @}
  */

/** @defgroup YT8512_BSR_Bit_Definition YT8512 BSR Bit Definition
  * @{
  */   
#define YT8512_BSR_100BASE_T4       ((uint16_t)0x8000U)
#define YT8512_BSR_100BASE_TX_FD    ((uint16_t)0x4000U)
#define YT8512_BSR_100BASE_TX_HD    ((uint16_t)0x2000U)
#define YT8512_BSR_10BASE_T_FD      ((uint16_t)0x1000U)
#define YT8512_BSR_10BASE_T_HD      ((uint16_t)0x0800U)
#define YT8512_BSR_100BASE_T2_FD    ((uint16_t)0x0400U)
#define YT8512_BSR_100BASE_T2_HD    ((uint16_t)0x0200U)
#define YT8512_BSR_EXTENDED_STATUS  ((uint16_t)0x0100U)
#define YT8512_BSR_AUTONEGO_CPLT    ((uint16_t)0x0020U)
#define YT8512_BSR_REMOTE_FAULT     ((uint16_t)0x0010U)
#define YT8512_BSR_AUTONEGO_ABILITY ((uint16_t)0x0008U)
#define YT8512_BSR_LINK_STATUS      ((uint16_t)0x0004U)
#define YT8512_BSR_JABBER_DETECT    ((uint16_t)0x0002U)
#define YT8512_BSR_EXTENDED_CAP     ((uint16_t)0x0001U)
/**
  * @}
  */

/** @defgroup YT8512_PHYI1R_Bit_Definition YT8512 PHYI1R Bit Definition
  * @{
  */
#define YT8512_PHYI1R_OUI_3_18           ((uint16_t)0xFFFFU)
/**
  * @}
  */

/** @defgroup YT8512_PHYI2R_Bit_Definition YT8512 PHYI2R Bit Definition
  * @{
  */
#define YT8512_PHYI2R_OUI_19_24          ((uint16_t)0xFC00U)
#define YT8512_PHYI2R_MODEL_NBR          ((uint16_t)0x03F0U)
#define YT8512_PHYI2R_REVISION_NBR       ((uint16_t)0x000FU)
/**
  * @}
  */

/** @defgroup YT8512_ANAR_Bit_Definition YT8512 ANAR Bit Definition
  * @{
  */
#define YT8512_ANAR_NEXT_PAGE               ((uint16_t)0x8000U)
#define YT8512_ANAR_REMOTE_FAULT            ((uint16_t)0x2000U)
#define YT8512_ANAR_PAUSE_OPERATION         ((uint16_t)0x0C00U)
#define YT8512_ANAR_PO_NOPAUSE              ((uint16_t)0x0000U)
#define YT8512_ANAR_PO_SYMMETRIC_PAUSE      ((uint16_t)0x0400U)
#define YT8512_ANAR_PO_ASYMMETRIC_PAUSE     ((uint16_t)0x0800U)
#define YT8512_ANAR_PO_ADVERTISE_SUPPORT    ((uint16_t)0x0C00U)
#define YT8512_ANAR_100BASE_TX_FD           ((uint16_t)0x0100U)
#define YT8512_ANAR_100BASE_TX              ((uint16_t)0x0080U)
#define YT8512_ANAR_10BASE_T_FD             ((uint16_t)0x0040U)
#define YT8512_ANAR_10BASE_T                ((uint16_t)0x0020U)
#define YT8512_ANAR_SELECTOR_FIELD          ((uint16_t)0x000FU)
/**
  * @}
  */

/** @defgroup YT8512_ANLPAR_Bit_Definition YT8512 ANLPAR Bit Definition
  * @{
  */
#define YT8512_ANLPAR_NEXT_PAGE            ((uint16_t)0x8000U)
#define YT8512_ANLPAR_REMOTE_FAULT         ((uint16_t)0x2000U)
#define YT8512_ANLPAR_PAUSE_OPERATION      ((uint16_t)0x0C00U)
#define YT8512_ANLPAR_PO_NOPAUSE           ((uint16_t)0x0000U)
#define YT8512_ANLPAR_PO_SYMMETRIC_PAUSE   ((uint16_t)0x0400U)
#define YT8512_ANLPAR_PO_ASYMMETRIC_PAUSE  ((uint16_t)0x0800U)
#define YT8512_ANLPAR_PO_ADVERTISE_SUPPORT ((uint16_t)0x0C00U)
#define YT8512_ANLPAR_100BASE_TX_FD        ((uint16_t)0x0100U)
#define YT8512_ANLPAR_100BASE_TX           ((uint16_t)0x0080U)
#define YT8512_ANLPAR_10BASE_T_FD          ((uint16_t)0x0040U)
#define YT8512_ANLPAR_10BASE_T             ((uint16_t)0x0020U)
#define YT8512_ANLPAR_SELECTOR_FIELD       ((uint16_t)0x000FU)
/**
  * @}
  */

/** @defgroup YT8512_ANER_Bit_Definition YT8512 ANER Bit Definition
  * @{
  */
#define YT8512_ANER_RX_NP_LOCATION_ABLE    ((uint16_t)0x0040U)
#define YT8512_ANER_RX_NP_STORAGE_LOCATION ((uint16_t)0x0020U)
#define YT8512_ANER_PARALLEL_DETECT_FAULT  ((uint16_t)0x0010U)
#define YT8512_ANER_LP_NP_ABLE             ((uint16_t)0x0008U)
#define YT8512_ANER_NP_ABLE                ((uint16_t)0x0004U)
#define YT8512_ANER_PAGE_RECEIVED          ((uint16_t)0x0002U)
#define YT8512_ANER_LP_AUTONEG_ABLE        ((uint16_t)0x0001U)
/**
  * @}
  */

/** @defgroup YT8512_ANNPTR_Bit_Definition YT8512 ANNPTR Bit Definition
  * @{
  */
#define YT8512_ANNPTR_NEXT_PAGE         ((uint16_t)0x8000U)
#define YT8512_ANNPTR_MESSAGE_PAGE      ((uint16_t)0x2000U)
#define YT8512_ANNPTR_ACK2              ((uint16_t)0x1000U)
#define YT8512_ANNPTR_TOGGLE            ((uint16_t)0x0800U)
#define YT8512_ANNPTR_MESSAGGE_CODE     ((uint16_t)0x07FFU)
/**
  * @}
  */

/** @defgroup YT8512_ANNPRR_Bit_Definition YT8512 ANNPRR Bit Definition
  * @{
  */
#define YT8512_ANNPTR_NEXT_PAGE         ((uint16_t)0x8000U)
#define YT8512_ANNPRR_ACK               ((uint16_t)0x4000U)
#define YT8512_ANNPRR_MESSAGE_PAGE      ((uint16_t)0x2000U)
#define YT8512_ANNPRR_ACK2              ((uint16_t)0x1000U)
#define YT8512_ANNPRR_TOGGLE            ((uint16_t)0x0800U)
#define YT8512_ANNPRR_MESSAGGE_CODE     ((uint16_t)0x07FFU)
/**
  * @}
  */

/** @defgroup YT8512_MMDACR_Bit_Definition YT8512 MMDACR Bit Definition
  * @{
  */
#define YT8512_MMDACR_MMD_FUNCTION       ((uint16_t)0xC000U) 
#define YT8512_MMDACR_MMD_FUNCTION_ADDR  ((uint16_t)0x0000U)
#define YT8512_MMDACR_MMD_FUNCTION_DATA  ((uint16_t)0x4000U)
#define YT8512_MMDACR_MMD_DEV_ADDR       ((uint16_t)0x001FU)
/**
  * @}
  */
	
/** @defgroup YT8512_PHYSCSR_Bit_Definition YT8512 PHYSCR Bit Definition
  * @{
  */
#define YT8512_PHYSCR_DEFAULT			   ((uint16_t)0x0062U)
/**
  * @}
  */
	
/** @defgroup YT8512_PHYSCSR_Bit_Definition YT8512 PHYSR Bit Definition
  * @{
  */
#define YT8512_PHYSR_AUTONEGO_DONE   ((uint16_t)0x0800U)
#define YT8512_PHYSR_HCDSPEEDMASK    ((uint16_t)0xE000U)
#define YT8512_PHYSR_10BT_HD         ((uint16_t)0x0000U)
#define YT8512_PHYSR_10BT_FD         ((uint16_t)0x2000U)
#define YT8512_PHYSR_100BTX_HD       ((uint16_t)0x4000U)
#define YT8512_PHYSR_100BTX_FD       ((uint16_t)0x6000U) 
/**
  * @}
  */
    
/** @defgroup YT8512_Status YT8512 Status
  * @{
  */    

#define  YT8512_STATUS_READ_ERROR            			((int32_t)-5)
#define  YT8512_STATUS_WRITE_ERROR           			((int32_t)-4)
#define  YT8512_STATUS_ADDRESS_ERROR         			((int32_t)-3)
#define  YT8512_STATUS_RESET_TIMEOUT         			((int32_t)-2)
#define  YT8512_STATUS_ERROR                 			((int32_t)-1)
#define  YT8512_STATUS_OK                    			((int32_t) 0)
#define  YT8512_STATUS_LINK_DOWN             			((int32_t) 1)
#define  YT8512_STATUS_AUTO_100MBITS_FULLDUPLEX   ((int32_t) 2)
#define  YT8512_STATUS_AUTO_100MBITS_HALFDUPLEX   ((int32_t) 3)
#define  YT8512_STATUS_AUTO_10MBITS_FULLDUPLEX    ((int32_t) 4)
#define  YT8512_STATUS_AUTO_10MBITS_HALFDUPLEX    ((int32_t) 5)
#define  YT8512_STATUS_AUTONEGO_NOTDONE      			((int32_t) 6)
#define  YT8512_STATUS_100MBITS_FULLDUPLEX   			((int32_t) 7)
#define  YT8512_STATUS_100MBITS_HALFDUPLEX   			((int32_t) 8)
#define  YT8512_STATUS_10MBITS_FULLDUPLEX    			((int32_t) 9)
#define  YT8512_STATUS_10MBITS_HALFDUPLEX    			((int32_t) 10)
/**
  * @}
  */

/* PHY地址 ---- 由用户设置 */
#define YT8512_ADDR                           ((uint16_t)0x03U)
/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup YT8512_Exported_Types YT8512 Exported Types
  * @{
  */
typedef int32_t  (*YT8512_Init_Func) (void); 
typedef int32_t  (*YT8512_DeInit_Func) (void);
typedef int32_t  (*YT8512_ReadReg_Func)   (uint32_t, uint32_t, uint32_t *);
typedef int32_t  (*YT8512_WriteReg_Func)  (uint32_t, uint32_t, uint32_t);
typedef int32_t  (*YT8512_GetTick_Func)  (void);

typedef struct 
{                   
  YT8512_Init_Func      Init; 
  YT8512_DeInit_Func    DeInit;
  YT8512_WriteReg_Func  WriteReg;
  YT8512_ReadReg_Func   ReadReg; 
  YT8512_GetTick_Func   GetTick;   
} YT8512_IOCtx_t;  

  
typedef struct 
{
  uint32_t            DevAddr;
  uint32_t            Is_Initialized;
  YT8512_IOCtx_t     IO;
  void               *pData;
}YT8512_Object_t;
/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup YT8512_Exported_Functions YT8512 Exported Functions
  * @{
  */
int32_t YT8512_RegisterBusIO(YT8512_Object_t *pObj, YT8512_IOCtx_t *ioctx);
int32_t YT8512_Init(YT8512_Object_t *pObj);
int32_t YT8512_DeInit(YT8512_Object_t *pObj);
int32_t YT8512_DisablePowerDownMode(YT8512_Object_t *pObj);
int32_t YT8512_EnablePowerDownMode(YT8512_Object_t *pObj);
int32_t YT8512_StartAutoNego(YT8512_Object_t *pObj);
int32_t YT8512_GetLinkState(YT8512_Object_t *pObj);
int32_t YT8512_SetLinkState(YT8512_Object_t *pObj, uint32_t LinkState);
int32_t YT8512_EnableLoopbackMode(YT8512_Object_t *pObj);
int32_t YT8512_DisableLoopbackMode(YT8512_Object_t *pObj);
int32_t YT8512_EnableIT(YT8512_Object_t *pObj, uint32_t Interrupt);
int32_t YT8512_DisableIT(YT8512_Object_t *pObj, uint32_t Interrupt);
int32_t YT8512_ClearIT(YT8512_Object_t *pObj, uint32_t Interrupt);
int32_t YT8512_GetITStatus(YT8512_Object_t *pObj, uint32_t Interrupt);
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif
#endif /* YT8512_H */

