/* 文件: stm32f4xx_hal_cryp.c | 编码: UTF-8 | 每行代码附中文注释 */
/**
  ******************************************************************************
  * @file    stm32f4xx_hal_cryp.c
  * @author  MCD Application Team
  * @brief   CRYP HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Cryptography (CRYP) peripheral:
  *           + Initialization, de-initialization, set config and get config  functions
  *           + DES/TDES, AES processing functions
  *           + DMA callback functions
  *           + CRYP IRQ handler management
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
      The CRYP HAL driver can be used in CRYP or TinyAES IP as follows:

      (#)Initialize the CRYP low level resources by implementing the HAL_CRYP_MspInit():
         (##) Enable the CRYP interface clock using __HAL_RCC_CRYP_CLK_ENABLE()or __HAL_RCC_AES_CLK_ENABLE for TinyAES IP
         (##) In case of using interrupts (e.g. HAL_CRYP_Encrypt_IT())
             (+++) Configure the CRYP interrupt priority using HAL_NVIC_SetPriority()
             (+++) Enable the CRYP IRQ handler using HAL_NVIC_EnableIRQ()
             (+++) In CRYP IRQ handler, call HAL_CRYP_IRQHandler()
         (##) In case of using DMA to control data transfer (e.g. HAL_CRYP_Encrypt_DMA())
             (+++) Enable the DMAx interface clock using __RCC_DMAx_CLK_ENABLE()
             (+++) Configure and enable two DMA streams one for managing data transfer from
                 memory to peripheral (input stream) and another stream for managing data
                 transfer from peripheral to memory (output stream)
             (+++) Associate the initialized DMA handle to the CRYP DMA handle
                 using  __HAL_LINKDMA()
             (+++) Configure the priority and enable the NVIC for the transfer complete
                 interrupt on the two DMA Streams. The output stream should have higher
                 priority than the input stream HAL_NVIC_SetPriority() and HAL_NVIC_EnableIRQ()

      (#)Initialize the CRYP according to the specified parameters :
         (##) The data type: 1-bit, 8-bit, 16-bit or 32-bit.
         (##) The key size: 128, 192 or 256.
         (##) The AlgoMode DES/ TDES Algorithm ECB/CBC or AES Algorithm ECB/CBC/CTR/GCM or CCM.
         (##) The initialization vector (counter). It is not used in ECB mode.
         (##) The key buffer used for encryption/decryption.
         (##) The Header used only in AES GCM and CCM Algorithm for authentication.
         (##) The HeaderSize The size of header buffer in word.
         (##) The B0 block is the first authentication block used only  in AES CCM mode.

      (#)Three processing (encryption/decryption) functions are available:
         (##) Polling mode: encryption and decryption APIs are blocking functions
              i.e. they process the data and wait till the processing is finished,
              e.g. HAL_CRYP_Encrypt & HAL_CRYP_Decrypt
         (##) Interrupt mode: encryption and decryption APIs are not blocking functions
              i.e. they process the data under interrupt,
              e.g. HAL_CRYP_Encrypt_IT & HAL_CRYP_Decrypt_IT
         (##) DMA mode: encryption and decryption APIs are not blocking functions
              i.e. the data transfer is ensured by DMA,
              e.g. HAL_CRYP_Encrypt_DMA & HAL_CRYP_Decrypt_DMA

      (#)When the processing function is called at first time after HAL_CRYP_Init()
         the CRYP peripheral is configured and processes the buffer in input.
         At second call, no need to Initialize the CRYP, user have to get current configuration via
         HAL_CRYP_GetConfig() API, then only  HAL_CRYP_SetConfig() is requested to set
         new parametres, finally user can  start encryption/decryption.

       (#)Call HAL_CRYP_DeInit() to deinitialize the CRYP peripheral.

       (#)To process a single message with consecutive calls to HAL_CRYP_Encrypt() or HAL_CRYP_Decrypt()
          without having to configure again the Key or the Initialization Vector between each API call,
          the field KeyIVConfigSkip of the initialization structure must be set to CRYP_KEYIVCONFIG_ONCE.
          Same is true for consecutive calls of HAL_CRYP_Encrypt_IT(), HAL_CRYP_Decrypt_IT(), HAL_CRYP_Encrypt_DMA()
          or HAL_CRYP_Decrypt_DMA().

    [..]
      The cryptographic processor supports following standards:
      (#) The data encryption standard (DES) and Triple-DES (TDES) supported only by CRYP1 IP:
         (##)64-bit data block processing
         (##) chaining modes supported :
             (+++)  Electronic Code Book(ECB)
             (+++)  Cipher Block Chaining (CBC)
         (##) keys length supported :64-bit, 128-bit and 192-bit.
      (#) The advanced encryption standard (AES) supported  by CRYP1 & TinyAES IP:
         (##)128-bit data block processing
         (##) chaining modes supported :
             (+++)  Electronic Code Book(ECB)
             (+++)  Cipher Block Chaining (CBC)
             (+++)  Counter mode (CTR)
             (+++)  Galois/counter mode (GCM/GMAC)
             (+++)  Counter with Cipher Block Chaining-Message(CCM)
         (##) keys length Supported :
             (+++) for CRYP1 IP: 128-bit, 192-bit and 256-bit.
             (+++) for TinyAES IP:  128-bit and 256-bit

    [..]  This section describes the AES Galois/counter mode (GCM) supported by both CRYP1 IP:
      (#)  Algorithm supported :
         (##) Galois/counter mode (GCM)
         (##) Galois message authentication code (GMAC) :is exactly the same as
              GCM algorithm composed only by an header.
      (#)  Four phases are performed in GCM :
         (##) Init phase: IP prepares the GCM hash subkey (H) and do the IV processing
         (##) Header phase: IP processes the Additional Authenticated Data (AAD), with hash
          computation only.
         (##) Payload phase: IP processes the plaintext (P) with hash computation + keystream
          encryption + data XORing. It works in a similar way for ciphertext (C).
         (##) Final phase: IP generates the authenticated tag (T) using the last block of data.
      (#)  structure of message construction in GCM is defined as below  :
         (##) 16 bytes Initial Counter Block (ICB)composed of IV and counter
         (##) The authenticated header A (also knows as Additional Authentication Data AAD)
          this part of the message is only authenticated, not encrypted.
         (##) The plaintext message P is both authenticated and encrypted as ciphertext.
          GCM standard specifies that ciphertext has same bit length as the plaintext.
         (##) The last block is composed of the length of A (on 64 bits) and the length of ciphertext
          (on 64 bits)

    [..]  This section describe The AES Counter with Cipher Block Chaining-Message
          Authentication Code (CCM) supported by both CRYP1 IP:
      (#)  Specific parameters for CCM  :

         (##) B0 block  : According to NIST Special Publication 800-38C,
            The first block B0 is formatted as follows, where l(m) is encoded in
            most-significant-byte first order(see below table 3)

              (+++)  Q: a bit string representation of the octet length of P (plaintext)
              (+++)  q The octet length of the binary representation of the octet length of the payload
              (+++)  A nonce (N), n The octet length of the where n+q=15.
              (+++)  Flags: most significant octet containing four flags for control information,
              (+++)  t The octet length of the MAC.
         (##) B1 block (header) : associated data length(a) concatenated with Associated Data (A)
              the associated data length expressed in bytes (a) defined as below:
            (+++)  If 0 < a < 216-28, then it is encoded as [a]16, i.e. two octets
            (+++)  If 216-28 < a < 232, then it is encoded as 0xff || 0xfe || [a]32, i.e. six octets
            (+++)  If 232 < a < 264, then it is encoded as 0xff || 0xff || [a]64, i.e. ten octets
         (##) CTRx block  : control blocks
            (+++) Generation of CTR1 from first block B0 information :
              equal to B0 with first 5 bits zeroed and most significant bits storing octet
              length of P also zeroed, then incremented by one ( see below Table 4)
            (+++) Generation of CTR0: same as CTR1 with bit[0] set to zero.

      (#)  Four phases are performed in CCM for CRYP1 IP:
         (##) Init phase: IP prepares the GCM hash subkey (H) and do the IV processing
         (##) Header phase: IP processes the Additional Authenticated Data (AAD), with hash
          computation only.
         (##) Payload phase: IP processes the plaintext (P) with hash computation + keystream
          encryption + data XORing. It works in a similar way for ciphertext (C).
         (##) Final phase: IP generates the authenticated tag (T) using the last block of data.

  *** Callback registration ***
  =============================================

  The compilation define  USE_HAL_CRYP_REGISTER_CALLBACKS when set to 1
  allows the user to configure dynamically the driver callbacks.
  Use Functions @ref HAL_CRYP_RegisterCallback() or HAL_CRYP_RegisterXXXCallback()
  to register an interrupt callback.

  Function @ref HAL_CRYP_RegisterCallback() allows to register following callbacks:
    (+) InCpltCallback     :  Input FIFO transfer completed callback.
    (+) OutCpltCallback    : Output FIFO transfer completed callback.
    (+) ErrorCallback      : callback for error detection.
    (+) MspInitCallback    : CRYP MspInit.
    (+) MspDeInitCallback  : CRYP MspDeInit.
  This function takes as parameters the HAL peripheral handle, the Callback ID
  and a pointer to the user callback function.

  Use function @ref HAL_CRYP_UnRegisterCallback() to reset a callback to the default
  weak function.
  @ref HAL_CRYP_UnRegisterCallback() takes as parameters the HAL peripheral handle,
  and the Callback ID.
  This function allows to reset following callbacks:
    (+) InCpltCallback     :  Input FIFO transfer completed callback.
    (+) OutCpltCallback    : Output FIFO transfer completed callback.
    (+) ErrorCallback      : callback for error detection.
    (+) MspInitCallback    : CRYP MspInit.
    (+) MspDeInitCallback  : CRYP MspDeInit.

  By default, after the @ref HAL_CRYP_Init() and when the state is HAL_CRYP_STATE_RESET
  all callbacks are set to the corresponding weak functions :
  examples @ref HAL_CRYP_InCpltCallback() , @ref HAL_CRYP_OutCpltCallback().
  Exception done for MspInit and MspDeInit functions that are
  reset to the legacy weak function in the @ref HAL_CRYP_Init()/ @ref HAL_CRYP_DeInit() only when
  these callbacks are null (not registered beforehand).
  if not, MspInit or MspDeInit are not null, the @ref HAL_CRYP_Init() / @ref HAL_CRYP_DeInit()
  keep and use the user MspInit/MspDeInit functions (registered beforehand)

  Callbacks can be registered/unregistered in HAL_CRYP_STATE_READY state only.
  Exception done MspInit/MspDeInit callbacks that can be registered/unregistered
  in HAL_CRYP_STATE_READY or HAL_CRYP_STATE_RESET state,
  thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
  In that case first register the MspInit/MspDeInit user callbacks
  using @ref HAL_CRYP_RegisterCallback() before calling @ref HAL_CRYP_DeInit()
  or @ref HAL_CRYP_Init() function.

  When The compilation define USE_HAL_CRYP_REGISTER_CALLBACKS is set to 0 or
  not defined, the callback registration feature is not available and all callbacks
  are set to the corresponding weak functions.

  Table 1. Initial Counter Block (ICB)
          +-------------------------------------------------------+
          |       Initialization vector (IV)      |  Counter      |
          |----------------|----------------|-----------|---------|
         127              95                63            31       0


              Bit Number    Register           Contents
              ----------   ---------------       -----------
              127 ...96    CRYP_IV1R[31:0]     ICB[127:96]
              95  ...64    CRYP_IV1L[31:0]     B0[95:64]
              63 ... 32    CRYP_IV0R[31:0]     ICB[63:32]
              31 ... 0     CRYP_IV0L[31:0]     ICB[31:0], where 32-bit counter= 0x2

  Table 2.  GCM last block definition

          +-------------------------------------------------------------------+
          |  Bit[0]   |  Bit[32]           |  Bit[64]  | Bit[96]              |
          |-----------|--------------------|-----------|----------------------|
          |   0x0     | Header length[31:0]|     0x0   | Payload length[31:0] |
          |-----------|--------------------|-----------|----------------------|

  Table 3. B0 block
                Octet Number   Contents
                ------------   ---------
                0              Flags
                1 ... 15-q     Nonce N
                16-q ... 15    Q

            the Flags field is formatted as follows:

                Bit Number   Contents
                ----------   ----------------------
                7            Reserved (always zero)
                6            Adata
                5 ... 3      (t-2)/2
                2 ... 0      [q-1]3

 Table 4. CTRx block
                Bit Number    Register           Contents
                ----------   ---------------       -----------
                127 ...96    CRYP_IV1R[31:0]     B0[127:96], where Q length bits are set to 0, except for
                                                 bit 0 that is set to 1
                95  ...64    CRYP_IV1L[31:0]     B0[95:64]
                63 ... 32    CRYP_IV0R[31:0]     B0[63:32]
                31 ... 0     CRYP_IV0L[31:0]     B0[31:0], where flag bits set to 0

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
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
#include "stm32f4xx_hal.h"                              /* 包含 STM32/HAL 头文件 */

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

#if defined (AES)  || defined (CRYP)                    /* 条件编译 */

/** @defgroup CRYP CRYP
  * @brief CRYP HAL module driver.
  * @{
  */


#ifdef HAL_CRYP_MODULE_ENABLED                          /* 条件编译 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup CRYP_Private_Defines
  * @{
  */
#define CRYP_TIMEOUT_KEYPREPARATION      82U         /*The latency of key preparation operation is 82 clock cycles.*/
#define CRYP_TIMEOUT_GCMCCMINITPHASE     299U        /*  The latency of  GCM/CCM init phase to prepare hash subkey is 299 clock cycles.*/
#define CRYP_TIMEOUT_GCMCCMHEADERPHASE   290U        /*  The latency of  GCM/CCM header phase is 290 clock cycles.*/

#define  CRYP_PHASE_READY                0x00000001U /*!< CRYP peripheral is ready for initialization. */
#define  CRYP_PHASE_PROCESS              0x00000002U /*!< CRYP peripheral is in processing phase */

#if defined(AES)                                        /* 条件编译 */
#define CRYP_OPERATINGMODE_ENCRYPT                   0x00000000U     /*!< Encryption mode(Mode 1)  */
#define CRYP_OPERATINGMODE_KEYDERIVATION             AES_CR_MODE_0   /*!< Key derivation mode  only used when performing ECB and CBC decryptions (Mode 2) */
#define CRYP_OPERATINGMODE_DECRYPT                   AES_CR_MODE_1   /*!< Decryption    (Mode 3)    */
#define CRYP_OPERATINGMODE_KEYDERIVATION_DECRYPT     AES_CR_MODE     /*!< Key derivation and decryption only used when performing ECB and CBC decryptions (Mode 4) */
#define CRYP_PHASE_INIT                              0x00000000U        /*!< GCM/GMAC (or CCM) init phase */
#define CRYP_PHASE_HEADER                            AES_CR_GCMPH_0     /*!< GCM/GMAC or CCM header phase */
#define CRYP_PHASE_PAYLOAD                           AES_CR_GCMPH_1     /*!< GCM(/CCM) payload phase      */
#define CRYP_PHASE_FINAL                             AES_CR_GCMPH       /*!< GCM/GMAC or CCM  final phase */
#else  /* CRYP */
#define CRYP_PHASE_INIT                              0x00000000U             /*!< GCM/GMAC (or CCM) init phase */
#define CRYP_PHASE_HEADER                            CRYP_CR_GCM_CCMPH_0     /*!< GCM/GMAC or CCM header phase */
#define CRYP_PHASE_PAYLOAD                           CRYP_CR_GCM_CCMPH_1     /*!< GCM(/CCM) payload phase      */
#define CRYP_PHASE_FINAL                             CRYP_CR_GCM_CCMPH       /*!< GCM/GMAC or CCM  final phase */
#define CRYP_OPERATINGMODE_ENCRYPT                   0x00000000U             /*!< Encryption mode   */
#define CRYP_OPERATINGMODE_DECRYPT                   CRYP_CR_ALGODIR         /*!< Decryption        */
#endif /* End CRYP or  AES */

/*  CTR1 information to use in CCM algorithm */
#define CRYP_CCM_CTR1_0                  0x07FFFFFFU    /* 定义宏 */
#define CRYP_CCM_CTR1_1                  0xFFFFFF00U    /* 定义宏 */
#define CRYP_CCM_CTR1_2                  0x00000001U    /* 定义宏 */


/**
  * @}
  */


/* Private macro -------------------------------------------------------------*/
/** @addtogroup CRYP_Private_Macros
  * @{
  */

#if defined(CRYP)                                       /* 条件编译 */

#define CRYP_SET_PHASE(__HANDLE__, __PHASE__)  do{(__HANDLE__)->Instance->CR &= (uint32_t)(~CRYP_CR_GCM_CCMPH);\
                                                        (__HANDLE__)->Instance->CR |= (uint32_t)(__PHASE__);\
                                                       }while(0)  /* 代码块结束 */

#define HAL_CRYP_FIFO_FLUSH(__HANDLE__) ((__HANDLE__)->Instance->CR |=  CRYP_CR_FFLUSH)  /* 定义宏 */

#else /*AES*/
#define CRYP_SET_PHASE(__HANDLE__, __PHASE__)  do{(__HANDLE__)->Instance->CR &= (uint32_t)(~AES_CR_GCMPH);\
                                                        (__HANDLE__)->Instance->CR |= (uint32_t)(__PHASE__);\
                                                       }while(0)  /* 代码块结束 */
#endif /* End AES or CRYP*/


/**
  * @}
  */

/* Private struct -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup CRYP_Private_Functions_prototypes
  * @{
  */

static void CRYP_SetDMAConfig(CRYP_HandleTypeDef *hcryp, uint32_t inputaddr, uint16_t Size, uint32_t outputaddr);  /* 静态函数 */
static void CRYP_DMAInCplt(DMA_HandleTypeDef *hdma);    /* 静态函数 */
static void CRYP_DMAOutCplt(DMA_HandleTypeDef *hdma);   /* 静态函数 */
static void CRYP_DMAError(DMA_HandleTypeDef *hdma);     /* 静态函数 */
static void CRYP_SetKey(CRYP_HandleTypeDef *hcryp, uint32_t KeySize);  /* 静态函数 */
static void CRYP_AES_IT(CRYP_HandleTypeDef *hcryp);     /* 静态函数 */
#if defined (CRYP_CR_ALGOMODE_AES_GCM)|| defined (AES)  /* 条件编译 */
static HAL_StatusTypeDef CRYP_GCMCCM_SetHeaderPhase(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
static void CRYP_GCMCCM_SetPayloadPhase_IT(CRYP_HandleTypeDef *hcryp);  /* 静态函数 */
static void CRYP_GCMCCM_SetHeaderPhase_IT(CRYP_HandleTypeDef *hcryp);  /* 静态函数 */
static HAL_StatusTypeDef CRYP_GCMCCM_SetHeaderPhase_DMA(CRYP_HandleTypeDef *hcryp);  /* 静态变量/函数 */
static void CRYP_Workaround(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态函数 */
static HAL_StatusTypeDef CRYP_AESGCM_Process_DMA(CRYP_HandleTypeDef *hcryp);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_AESGCM_Process_IT(CRYP_HandleTypeDef *hcryp);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_AESGCM_Process(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_AESCCM_Process(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_AESCCM_Process_IT(CRYP_HandleTypeDef *hcryp);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_AESCCM_Process_DMA(CRYP_HandleTypeDef *hcryp);  /* 静态变量/函数 */
#endif /* AES or GCM CCM defined*/
static void CRYP_AES_ProcessData(CRYP_HandleTypeDef *hcrypt, uint32_t Timeout);  /* 静态函数 */
static HAL_StatusTypeDef CRYP_AES_Encrypt(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_AES_Decrypt(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_AES_Decrypt_IT(CRYP_HandleTypeDef *hcryp);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_AES_Encrypt_IT(CRYP_HandleTypeDef *hcryp);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_AES_Decrypt_DMA(CRYP_HandleTypeDef *hcryp);  /* 静态变量/函数 */
#if defined (CRYP)                                      /* 条件编译 */
static void CRYP_TDES_IT(CRYP_HandleTypeDef *hcryp);    /* 静态函数 */
#if defined (CRYP_CR_ALGOMODE_AES_GCM)                  /* 条件编译 */
static HAL_StatusTypeDef CRYP_WaitOnIFEMFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
#endif /* GCM CCM defined*/
static HAL_StatusTypeDef CRYP_WaitOnBUSYFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_WaitOnOFNEFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
static HAL_StatusTypeDef CRYP_TDES_Process(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
#else /*AES*/
static HAL_StatusTypeDef CRYP_WaitOnCCFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout);  /* 静态变量/函数 */
#endif /* End CRYP or AES */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @defgroup CRYP_Exported_Functions CRYP Exported Functions
  * @{
  */


/** @defgroup CRYP_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions.
  *
@verbatim
  ========================================================================================
     ##### Initialization, de-initialization and Set and Get configuration functions #####
  ========================================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize the CRYP
      (+) DeInitialize the CRYP
      (+) Initialize the CRYP MSP
      (+) DeInitialize the CRYP MSP
      (+) configure CRYP (HAL_CRYP_SetConfig) with the specified parameters in the CRYP_ConfigTypeDef
          Parameters which are configured in This section are :
          (+) Key size
          (+) Data Type : 32,16, 8 or 1bit
          (+) AlgoMode :
              - for CRYP1 IP :
                 ECB and CBC in DES/TDES Standard
                 ECB,CBC,CTR,GCM/GMAC and CCM in AES Standard.
              - for TinyAES2 IP, only ECB,CBC,CTR,GCM/GMAC and CCM in AES Standard are supported.
      (+) Get CRYP configuration (HAL_CRYP_GetConfig) from the specified parameters in the CRYP_HandleTypeDef


@endverbatim
  * @{
  */


/**
  * @brief  Initializes the CRYP according to the specified
  *         parameters in the CRYP_ConfigTypeDef and creates the associated handle.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_Init(CRYP_HandleTypeDef *hcryp)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the CRYP handle allocation */
  if (hcryp == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check parameters */
  assert_param(IS_CRYP_KEYSIZE(hcryp->Init.KeySize));   /* 执行语句 */
  assert_param(IS_CRYP_DATATYPE(hcryp->Init.DataType));  /* 执行语句 */
  assert_param(IS_CRYP_ALGORITHM(hcryp->Init.Algorithm));  /* 执行语句 */
  assert_param(IS_CRYP_INIT(hcryp->Init.KeyIVConfigSkip));  /* 执行语句 */

#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  if (hcryp->State == HAL_CRYP_STATE_RESET)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hcryp->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */

    hcryp->InCpltCallback  = HAL_CRYP_InCpltCallback;  /* Legacy weak InCpltCallback   */
    hcryp->OutCpltCallback = HAL_CRYP_OutCpltCallback; /* Legacy weak OutCpltCallback  */
    hcryp->ErrorCallback   = HAL_CRYP_ErrorCallback;   /* Legacy weak ErrorCallback    */

    if (hcryp->MspInitCallback == NULL)                 /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->MspInitCallback = HAL_CRYP_MspInit; /* Legacy weak MspInit  */
    }                                                   /* 代码块结束 */

    /* Init the low level hardware */
    hcryp->MspInitCallback(hcryp);                      /* 执行语句 */
  }                                                     /* 代码块结束 */
#else                                                   /* 条件编译否则 */
  if (hcryp->State == HAL_CRYP_STATE_RESET)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Allocate lock resource and initialize it */
    hcryp->Lock = HAL_UNLOCKED;                         /* 赋值或初始化 */

    /* Init the low level hardware */
    HAL_CRYP_MspInit(hcryp);                            /* HAL库函数调用 */
  }                                                     /* 代码块结束 */
#endif /* (USE_HAL_CRYP_REGISTER_CALLBACKS) */

  /* Set the key size(This bit field is don抰 care in the DES or TDES modes) data type and Algorithm */
#if defined (CRYP)                                      /* 条件编译 */

  MODIFY_REG(hcryp->Instance->CR, CRYP_CR_DATATYPE | CRYP_CR_KEYSIZE | CRYP_CR_ALGOMODE,  /* 列表续行 */
             hcryp->Init.DataType | hcryp->Init.KeySize | hcryp->Init.Algorithm);  /* 执行语句 */

#else /*AES*/

  MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE | AES_CR_KEYSIZE | AES_CR_CHMOD,  /* 列表续行 */
             hcryp->Init.DataType | hcryp->Init.KeySize | hcryp->Init.Algorithm);  /* 执行语句 */

#endif  /* End AES or CRYP*/

  /* Reset Error Code field */
  hcryp->ErrorCode = HAL_CRYP_ERROR_NONE;               /* 赋值或初始化 */

  /* Change the CRYP state */
  hcryp->State = HAL_CRYP_STATE_READY;                  /* 赋值或初始化 */

  /* Set the default CRYP phase */
  hcryp->Phase = CRYP_PHASE_READY;                      /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  De-Initializes the CRYP peripheral.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_DeInit(CRYP_HandleTypeDef *hcryp)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the CRYP handle allocation */
  if (hcryp == NULL)                                    /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Set the default CRYP phase */
  hcryp->Phase = CRYP_PHASE_READY;                      /* 赋值或初始化 */

  /* Reset CrypInCount and CrypOutCount */
  hcryp->CrypInCount = 0;                               /* 赋值或初始化 */
  hcryp->CrypOutCount = 0;                              /* 赋值或初始化 */
  hcryp->CrypHeaderCount = 0;                           /* 赋值或初始化 */

  /* Disable the CRYP peripheral clock */
  __HAL_CRYP_DISABLE(hcryp);                            /* HAL辅助宏/函数 */

#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */

  if (hcryp->MspDeInitCallback == NULL)                 /* 条件判断 */
  {                                                     /* 代码块开始 */
    hcryp->MspDeInitCallback = HAL_CRYP_MspDeInit; /* Legacy weak MspDeInit  */
  }                                                     /* 代码块结束 */
  /* DeInit the low level hardware */
  hcryp->MspDeInitCallback(hcryp);                      /* 执行语句 */

#else                                                   /* 条件编译否则 */

  /* DeInit the low level hardware: CLOCK, NVIC.*/
  HAL_CRYP_MspDeInit(hcryp);                            /* HAL库函数调用 */

#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */

  /* Change the CRYP state */
  hcryp->State = HAL_CRYP_STATE_RESET;                  /* 赋值或初始化 */

  /* Release Lock */
  __HAL_UNLOCK(hcryp);                                  /* HAL辅助宏/函数 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Configure the CRYP according to the specified
  *         parameters in the CRYP_ConfigTypeDef
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure
  * @param  pConf: pointer to a CRYP_ConfigTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_SetConfig(CRYP_HandleTypeDef *hcryp, CRYP_ConfigTypeDef *pConf)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the CRYP handle allocation */
  if ((hcryp == NULL) || (pConf == NULL))               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Check parameters */
  assert_param(IS_CRYP_KEYSIZE(pConf->KeySize));        /* 执行语句 */
  assert_param(IS_CRYP_DATATYPE(pConf->DataType));      /* 执行语句 */
  assert_param(IS_CRYP_ALGORITHM(pConf->Algorithm));    /* 执行语句 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Change the CRYP state */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /* Set  CRYP parameters  */
    hcryp->Init.DataType   = pConf->DataType;           /* 赋值或初始化 */
    hcryp->Init.pKey       = pConf->pKey;               /* 赋值或初始化 */
    hcryp->Init.Algorithm  = pConf->Algorithm;          /* 赋值或初始化 */
    hcryp->Init.KeySize    = pConf->KeySize;            /* 赋值或初始化 */
    hcryp->Init.pInitVect  = pConf->pInitVect;          /* 赋值或初始化 */
    hcryp->Init.Header     = pConf->Header;             /* 赋值或初始化 */
    hcryp->Init.HeaderSize = pConf->HeaderSize;         /* 赋值或初始化 */
    hcryp->Init.B0         = pConf->B0;                 /* 赋值或初始化 */
    hcryp->Init.DataWidthUnit = pConf->DataWidthUnit;   /* 赋值或初始化 */
    hcryp->Init.KeyIVConfigSkip = pConf->KeyIVConfigSkip;  /* 赋值或初始化 */
    hcryp->Init.HeaderWidthUnit = pConf->HeaderWidthUnit;  /* 赋值或初始化 */

    /* Set the key size(This bit field is don抰 care in the DES or TDES modes) data type, AlgoMode and operating mode*/
#if defined (CRYP)                                      /* 条件编译 */

    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_DATATYPE | CRYP_CR_KEYSIZE | CRYP_CR_ALGOMODE,  /* 列表续行 */
               hcryp->Init.DataType | hcryp->Init.KeySize | hcryp->Init.Algorithm);  /* 执行语句 */

#else /*AES*/
    MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE | AES_CR_KEYSIZE | AES_CR_CHMOD,  /* 列表续行 */
               hcryp->Init.DataType | hcryp->Init.KeySize | hcryp->Init.Algorithm);  /* 执行语句 */

    /*clear error flags*/
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_ERR_CLEAR);       /* HAL辅助宏/函数 */

#endif  /* End AES or CRYP */

    /* Process Unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Reset Error Code field */
    hcryp->ErrorCode = HAL_CRYP_ERROR_NONE;             /* 赋值或初始化 */

    /* Change the CRYP state */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */

    /* Set the default CRYP phase */
    hcryp->Phase = CRYP_PHASE_READY;                    /* 赋值或初始化 */

    /* Return function status */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process Unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Get CRYP Configuration parameters in associated handle.
  * @param  pConf: pointer to a CRYP_ConfigTypeDef structure
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_GetConfig(CRYP_HandleTypeDef *hcryp, CRYP_ConfigTypeDef *pConf)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  /* Check the CRYP handle allocation */
  if ((hcryp == NULL) || (pConf == NULL))               /* 条件判断 */
  {                                                     /* 代码块开始 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Change the CRYP state */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /* Get  CRYP parameters  */
    pConf->DataType        = hcryp->Init.DataType;      /* 赋值或初始化 */
    pConf->pKey            = hcryp->Init.pKey;          /* 赋值或初始化 */
    pConf->Algorithm       = hcryp->Init.Algorithm;     /* 赋值或初始化 */
    pConf->KeySize         = hcryp->Init.KeySize ;      /* 赋值或初始化 */
    pConf->pInitVect       = hcryp->Init.pInitVect;     /* 赋值或初始化 */
    pConf->Header          = hcryp->Init.Header ;       /* 赋值或初始化 */
    pConf->HeaderSize      = hcryp->Init.HeaderSize;    /* 赋值或初始化 */
    pConf->B0              = hcryp->Init.B0;            /* 赋值或初始化 */
    pConf->DataWidthUnit   = hcryp->Init.DataWidthUnit;  /* 赋值或初始化 */
    pConf->KeyIVConfigSkip = hcryp->Init.KeyIVConfigSkip;  /* 赋值或初始化 */
    pConf->HeaderWidthUnit = hcryp->Init.HeaderWidthUnit;  /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Change the CRYP state */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */

    /* Return function status */
    return HAL_OK;                                      /* 返回值 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process Unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */
/**
  * @brief  Initializes the CRYP MSP.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval None
  */
__weak void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcryp);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CRYP_MspInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  DeInitializes CRYP MSP.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval None
  */
__weak void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcryp);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CRYP_MspDeInit can be implemented in the user file
   */
}                                                       /* 代码块结束 */

#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
/**
  * @brief  Register a User CRYP Callback
  *         To be used instead of the weak predefined callback
  * @param hcryp cryp handle
  * @param CallbackID ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_CRYP_INPUT_COMPLETE_CB_ID Input FIFO transfer completed callback ID
  *          @arg @ref HAL_CRYP_OUTPUT_COMPLETE_CB_ID Output FIFO transfer completed callback ID
  *          @arg @ref HAL_CRYP_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_CRYP_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_CRYP_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param pCallback pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_CRYP_RegisterCallback(CRYP_HandleTypeDef *hcryp, HAL_CRYP_CallbackIDTypeDef CallbackID,  /* HAL状态类型函数 */
                                            pCRYP_CallbackTypeDef pCallback)  /* 源代码 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (pCallback == NULL)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */

    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */
  /* Process locked */
  __HAL_LOCK(hcryp);                                    /* HAL辅助宏/函数 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_CRYP_INPUT_COMPLETE_CB_ID :              /* case标签 */
        hcryp->InCpltCallback = pCallback;              /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CRYP_OUTPUT_COMPLETE_CB_ID :             /* case标签 */
        hcryp->OutCpltCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CRYP_ERROR_CB_ID :                       /* case标签 */
        hcryp->ErrorCallback = pCallback;               /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CRYP_MSPINIT_CB_ID :                     /* case标签 */
        hcryp->MspInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CRYP_MSPDEINIT_CB_ID :                   /* case标签 */
        hcryp->MspDeInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hcryp->State == HAL_CRYP_STATE_RESET)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_CRYP_MSPINIT_CB_ID :                     /* case标签 */
        hcryp->MspInitCallback = pCallback;             /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CRYP_MSPDEINIT_CB_ID :                   /* case标签 */
        hcryp->MspDeInitCallback = pCallback;           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hcryp);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Unregister an CRYP Callback
  *         CRYP callback is redirected to the weak predefined callback
  * @param hcryp cryp handle
  * @param CallbackID ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_CRYP_INPUT_COMPLETE_CB_ID Input FIFO transfer completed callback ID
  *          @arg @ref HAL_CRYP_OUTPUT_COMPLETE_CB_ID Output FIFO transfer completed callback ID
  *          @arg @ref HAL_CRYP_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_CRYP_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_CRYP_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_CRYP_UnRegisterCallback(CRYP_HandleTypeDef *hcryp, HAL_CRYP_CallbackIDTypeDef CallbackID)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  /* Process locked */
  __HAL_LOCK(hcryp);                                    /* HAL辅助宏/函数 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_CRYP_INPUT_COMPLETE_CB_ID :              /* case标签 */
        hcryp->InCpltCallback = HAL_CRYP_InCpltCallback;  /* Legacy weak  InCpltCallback  */
        break;                                          /* 跳出 */

      case HAL_CRYP_OUTPUT_COMPLETE_CB_ID :             /* case标签 */
        hcryp->OutCpltCallback = HAL_CRYP_OutCpltCallback;         /* Legacy weak OutCpltCallback       */
        break;                                          /* 跳出 */

      case HAL_CRYP_ERROR_CB_ID :                       /* case标签 */
        hcryp->ErrorCallback = HAL_CRYP_ErrorCallback;           /* Legacy weak ErrorCallback        */
        break;                                          /* 跳出 */

      case HAL_CRYP_MSPINIT_CB_ID :                     /* case标签 */
        hcryp->MspInitCallback = HAL_CRYP_MspInit;      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CRYP_MSPDEINIT_CB_ID :                   /* case标签 */
        hcryp->MspDeInitCallback = HAL_CRYP_MspDeInit;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (hcryp->State == HAL_CRYP_STATE_RESET)        /* 否则如果 */
  {                                                     /* 代码块开始 */
    switch (CallbackID)                                 /* switch分支 */
    {                                                   /* 代码块开始 */
      case HAL_CRYP_MSPINIT_CB_ID :                     /* case标签 */
        hcryp->MspInitCallback = HAL_CRYP_MspInit;      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case HAL_CRYP_MSPDEINIT_CB_ID :                   /* case标签 */
        hcryp->MspDeInitCallback = HAL_CRYP_MspDeInit;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default :                                         /* default分支 */
        /* Update the error code */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
        /* Return error status */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Update the error code */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_INVALID_CALLBACK;  /* 赋值或初始化 */
    /* Return error status */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Release Lock */
  __HAL_UNLOCK(hcryp);                                  /* HAL辅助宏/函数 */

  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup CRYP_Exported_Functions_Group2  Encrypt Decrypt functions
  *  @brief   processing functions.
  *
@verbatim
  ==============================================================================
                      ##### Encrypt Decrypt  functions #####
  ==============================================================================
    [..]  This section provides API allowing to Encrypt/Decrypt Data following
          Standard DES/TDES or AES, and Algorithm configured by the user:
      (+) Standard DES/TDES only supported by CRYP1 IP, below list of Algorithm supported :
           - Electronic Code Book(ECB)
           - Cipher Block Chaining (CBC)
      (+) Standard AES  supported by CRYP1 IP & TinyAES, list of Algorithm supported:
           - Electronic Code Book(ECB)
           - Cipher Block Chaining (CBC)
           - Counter mode (CTR)
           - Cipher Block Chaining (CBC)
           - Counter mode (CTR)
           - Galois/counter mode (GCM)
           - Counter with Cipher Block Chaining-Message(CCM)
    [..]  Three processing functions are available:
      (+) Polling mode : HAL_CRYP_Encrypt & HAL_CRYP_Decrypt
      (+) Interrupt mode : HAL_CRYP_Encrypt_IT & HAL_CRYP_Decrypt_IT
      (+) DMA mode : HAL_CRYP_Encrypt_DMA & HAL_CRYP_Decrypt_DMA

@endverbatim
  * @{
  */


/**
  * @brief  Encryption mode.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Input: Pointer to the input buffer (plaintext)
  * @param  Size: Length of the plaintext buffer in word.
  * @param  Output: Pointer to the output buffer(ciphertext)
  * @param  Timeout: Specify Timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_Encrypt(CRYP_HandleTypeDef *hcryp, uint32_t *Input, uint16_t Size, uint32_t *Output,  /* HAL状态类型函数 */
                                   uint32_t Timeout)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t algo;                                        /* 32位无符号变量/函数 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Change state Busy */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /*  Reset CrypInCount, CrypOutCount and Initialize pCrypInBuffPtr and pCrypOutBuffPtr parameters*/
    hcryp->CrypInCount = 0U;                            /* 赋值或初始化 */
    hcryp->CrypOutCount = 0U;                           /* 赋值或初始化 */
    hcryp->pCrypInBuffPtr = Input;                      /* 赋值或初始化 */
    hcryp->pCrypOutBuffPtr = Output;                    /* 赋值或初始化 */

    /*  Calculate Size parameter in Byte*/
    if (hcryp->Init.DataWidthUnit == CRYP_DATAWIDTHUNIT_WORD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size * 4U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

#if defined (CRYP)                                      /* 条件编译 */
    /* Set Encryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGODIR, CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & CRYP_CR_ALGOMODE;      /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */
      case CRYP_DES_ECB:                                /* case标签 */
      case CRYP_DES_CBC:                                /* case标签 */
      case CRYP_TDES_ECB:                               /* case标签 */
      case CRYP_TDES_CBC:                               /* case标签 */

        /*Set Key */
        hcryp->Instance->K1LR = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
        hcryp->Instance->K1RR = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
        if ((hcryp->Init.Algorithm == CRYP_TDES_ECB) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->K2LR = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
          hcryp->Instance->K2RR = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */
          hcryp->Instance->K3LR = *(uint32_t *)(hcryp->Init.pKey + 4);  /* 赋值或初始化 */
          hcryp->Instance->K3RR = *(uint32_t *)(hcryp->Init.pKey + 5);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /*Set Initialization Vector (IV)*/
        if ((hcryp->Init.Algorithm == CRYP_DES_CBC) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
          hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Flush FIFO */
        HAL_CRYP_FIFO_FLUSH(hcryp);                     /* HAL库函数调用 */

        /* Set the phase */
        hcryp->Phase = CRYP_PHASE_PROCESS;              /* 赋值或初始化 */

        /* Statrt DES/TDES encryption process */
        status = CRYP_TDES_Process(hcryp, Timeout);     /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        /* AES encryption */
        status = CRYP_AES_Encrypt(hcryp, Timeout);      /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #if defined (CRYP_CR_ALGOMODE_AES_GCM)          /* 条件编译 */
      case CRYP_AES_GCM:                                /* case标签 */

        /* AES GCM encryption */
        status = CRYP_AESGCM_Process(hcryp, Timeout);   /* 赋值或初始化 */

        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */

        /* AES CCM encryption */
        status = CRYP_AESCCM_Process(hcryp, Timeout);   /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #endif /* GCM CCM defined*/
      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
    }                                                   /* 代码块结束 */

#else /*AES*/

    /* Set the operating mode*/
    MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & AES_CR_CHMOD;          /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        /* AES encryption */
        status = CRYP_AES_Encrypt(hcryp, Timeout);      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_GCM_GMAC:                           /* case标签 */

        /* AES GCM encryption */
        status = CRYP_AESGCM_Process(hcryp, Timeout) ;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */

        /* AES CCM encryption */
        status = CRYP_AESCCM_Process(hcryp, Timeout);   /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
    }                                                   /* 代码块结束 */
#endif /*end AES or CRYP */

    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Change the CRYP peripheral state */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Decryption mode.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Input: Pointer to the input buffer (ciphertext )
  * @param  Size: Length of the plaintext buffer in word.
  * @param  Output: Pointer to the output buffer(plaintext)
  * @param  Timeout: Specify Timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_Decrypt(CRYP_HandleTypeDef *hcryp, uint32_t *Input, uint16_t Size, uint32_t *Output,  /* HAL状态类型函数 */
                                   uint32_t Timeout)    /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  HAL_StatusTypeDef status;                             /* HAL状态类型函数 */
  uint32_t algo;                                        /* 32位无符号变量/函数 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Change state Busy */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /*  Reset CrypInCount, CrypOutCount and Initialize pCrypInBuffPtr and pCrypOutBuffPtr  parameters*/
    hcryp->CrypInCount = 0U;                            /* 赋值或初始化 */
    hcryp->CrypOutCount = 0U;                           /* 赋值或初始化 */
    hcryp->pCrypInBuffPtr = Input;                      /* 赋值或初始化 */
    hcryp->pCrypOutBuffPtr = Output;                    /* 赋值或初始化 */

    /*  Calculate Size parameter in Byte*/
    if (hcryp->Init.DataWidthUnit == CRYP_DATAWIDTHUNIT_WORD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size * 4U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

#if defined (CRYP)                                      /* 条件编译 */

    /* Set Decryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGODIR, CRYP_OPERATINGMODE_DECRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & CRYP_CR_ALGOMODE;      /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */
      case CRYP_DES_ECB:                                /* case标签 */
      case CRYP_DES_CBC:                                /* case标签 */
      case CRYP_TDES_ECB:                               /* case标签 */
      case CRYP_TDES_CBC:                               /* case标签 */

        /*Set Key */
        hcryp->Instance->K1LR = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
        hcryp->Instance->K1RR = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
        if ((hcryp->Init.Algorithm == CRYP_TDES_ECB) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->K2LR = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
          hcryp->Instance->K2RR = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */
          hcryp->Instance->K3LR = *(uint32_t *)(hcryp->Init.pKey + 4);  /* 赋值或初始化 */
          hcryp->Instance->K3RR = *(uint32_t *)(hcryp->Init.pKey + 5);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /*Set Initialization Vector (IV)*/
        if ((hcryp->Init.Algorithm == CRYP_DES_CBC) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
          hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Flush FIFO */
        HAL_CRYP_FIFO_FLUSH(hcryp);                     /* HAL库函数调用 */

        /* Set the phase */
        hcryp->Phase = CRYP_PHASE_PROCESS;              /* 赋值或初始化 */

        /* Start DES/TDES decryption process */
        status = CRYP_TDES_Process(hcryp, Timeout);     /* 赋值或初始化 */

        break;                                          /* 跳出 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        /* AES decryption */
        status = CRYP_AES_Decrypt(hcryp, Timeout);      /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #if defined (CRYP_CR_ALGOMODE_AES_GCM)          /* 条件编译 */
      case CRYP_AES_GCM:                                /* case标签 */

        /* AES GCM decryption */
        status = CRYP_AESGCM_Process(hcryp, Timeout) ;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */

        /* AES CCM decryption */
        status = CRYP_AESCCM_Process(hcryp, Timeout);   /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #endif /* GCM CCM defined*/
      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
    }                                                   /* 代码块结束 */

#else /*AES*/

    /* Set Decryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_DECRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & AES_CR_CHMOD;          /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        /* AES decryption */
        status = CRYP_AES_Decrypt(hcryp, Timeout);      /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_GCM_GMAC:                           /* case标签 */

        /* AES GCM decryption */
        status = CRYP_AESGCM_Process(hcryp, Timeout) ;  /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */

        /* AES CCM decryption */
        status = CRYP_AESCCM_Process(hcryp, Timeout);   /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
    }                                                   /* 代码块结束 */
#endif /* End AES or CRYP */

    if (status == HAL_OK)                               /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Change the CRYP peripheral state */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
    return HAL_ERROR;                                   /* 返回值 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Encryption in interrupt mode.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Input: Pointer to the input buffer (plaintext)
  * @param  Size: Length of the plaintext buffer in word
  * @param  Output: Pointer to the output buffer(ciphertext)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_Encrypt_IT(CRYP_HandleTypeDef *hcryp, uint32_t *Input, uint16_t Size, uint32_t *Output)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t algo;                                        /* 32位无符号变量/函数 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Change state Busy */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /*  Reset CrypInCount, CrypOutCount and Initialize pCrypInBuffPtr and pCrypOutBuffPtr parameters*/
    hcryp->CrypInCount = 0U;                            /* 赋值或初始化 */
    hcryp->CrypOutCount = 0U;                           /* 赋值或初始化 */
    hcryp->pCrypInBuffPtr = Input;                      /* 赋值或初始化 */
    hcryp->pCrypOutBuffPtr = Output;                    /* 赋值或初始化 */

    /*  Calculate Size parameter in Byte*/
    if (hcryp->Init.DataWidthUnit == CRYP_DATAWIDTHUNIT_WORD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size * 4U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

#if defined (CRYP)                                      /* 条件编译 */

    /* Set encryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGODIR, CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = (hcryp->Instance->CR & CRYP_CR_ALGOMODE);    /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */
      case CRYP_DES_ECB:                                /* case标签 */
      case CRYP_DES_CBC:                                /* case标签 */
      case CRYP_TDES_ECB:                               /* case标签 */
      case CRYP_TDES_CBC:                               /* case标签 */

        /*Set Key */
        hcryp->Instance->K1LR = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
        hcryp->Instance->K1RR = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
        if ((hcryp->Init.Algorithm == CRYP_TDES_ECB) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->K2LR = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
          hcryp->Instance->K2RR = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */
          hcryp->Instance->K3LR = *(uint32_t *)(hcryp->Init.pKey + 4);  /* 赋值或初始化 */
          hcryp->Instance->K3RR = *(uint32_t *)(hcryp->Init.pKey + 5);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        /* Set the Initialization Vector*/
        if ((hcryp->Init.Algorithm == CRYP_DES_CBC) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
          hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Flush FIFO */
        HAL_CRYP_FIFO_FLUSH(hcryp);                     /* HAL库函数调用 */

        /* Set the phase */
        hcryp->Phase = CRYP_PHASE_PROCESS;              /* 赋值或初始化 */

        /* Enable interrupts */
        __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_INI | CRYP_IT_OUTI);  /* HAL辅助宏/函数 */

        /* Enable CRYP to start DES/TDES process*/
        __HAL_CRYP_ENABLE(hcryp);                       /* HAL辅助宏/函数 */
        break;                                          /* 跳出 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        status = CRYP_AES_Encrypt_IT(hcryp);            /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #if defined (CRYP_CR_ALGOMODE_AES_GCM)          /* 条件编译 */
      case CRYP_AES_GCM:                                /* case标签 */

        status = CRYP_AESGCM_Process_IT(hcryp) ;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */

        status = CRYP_AESCCM_Process_IT(hcryp);         /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #endif /* GCM CCM defined*/
      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */

#else /* AES */

    /* Set encryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & AES_CR_CHMOD;          /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */
      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        /* AES encryption */
        status = CRYP_AES_Encrypt_IT(hcryp);            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_GCM_GMAC:                           /* case标签 */

        /* AES GCM encryption */
        status = CRYP_AESGCM_Process_IT(hcryp) ;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */

        /* AES CCM encryption */
        status = CRYP_AESCCM_Process_IT(hcryp);         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
#endif /*end AES or CRYP*/

  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Decryption in itnterrupt mode.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Input: Pointer to the input buffer (ciphertext )
  * @param  Size: Length of the plaintext buffer in word.
  * @param  Output: Pointer to the output buffer(plaintext)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_Decrypt_IT(CRYP_HandleTypeDef *hcryp, uint32_t *Input, uint16_t Size, uint32_t *Output)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t algo;                                        /* 32位无符号变量/函数 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Change state Busy */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /*  Reset CrypInCount, CrypOutCount and Initialize pCrypInBuffPtr and pCrypOutBuffPtr parameters*/
    hcryp->CrypInCount = 0U;                            /* 赋值或初始化 */
    hcryp->CrypOutCount = 0U;                           /* 赋值或初始化 */
    hcryp->pCrypInBuffPtr = Input;                      /* 赋值或初始化 */
    hcryp->pCrypOutBuffPtr = Output;                    /* 赋值或初始化 */

    /*  Calculate Size parameter in Byte*/
    if (hcryp->Init.DataWidthUnit == CRYP_DATAWIDTHUNIT_WORD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size * 4U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

#if defined (CRYP)                                      /* 条件编译 */

    /* Set decryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGODIR, CRYP_OPERATINGMODE_DECRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & CRYP_CR_ALGOMODE;      /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */
      case CRYP_DES_ECB:                                /* case标签 */
      case CRYP_DES_CBC:                                /* case标签 */
      case CRYP_TDES_ECB:                               /* case标签 */
      case CRYP_TDES_CBC:                               /* case标签 */

        /*Set Key */
        hcryp->Instance->K1LR = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
        hcryp->Instance->K1RR = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
        if ((hcryp->Init.Algorithm == CRYP_TDES_ECB) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->K2LR = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
          hcryp->Instance->K2RR = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */
          hcryp->Instance->K3LR = *(uint32_t *)(hcryp->Init.pKey + 4);  /* 赋值或初始化 */
          hcryp->Instance->K3RR = *(uint32_t *)(hcryp->Init.pKey + 5);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Set the Initialization Vector*/
        if ((hcryp->Init.Algorithm == CRYP_DES_CBC) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
          hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        /* Flush FIFO */
        HAL_CRYP_FIFO_FLUSH(hcryp);                     /* HAL库函数调用 */

        /* Set the phase */
        hcryp->Phase = CRYP_PHASE_PROCESS;              /* 赋值或初始化 */

        /* Enable interrupts */
        __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_INI | CRYP_IT_OUTI);  /* HAL辅助宏/函数 */

        /* Enable CRYP and start DES/TDES process*/
        __HAL_CRYP_ENABLE(hcryp);                       /* HAL辅助宏/函数 */

        break;                                          /* 跳出 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        /* AES decryption */
        status = CRYP_AES_Decrypt_IT(hcryp);            /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #if defined (CRYP_CR_ALGOMODE_AES_GCM)          /* 条件编译 */
      case CRYP_AES_GCM:                                /* case标签 */

        /* AES GCM decryption */
        status = CRYP_AESGCM_Process_IT(hcryp) ;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */

        /* AES CCMdecryption */
        status = CRYP_AESCCM_Process_IT(hcryp);         /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #endif /* GCM CCM defined*/
      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */

#else /*AES*/

    /* Set decryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_DECRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & AES_CR_CHMOD;          /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */
      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        /* AES decryption */
        status = CRYP_AES_Decrypt_IT(hcryp);            /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_GCM_GMAC:                           /* case标签 */

        /* AES GCM decryption */
        status = CRYP_AESGCM_Process_IT(hcryp) ;        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */

        /* AES CCM decryption */
        status = CRYP_AESCCM_Process_IT(hcryp);         /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
#endif /* End AES or CRYP */

  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Encryption in DMA mode.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Input: Pointer to the input buffer (plaintext)
  * @param  Size: Length of the plaintext buffer in word.
  * @param  Output: Pointer to the output buffer(ciphertext)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_Encrypt_DMA(CRYP_HandleTypeDef *hcryp, uint32_t *Input, uint16_t Size, uint32_t *Output)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t algo;                                        /* 32位无符号变量/函数 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Change state Busy */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /*  Reset CrypInCount, CrypOutCount and Initialize pCrypInBuffPtr and pCrypOutBuffPtr parameters*/
    hcryp->CrypInCount = 0U;                            /* 赋值或初始化 */
    hcryp->CrypOutCount = 0U;                           /* 赋值或初始化 */
    hcryp->pCrypInBuffPtr = Input;                      /* 赋值或初始化 */
    hcryp->pCrypOutBuffPtr = Output;                    /* 赋值或初始化 */

    /*  Calculate Size parameter in Byte*/
    if (hcryp->Init.DataWidthUnit == CRYP_DATAWIDTHUNIT_WORD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size * 4U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

#if defined (CRYP)                                      /* 条件编译 */

    /* Set encryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGODIR, CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & CRYP_CR_ALGOMODE;      /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */
      case CRYP_DES_ECB:                                /* case标签 */
      case CRYP_DES_CBC:                                /* case标签 */
      case CRYP_TDES_ECB:                               /* case标签 */
      case CRYP_TDES_CBC:                               /* case标签 */

        /*Set Key */
        hcryp->Instance->K1LR = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
        hcryp->Instance->K1RR = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
        if ((hcryp->Init.Algorithm == CRYP_TDES_ECB) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->K2LR = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
          hcryp->Instance->K2RR = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */
          hcryp->Instance->K3LR = *(uint32_t *)(hcryp->Init.pKey + 4);  /* 赋值或初始化 */
          hcryp->Instance->K3RR = *(uint32_t *)(hcryp->Init.pKey + 5);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Set the Initialization Vector*/
        if ((hcryp->Init.Algorithm == CRYP_DES_CBC) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
          hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Flush FIFO */
        HAL_CRYP_FIFO_FLUSH(hcryp);                     /* HAL库函数调用 */

        /* Set the phase */
        hcryp->Phase = CRYP_PHASE_PROCESS;              /* 赋值或初始化 */

        /* Start DMA process transfer for DES/TDES */
        CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), ((uint16_t)(hcryp->Size) / 4U),  /* 列表续行 */
                          (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
        break;                                          /* 跳出 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          if (hcryp->KeyIVConfig == 1U)                 /* 条件判断 */
          {                                             /* 代码块开始 */
            /* If the Key and IV configuration has to be done only once
            and if it has already been done, skip it */
            DoKeyIVConfig = 0U;                         /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* If the Key and IV configuration has to be done only once
            and if it has not been done already, do it and set KeyIVConfig
            to keep track it won't have to be done again next time */
            hcryp->KeyIVConfig = 1U;                    /* 赋值或初始化 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */

        if (DoKeyIVConfig == 1U)                        /* 条件判断 */
        {                                               /* 代码块开始 */
          /*  Set the Key*/
          CRYP_SetKey(hcryp, hcryp->Init.KeySize);      /* 执行语句 */

          /* Set the Initialization Vector*/
          if (hcryp->Init.Algorithm != CRYP_AES_ECB)    /* 条件判断 */
          {                                             /* 代码块开始 */
            hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
            hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1U);  /* 赋值或初始化 */
            hcryp->Instance->IV1LR = *(uint32_t *)(hcryp->Init.pInitVect + 2U);  /* 赋值或初始化 */
            hcryp->Instance->IV1RR = *(uint32_t *)(hcryp->Init.pInitVect + 3U);  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
        } /* if (DoKeyIVConfig == 1U) */

        /* Set the phase */
        hcryp->Phase = CRYP_PHASE_PROCESS;              /* 赋值或初始化 */

        /* Start DMA process transfer for AES */
        CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), ((uint16_t)(hcryp->Size) / 4U),  /* 列表续行 */
                          (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
        break;                                          /* 跳出 */
        #if defined (CRYP_CR_ALGOMODE_AES_GCM)          /* 条件编译 */
      case CRYP_AES_GCM:                                /* case标签 */
        /* AES GCM encryption */
        status = CRYP_AESGCM_Process_DMA(hcryp) ;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */
        /* AES CCM encryption */
        status = CRYP_AESCCM_Process_DMA(hcryp);        /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #endif /* GCM CCM defined*/
      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */

#else /*AES*/
    /* Set encryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & AES_CR_CHMOD;          /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
        {                                               /* 代码块开始 */
          if (hcryp->KeyIVConfig == 1U)                 /* 条件判断 */
          {                                             /* 代码块开始 */
            /* If the Key and IV configuration has to be done only once
            and if it has already been done, skip it */
            DoKeyIVConfig = 0U;                         /* 赋值或初始化 */
          }                                             /* 代码块结束 */
          else                                          /* 否则 */
          {                                             /* 代码块开始 */
            /* If the Key and IV configuration has to be done only once
            and if it has not been done already, do it and set KeyIVConfig
            to keep track it won't have to be done again next time */
            hcryp->KeyIVConfig = 1U;                    /* 赋值或初始化 */
          }                                             /* 代码块结束 */
        }                                               /* 代码块结束 */

        if (DoKeyIVConfig == 1U)                        /* 条件判断 */
        {                                               /* 代码块开始 */
          /*  Set the Key*/
          CRYP_SetKey(hcryp, hcryp->Init.KeySize);      /* 执行语句 */

          /* Set the Initialization Vector*/
          if (hcryp->Init.Algorithm != CRYP_AES_ECB)    /* 条件判断 */
          {                                             /* 代码块开始 */
            hcryp->Instance->IVR3 = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
            hcryp->Instance->IVR2 = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
            hcryp->Instance->IVR1 = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
            hcryp->Instance->IVR0 = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
          }                                             /* 代码块结束 */
        } /* if (DoKeyIVConfig == 1U) */
        /* Set the phase */
        hcryp->Phase = CRYP_PHASE_PROCESS;              /* 赋值或初始化 */

        /* Start DMA process transfer for AES */
        CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), (hcryp->Size / 4U), (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
        break;                                          /* 跳出 */

      case CRYP_AES_GCM_GMAC:                           /* case标签 */
        /* AES GCM encryption */
        status = CRYP_AESGCM_Process_DMA(hcryp) ;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */
        /* AES CCM encryption */
        status = CRYP_AESCCM_Process_DMA(hcryp);        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
#endif /* End AES or CRYP */

  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Decryption in DMA mode.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Input: Pointer to the input buffer (ciphertext )
  * @param  Size: Length of the plaintext buffer in word
  * @param  Output: Pointer to the output buffer(plaintext)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_CRYP_Decrypt_DMA(CRYP_HandleTypeDef *hcryp, uint32_t *Input, uint16_t Size, uint32_t *Output)  /* HAL状态类型函数 */
{                                                       /* 代码块开始 */
  uint32_t algo;                                        /* 32位无符号变量/函数 */
  HAL_StatusTypeDef status = HAL_OK;                    /* HAL状态类型函数 */

  if (hcryp->State == HAL_CRYP_STATE_READY)             /* 条件判断 */
  {                                                     /* 代码块开始 */

    /* Change state Busy */
    hcryp->State = HAL_CRYP_STATE_BUSY;                 /* 赋值或初始化 */

    /* Process locked */
    __HAL_LOCK(hcryp);                                  /* HAL辅助宏/函数 */

    /*  Reset CrypInCount, CrypOutCount and Initialize pCrypInBuffPtr, pCrypOutBuffPtr and Size parameters*/
    hcryp->CrypInCount = 0U;                            /* 赋值或初始化 */
    hcryp->CrypOutCount = 0U;                           /* 赋值或初始化 */
    hcryp->pCrypInBuffPtr = Input;                      /* 赋值或初始化 */
    hcryp->pCrypOutBuffPtr = Output;                    /* 赋值或初始化 */

    /*  Calculate Size parameter in Byte*/
    if (hcryp->Init.DataWidthUnit == CRYP_DATAWIDTHUNIT_WORD)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size * 4U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Size = Size;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

#if defined (CRYP)                                      /* 条件编译 */

    /* Set decryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGODIR, CRYP_OPERATINGMODE_DECRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & CRYP_CR_ALGOMODE;      /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */
      case CRYP_DES_ECB:                                /* case标签 */
      case CRYP_DES_CBC:                                /* case标签 */
      case CRYP_TDES_ECB:                               /* case标签 */
      case CRYP_TDES_CBC:                               /* case标签 */

        /*Set Key */
        hcryp->Instance->K1LR = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
        hcryp->Instance->K1RR = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
        if ((hcryp->Init.Algorithm == CRYP_TDES_ECB) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->K2LR = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
          hcryp->Instance->K2RR = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */
          hcryp->Instance->K3LR = *(uint32_t *)(hcryp->Init.pKey + 4);  /* 赋值或初始化 */
          hcryp->Instance->K3RR = *(uint32_t *)(hcryp->Init.pKey + 5);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Set the Initialization Vector*/
        if ((hcryp->Init.Algorithm == CRYP_DES_CBC) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 条件判断 */
        {                                               /* 代码块开始 */
          hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
          hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /* Flush FIFO */
        HAL_CRYP_FIFO_FLUSH(hcryp);                     /* HAL库函数调用 */

        /* Set the phase */
        hcryp->Phase = CRYP_PHASE_PROCESS;              /* 赋值或初始化 */

        /* Start DMA process transfer for DES/TDES */
        CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), ((uint16_t)(hcryp->Size) / 4U),  /* 列表续行 */
                          (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
        break;                                          /* 跳出 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        /* AES decryption */
        status = CRYP_AES_Decrypt_DMA(hcryp);           /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #if defined (CRYP_CR_ALGOMODE_AES_GCM)          /* 条件编译 */
      case CRYP_AES_GCM:                                /* case标签 */
        /* AES GCM decryption */
        status = CRYP_AESGCM_Process_DMA(hcryp) ;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */
        /* AES CCM decryption */
        status = CRYP_AESCCM_Process_DMA(hcryp);        /* 赋值或初始化 */
        break;                                          /* 跳出 */
        #endif /* GCM CCM defined*/
      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */

#else /*AES*/

    /* Set decryption operating mode*/
    MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_DECRYPT);  /* 执行语句 */

    /* algo get algorithm selected */
    algo = hcryp->Instance->CR & AES_CR_CHMOD;          /* 赋值或初始化 */

    switch (algo)                                       /* switch分支 */
    {                                                   /* 代码块开始 */

      case CRYP_AES_ECB:                                /* case标签 */
      case CRYP_AES_CBC:                                /* case标签 */
      case CRYP_AES_CTR:                                /* case标签 */

        /* AES decryption */
        status = CRYP_AES_Decrypt_DMA(hcryp);           /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_GCM_GMAC:                           /* case标签 */
        /* AES GCM decryption */
        status = CRYP_AESGCM_Process_DMA(hcryp) ;       /* 赋值或初始化 */
        break;                                          /* 跳出 */

      case CRYP_AES_CCM:                                /* case标签 */
        /* AES CCM decryption */
        status = CRYP_AESCCM_Process_DMA(hcryp);        /* 赋值或初始化 */
        break;                                          /* 跳出 */

      default:                                          /* default分支 */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_NOT_SUPPORTED;  /* 赋值或初始化 */
        /* Change the CRYP peripheral state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */
        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        status =  HAL_ERROR;                            /* 赋值或初始化 */
        break;                                          /* 跳出 */
    }                                                   /* 代码块结束 */
#endif /* End AES or CRYP */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
    status =  HAL_ERROR;                                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return status;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @}
  */

/** @defgroup CRYP_Exported_Functions_Group3 CRYP IRQ handler management
  *  @brief   CRYP IRQ handler.
  *
@verbatim
  ==============================================================================
                ##### CRYP IRQ handler management #####
  ==============================================================================
[..]  This section provides CRYP IRQ handler and callback functions.
      (+) HAL_CRYP_IRQHandler CRYP interrupt request
      (+) HAL_CRYP_InCpltCallback input data transfer complete callback
      (+) HAL_CRYP_OutCpltCallback output data transfer complete callback
      (+) HAL_CRYP_ErrorCallback  CRYP error callback
      (+) HAL_CRYP_GetState return the CRYP state
      (+) HAL_CRYP_GetError return the CRYP error code
@endverbatim
  * @{
  */

/**
  * @brief  This function handles cryptographic interrupt request.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval None
  */
void HAL_CRYP_IRQHandler(CRYP_HandleTypeDef *hcryp)     /* 中断服务函数 */
{                                                       /* 代码块开始 */

#if defined (CRYP)                                      /* 条件编译 */

  uint32_t itstatus = hcryp->Instance->MISR;            /* 32位无符号变量/函数 */

  if ((itstatus & (CRYP_IT_INI | CRYP_IT_OUTI)) != 0U)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hcryp->Init.Algorithm == CRYP_DES_ECB) || (hcryp->Init.Algorithm == CRYP_DES_CBC)  /* 条件判断 */
        || (hcryp->Init.Algorithm == CRYP_TDES_ECB) || (hcryp->Init.Algorithm == CRYP_TDES_CBC))  /* 源代码 */
    {                                                   /* 代码块开始 */
      CRYP_TDES_IT(hcryp); /* DES or TDES*/
    }                                                   /* 代码块结束 */
    else if ((hcryp->Init.Algorithm == CRYP_AES_ECB) || (hcryp->Init.Algorithm == CRYP_AES_CBC)  /* 否则如果 */
             || (hcryp->Init.Algorithm == CRYP_AES_CTR))  /* 源代码 */
    {                                                   /* 代码块开始 */
      CRYP_AES_IT(hcryp); /*AES*/
    }                                                   /* 代码块结束 */
    #if defined (CRYP_CR_ALGOMODE_AES_GCM)              /* 条件编译 */
    else if ((hcryp->Init.Algorithm == CRYP_AES_GCM) || (hcryp->Init.Algorithm == CRYP_CR_ALGOMODE_AES_CCM))  /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* if header phase */
      if ((hcryp->Instance->CR & CRYP_PHASE_HEADER) == CRYP_PHASE_HEADER)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        CRYP_GCMCCM_SetHeaderPhase_IT(hcryp);           /* 执行语句 */
      }                                                 /* 代码块结束 */
      else  /* if payload phase */
      {                                                 /* 代码块开始 */
        CRYP_GCMCCM_SetPayloadPhase_IT(hcryp);          /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    #endif /* GCM CCM defined*/
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

#else /*AES*/
  if (__HAL_CRYP_GET_FLAG(hcryp, CRYP_IT_CCF) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_CRYP_GET_IT_SOURCE(hcryp, CRYP_IT_CCFIE) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */

      /* Clear computation complete flag */
      __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);     /* HAL辅助宏/函数 */

      if (hcryp->Init.Algorithm == CRYP_AES_GCM_GMAC)   /* 条件判断 */
      {                                                 /* 代码块开始 */

        /* if header phase */
        if ((hcryp->Instance->CR & CRYP_PHASE_HEADER) == CRYP_PHASE_HEADER)  /* 条件判断 */
        {                                               /* 代码块开始 */
          CRYP_GCMCCM_SetHeaderPhase_IT(hcryp);         /* 执行语句 */
        }                                               /* 代码块结束 */
        else  /* if payload phase */
        {                                               /* 代码块开始 */
          CRYP_GCMCCM_SetPayloadPhase_IT(hcryp);        /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if (hcryp->Init.Algorithm == CRYP_AES_CCM)   /* 否则如果 */
      {                                                 /* 代码块开始 */
        /* if header phase */
        if (hcryp->Init.HeaderSize >=  hcryp->CrypHeaderCount)  /* 条件判断 */
        {                                               /* 代码块开始 */
          CRYP_GCMCCM_SetHeaderPhase_IT(hcryp);         /* 执行语句 */
        }                                               /* 代码块结束 */
        else   /* if payload phase */
        {                                               /* 代码块开始 */
          CRYP_GCMCCM_SetPayloadPhase_IT(hcryp);        /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else  /* AES Algorithm ECB,CBC or CTR*/
      {                                                 /* 代码块开始 */
        CRYP_AES_IT(hcryp);                             /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  /* Check if error occurred */
  if (__HAL_CRYP_GET_IT_SOURCE(hcryp, CRYP_IT_ERRIE) != RESET)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* If write Error occurred */
    if (__HAL_CRYP_GET_FLAG(hcryp, CRYP_IT_WRERR) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_WRITE;         /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* If read Error occurred */
    if (__HAL_CRYP_GET_FLAG(hcryp, CRYP_IT_RDERR) != RESET)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_READ;          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* End AES or CRYP */
}                                                       /* 代码块结束 */

/**
  * @brief  Return the CRYP error code.
  * @param  hcryp : pointer to a CRYP_HandleTypeDef structure that contains
  *                 the configuration information for the  CRYP IP
  * @retval CRYP error code
  */
uint32_t HAL_CRYP_GetError(CRYP_HandleTypeDef *hcryp)   /* 32位无符号变量/函数 */
{                                                       /* 代码块开始 */
  return hcryp->ErrorCode;                              /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Returns the CRYP state.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @retval HAL state
  */
HAL_CRYP_STATETypeDef HAL_CRYP_GetState(CRYP_HandleTypeDef *hcryp)  /* HAL库函数调用 */
{                                                       /* 代码块开始 */
  return hcryp->State;                                  /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Input FIFO transfer completed callback.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @retval None
  */
__weak void HAL_CRYP_InCpltCallback(CRYP_HandleTypeDef *hcryp)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcryp);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CRYP_InCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  Output FIFO transfer completed callback.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @retval None
  */
__weak void HAL_CRYP_OutCpltCallback(CRYP_HandleTypeDef *hcryp)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcryp);                                        /* 执行语句 */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_CRYP_OutCpltCallback can be implemented in the user file
   */
}                                                       /* 代码块结束 */

/**
  * @brief  CRYP error callback.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @retval None
  */
__weak void HAL_CRYP_ErrorCallback(CRYP_HandleTypeDef *hcryp)  /* 源代码 */
{                                                       /* 代码块开始 */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hcryp);                                        /* 执行语句 */

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_CRYP_ErrorCallback could be implemented in the user file
   */
}                                                       /* 代码块结束 */
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup CRYP_Private_Functions
  * @{
  */

#if defined (CRYP)                                      /* 条件编译 */

/**
  * @brief  Encryption in ECB/CBC Algorithm with DES/TDES standard.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Timeout: specify Timeout value
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_TDES_Process(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t temp[2];  /* Temporary CrypOutBuff */
  uint16_t incount; /* Temporary CrypInCount Value */
  uint16_t outcount;  /* Temporary CrypOutCount Value */
  uint32_t i;                                           /* 32位无符号变量/函数 */

  /* Enable CRYP */
  __HAL_CRYP_ENABLE(hcryp);                             /* HAL辅助宏/函数 */
  /*Temporary CrypOutCount Value*/
  outcount = hcryp->CrypOutCount;                       /* 赋值或初始化 */

  /*Start processing*/
  while ((hcryp->CrypInCount < (hcryp->Size / 4U)) && (outcount < (hcryp->Size / 4U)))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Temporary CrypInCount Value */
    incount = hcryp->CrypInCount;                       /* 赋值或初始化 */
    /* Write plain data and get cipher data */
    if (((hcryp->Instance->SR & CRYP_FLAG_IFNF) != 0x0U) && (incount < (hcryp->Size / 4U)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Write the input block in the IN FIFO */
      hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Wait for OFNE flag to be raised */
    if (CRYP_WaitOnOFNEFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the CRYP peripheral clock */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      /* Change state & errorCode*/
      hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;       /* 赋值或初始化 */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /*Call registered error callback*/
      hcryp->ErrorCallback(hcryp);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call legacy weak error callback*/
      HAL_CRYP_ErrorCallback(hcryp);                    /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */

    /*Temporary CrypOutCount Value*/
    outcount = hcryp->CrypOutCount;                     /* 赋值或初始化 */

    if (((hcryp->Instance->SR & CRYP_FLAG_OFNE) != 0x0U) && (outcount < (hcryp->Size / 4U)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read the output block from the Output FIFO and put them in temporary Buffer then get CrypOutBuff from temporary buffer  */
      for (i = 0U; i < 2U; i++)                         /* for循环 */
      {                                                 /* 代码块开始 */
        temp[i] = hcryp->Instance->DOUT;                /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      i = 0U;                                           /* 赋值或初始化 */
      while (((hcryp->CrypOutCount < ((hcryp->Size) / 4U))) && (i < 2U))  /* while循环 */
      {                                                 /* 代码块开始 */
        *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
        hcryp->CrypOutCount++;                          /* 执行语句 */
        i++;                                            /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /*Temporary CrypOutCount Value*/
    outcount = hcryp->CrypOutCount;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Disable CRYP */
  __HAL_CRYP_DISABLE(hcryp);                            /* HAL辅助宏/函数 */
  /* Change the CRYP state */
  hcryp->State = HAL_CRYP_STATE_READY;                  /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  CRYP block input/output data handling under interruption with DES/TDES standard.
  * @note   The function is called under interruption only, once
  *         interruptions have been enabled by CRYP_Decrypt_IT() and CRYP_Encrypt_IT().
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @retval none
  */
static void CRYP_TDES_IT(CRYP_HandleTypeDef *hcryp)     /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t temp[2];  /* Temporary CrypOutBuff */
  uint32_t i;                                           /* 32位无符号变量/函数 */

  if (hcryp->State == HAL_CRYP_STATE_BUSY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (__HAL_CRYP_GET_IT(hcryp, CRYP_IT_INI) != 0x0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (__HAL_CRYP_GET_FLAG(hcryp, CRYP_FLAG_INRIS) != 0x0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Write input block in the IN FIFO */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
        hcryp->CrypInCount++;                           /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
        hcryp->CrypInCount++;                           /* 执行语句 */

        if (hcryp->CrypInCount == ((uint16_t)(hcryp->Size) / 4U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable interruption */
          __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_INI);    /* HAL辅助宏/函数 */
          /* Call the input data transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)             /* 条件编译 */
          /*Call registered Input complete callback*/
          hcryp->InCpltCallback(hcryp);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
          /*Call legacy weak Input complete callback*/
          HAL_CRYP_InCpltCallback(hcryp);               /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    if (__HAL_CRYP_GET_IT(hcryp, CRYP_IT_OUTI) != 0x0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (__HAL_CRYP_GET_FLAG(hcryp, CRYP_FLAG_OUTRIS) != 0x0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Read the output block from the Output FIFO and put them in temporary Buffer then get CrypOutBuff from temporary buffer  */
        for (i = 0U; i < 2U; i++)                       /* for循环 */
        {                                               /* 代码块开始 */
          temp[i] = hcryp->Instance->DOUT;              /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        i = 0U;                                         /* 赋值或初始化 */
        while (((hcryp->CrypOutCount < ((hcryp->Size) / 4U))) && (i < 2U))  /* while循环 */
        {                                               /* 代码块开始 */
          *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
          hcryp->CrypOutCount++;                        /* 执行语句 */
          i++;                                          /* 执行语句 */
        }                                               /* 代码块结束 */
        if (hcryp->CrypOutCount == ((uint16_t)(hcryp->Size) / 4U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable interruption */
          __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_OUTI);   /* HAL辅助宏/函数 */

          /* Disable CRYP */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */

          /* Change the CRYP state */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */
          /* Call output transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
          /*Call registered Output complete callback*/
          hcryp->OutCpltCallback(hcryp);                /* 执行语句 */
#else                                                   /* 条件编译否则 */
          /*Call legacy weak Output complete callback*/
          HAL_CRYP_OutCpltCallback(hcryp);              /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */
    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hcryp->ErrorCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak error callback*/
    HAL_CRYP_ErrorCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

#endif /* CRYP */

/**
  * @brief  Encryption in ECB/CBC & CTR Algorithm with AES Standard
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure
  * @param  Timeout: specify Timeout value
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AES_Encrypt(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint16_t outcount;  /* Temporary CrypOutCount Value */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */

    /*  Set the Key*/
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

    if (hcryp->Init.Algorithm != CRYP_AES_ECB)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the Initialization Vector*/
#if defined (AES)                                       /* 条件编译 */
      hcryp->Instance->IVR3 = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IVR2 = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IVR1 = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IVR0 = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
#else /* CRYP */
      hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IV1LR = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IV1RR = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
#endif /* End AES or CRYP */
    }                                                   /* 代码块结束 */
  } /* if (DoKeyIVConfig == 1U) */

  /* Set the phase */
  hcryp->Phase = CRYP_PHASE_PROCESS;                    /* 赋值或初始化 */

  /* Enable CRYP */
  __HAL_CRYP_ENABLE(hcryp);                             /* HAL辅助宏/函数 */

  /*Temporary CrypOutCount Value*/
  outcount = hcryp->CrypOutCount;                       /* 赋值或初始化 */

  while ((hcryp->CrypInCount < (hcryp->Size / 4U)) && (outcount < (hcryp->Size / 4U)))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Write plain Ddta and get cipher data */
    CRYP_AES_ProcessData(hcryp, Timeout);               /* 执行语句 */
    /*Temporary CrypOutCount Value*/
    outcount = hcryp->CrypOutCount;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Disable CRYP */
  __HAL_CRYP_DISABLE(hcryp);                            /* HAL辅助宏/函数 */

  /* Change the CRYP state */
  hcryp->State = HAL_CRYP_STATE_READY;                  /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Encryption in ECB/CBC & CTR mode with AES Standard using interrupt mode
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AES_Encrypt_IT(CRYP_HandleTypeDef *hcryp)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*  Set the Key*/
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

    if (hcryp->Init.Algorithm != CRYP_AES_ECB)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the Initialization Vector*/
#if defined (AES)                                       /* 条件编译 */
      hcryp->Instance->IVR3 = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IVR2 = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IVR1 = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IVR0 = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */

#else /* CRYP */
      hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IV1LR = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IV1RR = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
#endif /* End AES or CRYP */
    }                                                   /* 代码块结束 */
  } /* if (DoKeyIVConfig == 1U) */

  /* Set the phase */
  hcryp->Phase = CRYP_PHASE_PROCESS;                    /* 赋值或初始化 */

  if (hcryp->Size != 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
#if defined (AES)                                       /* 条件编译 */

    /* Enable computation complete flag and error interrupts */
    __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_CCFIE | CRYP_IT_ERRIE);  /* HAL辅助宏/函数 */

    /* Enable CRYP */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /* Write the input block in the IN FIFO */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */

#else /* CRYP */

    /* Enable interrupts */
    __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_INI | CRYP_IT_OUTI);  /* HAL辅助宏/函数 */

    /* Enable CRYP */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

#endif /*  End AES or CRYP  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Change the CRYP state */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Decryption in ECB/CBC & CTR mode with AES Standard
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure
  * @param  Timeout: Specify Timeout value
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AES_Decrypt(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint16_t outcount;  /* Temporary CrypOutCount Value */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*  Key preparation for ECB/CBC */
    if (hcryp->Init.Algorithm != CRYP_AES_CTR)          /* 条件判断 */
    {                                                   /* 代码块开始 */
#if defined (AES)                                       /* 条件编译 */
      if (hcryp->AutoKeyDerivation == DISABLE)/*Mode 2 Key preparation*/
      {                                                 /* 代码块开始 */
        /* Set key preparation for decryption operating mode*/
        MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_KEYDERIVATION);  /* 执行语句 */

        /*  Set the Key*/
        CRYP_SetKey(hcryp, hcryp->Init.KeySize);        /* 执行语句 */

        /* Enable CRYP */
        __HAL_CRYP_ENABLE(hcryp);                       /* HAL辅助宏/函数 */

        /* Wait for CCF flag to be raised */
        if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state & error code*/
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
        /* Clear CCF Flag */
        __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);   /* HAL辅助宏/函数 */

        /* Return to decryption operating mode(Mode 3)*/
        MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_DECRYPT);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else /*Mode 4 : decryption & Key preparation*/
      {                                                 /* 代码块开始 */
        /*  Set the Key*/
        CRYP_SetKey(hcryp, hcryp->Init.KeySize);        /* 执行语句 */

        /* Set decryption & Key preparation operating mode*/
        MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_KEYDERIVATION_DECRYPT);  /* 执行语句 */
      }                                                 /* 代码块结束 */
#else /* CRYP */
      /* change ALGOMODE to key preparation for decryption*/
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, CRYP_CR_ALGOMODE_AES_KEY);  /* 执行语句 */

      /*  Set the Key*/
      CRYP_SetKey(hcryp, hcryp->Init.KeySize);          /* 执行语句 */

      /* Enable CRYP */
      __HAL_CRYP_ENABLE(hcryp);                         /* HAL辅助宏/函数 */

      /* Wait for BUSY flag to be raised */
      if (CRYP_WaitOnBUSYFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Turn back to ALGOMODE of the configuration */
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, hcryp->Init.Algorithm);  /* 执行语句 */

#endif /* End AES or CRYP  */
    }                                                   /* 代码块结束 */
    else  /*Algorithm CTR */
    {                                                   /* 代码块开始 */
      /*  Set the Key*/
      CRYP_SetKey(hcryp, hcryp->Init.KeySize);          /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Set IV */
    if (hcryp->Init.Algorithm != CRYP_AES_ECB)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the Initialization Vector*/
#if defined (AES)                                       /* 条件编译 */
      hcryp->Instance->IVR3 = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IVR2 = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IVR1 = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IVR0 = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
#else /* CRYP */
      hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IV1LR = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IV1RR = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
#endif /* End AES or CRYP */
    }                                                   /* 代码块结束 */
  } /* if (DoKeyIVConfig == 1U) */
  /* Set the phase */
  hcryp->Phase = CRYP_PHASE_PROCESS;                    /* 赋值或初始化 */

  /* Enable CRYP */
  __HAL_CRYP_ENABLE(hcryp);                             /* HAL辅助宏/函数 */

  /*Temporary CrypOutCount Value*/
  outcount = hcryp->CrypOutCount;                       /* 赋值或初始化 */

  while ((hcryp->CrypInCount < (hcryp->Size / 4U)) && (outcount < (hcryp->Size / 4U)))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Write plain data and get cipher data */
    CRYP_AES_ProcessData(hcryp, Timeout);               /* 执行语句 */
    /*Temporary CrypOutCount Value*/
    outcount = hcryp->CrypOutCount;                     /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Disable CRYP */
  __HAL_CRYP_DISABLE(hcryp);                            /* HAL辅助宏/函数 */

  /* Change the CRYP state */
  hcryp->State = HAL_CRYP_STATE_READY;                  /* 赋值或初始化 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Decryption in ECB/CBC & CTR mode with AES Standard using interrupt mode
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AES_Decrypt_IT(CRYP_HandleTypeDef *hcryp)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*  Key preparation for ECB/CBC */
    if (hcryp->Init.Algorithm != CRYP_AES_CTR)          /* 条件判断 */
    {                                                   /* 代码块开始 */
#if defined (AES)                                       /* 条件编译 */
      if (hcryp->AutoKeyDerivation == DISABLE)/*Mode 2 Key preparation*/
      {                                                 /* 代码块开始 */
        /* Set key preparation for decryption operating mode*/
        MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_KEYDERIVATION);  /* 执行语句 */

        /*  Set the Key*/
        CRYP_SetKey(hcryp, hcryp->Init.KeySize);        /* 执行语句 */

        /* Enable CRYP */
        __HAL_CRYP_ENABLE(hcryp);                       /* HAL辅助宏/函数 */

        /* Wait for CCF flag to be raised */
        count = CRYP_TIMEOUT_KEYPREPARATION;            /* 赋值或初始化 */
        do                                              /* do-while循环 */
        {                                               /* 代码块开始 */
          count-- ;                                     /* 执行语句 */
          if (count == 0U)                              /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable the CRYP peripheral clock */
            __HAL_CRYP_DISABLE(hcryp);                  /* HAL辅助宏/函数 */

            /* Change state */
            hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
            hcryp->State = HAL_CRYP_STATE_READY;        /* 赋值或初始化 */

            /* Process unlocked */
            __HAL_UNLOCK(hcryp);                        /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */

        /* Clear CCF Flag */
        __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);   /* HAL辅助宏/函数 */

        /* Return to decryption operating mode(Mode 3)*/
        MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_DECRYPT);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else /*Mode 4 : decryption & key preparation*/
      {                                                 /* 代码块开始 */
        /*  Set the Key*/
        CRYP_SetKey(hcryp, hcryp->Init.KeySize);        /* 执行语句 */

        /* Set decryption & key preparation operating mode*/
        MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_KEYDERIVATION_DECRYPT);  /* 执行语句 */
      }                                                 /* 代码块结束 */
#else /* CRYP */

      /* change ALGOMODE to key preparation for decryption*/
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, CRYP_CR_ALGOMODE_AES_KEY);  /* 执行语句 */

      /*  Set the Key*/
      CRYP_SetKey(hcryp, hcryp->Init.KeySize);          /* 执行语句 */

      /* Enable CRYP */
      __HAL_CRYP_ENABLE(hcryp);                         /* HAL辅助宏/函数 */

      /* Wait for BUSY flag to be raised */
      count = CRYP_TIMEOUT_KEYPREPARATION;              /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        count-- ;                                       /* 执行语句 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      } while (HAL_IS_BIT_SET(hcryp->Instance->SR, CRYP_FLAG_BUSY));  /* 代码块结束 */

      /* Turn back to ALGOMODE of the configuration */
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, hcryp->Init.Algorithm);  /* 执行语句 */

#endif /* End AES or CRYP */
    }                                                   /* 代码块结束 */

    else  /*Algorithm CTR */
    {                                                   /* 代码块开始 */
      /*  Set the Key*/
      CRYP_SetKey(hcryp, hcryp->Init.KeySize);          /* 执行语句 */
    }                                                   /* 代码块结束 */

    /* Set IV */
    if (hcryp->Init.Algorithm != CRYP_AES_ECB)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the Initialization Vector*/
#if defined (AES)                                       /* 条件编译 */
      hcryp->Instance->IVR3 = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IVR2 = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IVR1 = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IVR0 = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
#else /* CRYP */
      hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IV1LR = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IV1RR = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
#endif /* End AES or CRYP */
    }                                                   /* 代码块结束 */
  } /* if (DoKeyIVConfig == 1U) */

  /* Set the phase */
  hcryp->Phase = CRYP_PHASE_PROCESS;                    /* 赋值或初始化 */
  if (hcryp->Size != 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */

#if defined (AES)                                       /* 条件编译 */

    /* Enable computation complete flag and error interrupts */
    __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_CCFIE | CRYP_IT_ERRIE);  /* HAL辅助宏/函数 */

    /* Enable CRYP */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /* Write the input block in the IN FIFO */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */

#else /* CRYP */

    /* Enable interrupts */
    __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_INI | CRYP_IT_OUTI);  /* HAL辅助宏/函数 */

    /* Enable CRYP */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

#endif /* End AES or CRYP */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process locked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Change the CRYP state */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  Decryption in ECB/CBC & CTR mode with AES Standard using DMA mode
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AES_Decrypt_DMA(CRYP_HandleTypeDef *hcryp)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*  Key preparation for ECB/CBC */
    if (hcryp->Init.Algorithm != CRYP_AES_CTR)          /* 条件判断 */
    {                                                   /* 代码块开始 */
#if defined (AES)                                       /* 条件编译 */
      if (hcryp->AutoKeyDerivation == DISABLE)/*Mode 2 key preparation*/
      {                                                 /* 代码块开始 */
        /* Set key preparation for decryption operating mode*/
        MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_KEYDERIVATION);  /* 执行语句 */

        /*  Set the Key*/
        CRYP_SetKey(hcryp, hcryp->Init.KeySize);        /* 执行语句 */

        /* Enable CRYP */
        __HAL_CRYP_ENABLE(hcryp);                       /* HAL辅助宏/函数 */

        /* Wait for CCF flag to be raised */
        count = CRYP_TIMEOUT_KEYPREPARATION;            /* 赋值或初始化 */
        do                                              /* do-while循环 */
        {                                               /* 代码块开始 */
          count-- ;                                     /* 执行语句 */
          if (count == 0U)                              /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable the CRYP peripheral clock */
            __HAL_CRYP_DISABLE(hcryp);                  /* HAL辅助宏/函数 */

            /* Change state */
            hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
            hcryp->State = HAL_CRYP_STATE_READY;        /* 赋值或初始化 */

            /* Process unlocked */
            __HAL_UNLOCK(hcryp);                        /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */

        /* Clear CCF Flag */
        __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);   /* HAL辅助宏/函数 */

        /* Return to decryption operating mode(Mode 3)*/
        MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_DECRYPT);  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else /*Mode 4 : decryption & key preparation*/
      {                                                 /* 代码块开始 */
        /*  Set the Key*/
        CRYP_SetKey(hcryp, hcryp->Init.KeySize);        /* 执行语句 */

        /* Set decryption & Key preparation operating mode*/
        MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_KEYDERIVATION_DECRYPT);  /* 执行语句 */
      }                                                 /* 代码块结束 */
#else /* CRYP */
      /* change ALGOMODE to key preparation for decryption*/
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, CRYP_CR_ALGOMODE_AES_KEY);  /* 执行语句 */

      /*  Set the Key*/
      CRYP_SetKey(hcryp, hcryp->Init.KeySize);          /* 执行语句 */

      /* Enable CRYP */
      __HAL_CRYP_ENABLE(hcryp);                         /* HAL辅助宏/函数 */

      /* Wait for BUSY flag to be raised */
      count = CRYP_TIMEOUT_KEYPREPARATION;              /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        count-- ;                                       /* 执行语句 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      } while (HAL_IS_BIT_SET(hcryp->Instance->SR, CRYP_FLAG_BUSY));  /* 代码块结束 */

      /* Turn back to ALGOMODE of the configuration */
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, hcryp->Init.Algorithm);  /* 执行语句 */

#endif /* End AES or CRYP  */
    }                                                   /* 代码块结束 */
    else  /*Algorithm CTR */
    {                                                   /* 代码块开始 */
      /*  Set the Key*/
      CRYP_SetKey(hcryp, hcryp->Init.KeySize);          /* 执行语句 */
    }                                                   /* 代码块结束 */

    if (hcryp->Init.Algorithm != CRYP_AES_ECB)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Set the Initialization Vector*/
#if defined (AES)                                       /* 条件编译 */
      hcryp->Instance->IVR3 = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IVR2 = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IVR1 = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IVR0 = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
#else /* CRYP */
      hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
      hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
      hcryp->Instance->IV1LR = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
      hcryp->Instance->IV1RR = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */
#endif /* End AES or CRYP  */
    }                                                   /* 代码块结束 */
  } /* if (DoKeyIVConfig == 1U) */

  /* Set the phase */
  hcryp->Phase = CRYP_PHASE_PROCESS;                    /* 赋值或初始化 */

  if (hcryp->Size != 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the input and output addresses and start DMA transfer */
    CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), (hcryp->Size / 4U), (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Change the CRYP state */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  DMA CRYP input data process complete callback.
  * @param  hdma: DMA handle
  * @retval None
  */
static void CRYP_DMAInCplt(DMA_HandleTypeDef *hdma)     /* 静态函数 */
{                                                       /* 代码块开始 */
  CRYP_HandleTypeDef *hcryp = (CRYP_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Disable the DMA transfer for input FIFO request by resetting the DIEN bit
  in the DMACR register */
#if defined (CRYP)                                      /* 条件编译 */
  hcryp->Instance->DMACR &= (uint32_t)(~CRYP_DMACR_DIEN);  /* 赋值或初始化 */

#else /* AES */
  CLEAR_BIT(hcryp->Instance->CR, AES_CR_DMAINEN);       /* 执行语句 */

  /* TinyAES2, No output on CCM AES, unlock should be done when input data process complete */
  if ((hcryp->Init.Algorithm & CRYP_AES_CCM) == CRYP_AES_CCM)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Clear CCF flag */
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);       /* HAL辅助宏/函数 */

    /* Change the CRYP state to ready */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */

    /* Process Unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
#endif /* End AES or CRYP */

  /* Call input data transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /*Call registered Input complete callback*/
  hcryp->InCpltCallback(hcryp);                         /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /*Call legacy weak Input complete callback*/
  HAL_CRYP_InCpltCallback(hcryp);                       /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA CRYP output data process complete callback.
  * @param  hdma: DMA handle
  * @retval None
  */
static void CRYP_DMAOutCplt(DMA_HandleTypeDef *hdma)    /* 静态函数 */
{                                                       /* 代码块开始 */
  CRYP_HandleTypeDef *hcryp = (CRYP_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Disable the DMA transfer for output FIFO request by resetting
  the DOEN bit in the DMACR register */

#if defined (CRYP)                                      /* 条件编译 */

  hcryp->Instance->DMACR &= (uint32_t)(~CRYP_DMACR_DOEN);  /* 赋值或初始化 */
  #if defined (CRYP_CR_ALGOMODE_AES_GCM)                /* 条件编译 */
  if ((hcryp->Init.Algorithm & CRYP_AES_GCM) != CRYP_AES_GCM)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable CRYP  (not allowed in  GCM)*/
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  #else /*NO GCM CCM */
  /* Disable CRYP */
  __HAL_CRYP_DISABLE(hcryp);                            /* HAL辅助宏/函数 */
  #endif /* GCM CCM defined*/
#else /* AES */

  CLEAR_BIT(hcryp->Instance->CR, AES_CR_DMAOUTEN);      /* 执行语句 */

  /* Clear CCF flag */
  __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);         /* HAL辅助宏/函数 */

  if ((hcryp->Init.Algorithm & CRYP_AES_GCM_GMAC) != CRYP_AES_GCM_GMAC)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable CRYP (not allowed in  GCM)*/
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
#endif /* End AES or CRYP */

  /* Change the CRYP state to ready */
  hcryp->State = HAL_CRYP_STATE_READY;                  /* 赋值或初始化 */

  /* Process unlocked */
  __HAL_UNLOCK(hcryp);                                  /* HAL辅助宏/函数 */
  /* Call output data transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /*Call registered Output complete callback*/
  hcryp->OutCpltCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /*Call legacy weak Output complete callback*/
  HAL_CRYP_OutCpltCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  DMA CRYP communication error callback.
  * @param  hdma: DMA handle
  * @retval None
  */
static void CRYP_DMAError(DMA_HandleTypeDef *hdma)      /* 静态函数 */
{                                                       /* 代码块开始 */
  CRYP_HandleTypeDef *hcryp = (CRYP_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;  /* 赋值或初始化 */

  /* Change the CRYP peripheral state */
  hcryp->State = HAL_CRYP_STATE_READY;                  /* 赋值或初始化 */

  /* DMA error code field */
  hcryp->ErrorCode |= HAL_CRYP_ERROR_DMA;               /* 赋值或初始化 */

#if defined (AES)                                       /* 条件编译 */

  /* Clear CCF flag */
  __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);         /* HAL辅助宏/函数 */

#endif /* AES */

  /* Call error callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
  /*Call registered error callback*/
  hcryp->ErrorCallback(hcryp);                          /* 执行语句 */
#else                                                   /* 条件编译否则 */
  /*Call legacy weak error callback*/
  HAL_CRYP_ErrorCallback(hcryp);                        /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
}                                                       /* 代码块结束 */

/**
  * @brief  Set the DMA configuration and start the DMA transfer
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  inputaddr: address of the input buffer
  * @param  Size: size of the input buffer, must be a multiple of 16.
  * @param  outputaddr: address of the output buffer
  * @retval None
  */
static void CRYP_SetDMAConfig(CRYP_HandleTypeDef *hcryp, uint32_t inputaddr, uint16_t Size, uint32_t outputaddr)  /* 静态函数 */
{                                                       /* 代码块开始 */
  /* Set the CRYP DMA transfer complete callback */
  hcryp->hdmain->XferCpltCallback = CRYP_DMAInCplt;     /* 赋值或初始化 */

  /* Set the DMA input error callback */
  hcryp->hdmain->XferErrorCallback = CRYP_DMAError;     /* 赋值或初始化 */

  /* Set the CRYP DMA transfer complete callback */
  hcryp->hdmaout->XferCpltCallback = CRYP_DMAOutCplt;   /* 赋值或初始化 */

  /* Set the DMA output error callback */
  hcryp->hdmaout->XferErrorCallback = CRYP_DMAError;    /* 赋值或初始化 */

#if defined (CRYP)                                      /* 条件编译 */

  /* Enable CRYP */
  __HAL_CRYP_ENABLE(hcryp);                             /* HAL辅助宏/函数 */

  /* Enable the input DMA Stream */
  if (HAL_DMA_Start_IT(hcryp->hdmain, inputaddr, (uint32_t)&hcryp->Instance->DIN, Size) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* DMA error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_DMA;             /* 赋值或初始化 */

    /* Call error callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hcryp->ErrorCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak error callback*/
    HAL_CRYP_ErrorCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  /* Enable the output DMA Stream */
  if (HAL_DMA_Start_IT(hcryp->hdmaout, (uint32_t)&hcryp->Instance->DOUT, outputaddr, Size) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* DMA error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_DMA;             /* 赋值或初始化 */

    /* Call error callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hcryp->ErrorCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak error callback*/
    HAL_CRYP_ErrorCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  /* Enable In/Out DMA request */
  hcryp->Instance->DMACR = CRYP_DMACR_DOEN | CRYP_DMACR_DIEN;  /* 赋值或初始化 */

#else /* AES */

  if (((hcryp->Init.Algorithm & CRYP_AES_GCM_GMAC) != CRYP_AES_GCM_GMAC)  /* 条件判断 */
      && ((hcryp->Init.Algorithm & CRYP_AES_CCM) != CRYP_AES_CCM))  /* 源代码 */
  {                                                     /* 代码块开始 */
    /* Enable CRYP (not allowed in  GCM & CCM)*/
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */

  /* Enable the DMA input stream */
  if (HAL_DMA_Start_IT(hcryp->hdmain, inputaddr, (uint32_t)&hcryp->Instance->DINR, Size) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* DMA error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_DMA;             /* 赋值或初始化 */

    /* Call error callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hcryp->ErrorCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak error callback*/
    HAL_CRYP_ErrorCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  /* Enable the DMA output stream */
  if (HAL_DMA_Start_IT(hcryp->hdmaout, (uint32_t)&hcryp->Instance->DOUTR, outputaddr, Size) != HAL_OK)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* DMA error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_DMA;             /* 赋值或初始化 */

    /* Call error callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hcryp->ErrorCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak error callback*/
    HAL_CRYP_ErrorCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
  /*AES2v1.1.1 : CCM authentication : no init phase, only header and final phase */
  /* Enable In and Out DMA requests */
  if ((hcryp->Init.Algorithm & CRYP_AES_CCM) == CRYP_AES_CCM)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Enable only In DMA requests for CCM*/
    SET_BIT(hcryp->Instance->CR, (AES_CR_DMAINEN));     /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Enable In and Out DMA requests */
    SET_BIT(hcryp->Instance->CR, (AES_CR_DMAINEN | AES_CR_DMAOUTEN));  /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* End AES or CRYP */
}                                                       /* 代码块结束 */

/**
  * @brief  Process Data: Write Input data in polling mode and used in AES functions.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Timeout: Specify Timeout value
  * @retval None
  */
static void CRYP_AES_ProcessData(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态函数 */
{                                                       /* 代码块开始 */

  uint32_t temp[4];  /* Temporary CrypOutBuff */
  uint32_t i;                                           /* 32位无符号变量/函数 */
#if defined (CRYP)                                      /* 条件编译 */
  uint16_t incount;  /* Temporary CrypInCount Value */
  uint16_t outcount;  /* Temporary CrypOutCount Value */
#endif                                                  /* 条件编译结束 */

#if defined (CRYP)                                      /* 条件编译 */

  /*Temporary CrypOutCount Value*/
  incount = hcryp->CrypInCount;                         /* 赋值或初始化 */

  if (((hcryp->Instance->SR & CRYP_FLAG_IFNF) != 0x0U) && (incount < (hcryp->Size / 4U)))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Write the input block in the IN FIFO */
    hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Wait for OFNE flag to be raised */
  if (CRYP_WaitOnOFNEFlag(hcryp, Timeout) != HAL_OK)    /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the CRYP peripheral clock */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

    /* Change state & error code*/
    hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;         /* 赋值或初始化 */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hcryp->ErrorCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak error callback*/
    HAL_CRYP_ErrorCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /*Temporary CrypOutCount Value*/
  outcount = hcryp->CrypOutCount;                       /* 赋值或初始化 */

  if (((hcryp->Instance->SR & CRYP_FLAG_OFNE) != 0x0U) && (outcount < (hcryp->Size / 4U)))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Read the output block from the Output FIFO and put them in temporary buffer then get CrypOutBuff from temporary buffer  */
    for (i = 0U; i < 4U; i++)                           /* for循环 */
    {                                                   /* 代码块开始 */
      temp[i] = hcryp->Instance->DOUT;                  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    i = 0U;                                             /* 赋值或初始化 */
    while (((hcryp->CrypOutCount < ((hcryp->Size) / 4U))) && (i < 4U))  /* while循环 */
    {                                                   /* 代码块开始 */
      *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
      hcryp->CrypOutCount++;                            /* 执行语句 */
      i++;                                              /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

#else /* AES */

  /* Write the input block in the IN FIFO */
  hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
  hcryp->CrypInCount++;                                 /* 执行语句 */
  hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
  hcryp->CrypInCount++;                                 /* 执行语句 */
  hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
  hcryp->CrypInCount++;                                 /* 执行语句 */
  hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
  hcryp->CrypInCount++;                                 /* 执行语句 */

  /* Wait for CCF flag to be raised */
  if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable the CRYP peripheral clock */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

    /* Change state */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;         /* 赋值或初始化 */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hcryp->ErrorCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak error callback*/
    HAL_CRYP_ErrorCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Clear CCF Flag */
  __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);         /* HAL辅助宏/函数 */

  /* Read the output block from the output FIFO and put them in temporary buffer then get CrypOutBuff from temporary buffer*/
  for (i = 0U; i < 4U; i++)                             /* for循环 */
  {                                                     /* 代码块开始 */
    temp[i] = hcryp->Instance->DOUTR;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  i = 0U;                                               /* 赋值或初始化 */
  while ((hcryp->CrypOutCount < ((hcryp->Size + 3U) / 4U)) && (i < 4U))  /* while循环 */
  {                                                     /* 代码块开始 */
    *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
    hcryp->CrypOutCount++;                              /* 执行语句 */
    i++;                                                /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* End AES or CRYP */
}                                                       /* 代码块结束 */

/**
  * @brief  Handle CRYP block input/output data handling under interruption.
  * @note   The function is called under interruption only, once
  *         interruptions have been enabled by HAL_CRYP_Encrypt_IT or HAL_CRYP_Decrypt_IT.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @retval HAL status
  */
static void CRYP_AES_IT(CRYP_HandleTypeDef *hcryp)      /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t temp[4];  /* Temporary CrypOutBuff */
  uint32_t i;                                           /* 32位无符号变量/函数 */
#if defined (CRYP)                                      /* 条件编译 */
  uint16_t incount; /* Temporary CrypInCount Value */
  uint16_t outcount;  /* Temporary CrypOutCount Value */
#endif                                                  /* 条件编译结束 */

  if (hcryp->State == HAL_CRYP_STATE_BUSY)              /* 条件判断 */
  {                                                     /* 代码块开始 */
#if defined (CRYP)                                      /* 条件编译 */

    /*Temporary CrypOutCount Value*/
    incount = hcryp->CrypInCount;                       /* 赋值或初始化 */
    if (((hcryp->Instance->SR & CRYP_FLAG_IFNF) != 0x0U) && (incount < (hcryp->Size / 4U)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Write the input block in the IN FIFO */
      hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      if (hcryp->CrypInCount == ((uint16_t)(hcryp->Size) / 4U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable interrupts */
        __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_INI);      /* HAL辅助宏/函数 */

        /* Call the input data transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered Input complete callback*/
        hcryp->InCpltCallback(hcryp);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak Input complete callback*/
        HAL_CRYP_InCpltCallback(hcryp);                 /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /*Temporary CrypOutCount Value*/
    outcount = hcryp->CrypOutCount;                     /* 赋值或初始化 */

    if (((hcryp->Instance->SR & CRYP_FLAG_OFNE) != 0x0U) && (outcount < (hcryp->Size / 4U)))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read the output block from the output FIFO and put them in temporary buffer then get CrypOutBuff from temporary buffer  */
      for (i = 0U; i < 4U; i++)                         /* for循环 */
      {                                                 /* 代码块开始 */
        temp[i] = hcryp->Instance->DOUT;                /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      i = 0U;                                           /* 赋值或初始化 */
      while (((hcryp->CrypOutCount < ((hcryp->Size) / 4U))) && (i < 4U))  /* while循环 */
      {                                                 /* 代码块开始 */
        *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
        hcryp->CrypOutCount++;                          /* 执行语句 */
        i++;                                            /* 执行语句 */
      }                                                 /* 代码块结束 */
      if (hcryp->CrypOutCount == ((uint16_t)(hcryp->Size) / 4U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable interrupts */
        __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_OUTI);     /* HAL辅助宏/函数 */

        /* Change the CRYP state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Disable CRYP */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */

        /* Call Output transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered Output complete callback*/
        hcryp->OutCpltCallback(hcryp);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak Output complete callback*/
        HAL_CRYP_OutCpltCallback(hcryp);                /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

#else /*AES*/

    /* Read the output block from the output FIFO and put them in temporary buffer then get CrypOutBuff from temporary buffer*/
    for (i = 0U; i < 4U; i++)                           /* for循环 */
    {                                                   /* 代码块开始 */
      temp[i] = hcryp->Instance->DOUTR;                 /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    i = 0U;                                             /* 赋值或初始化 */
    while ((hcryp->CrypOutCount < ((hcryp->Size + 3U) / 4U)) && (i < 4U))  /* while循环 */
    {                                                   /* 代码块开始 */
      *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
      hcryp->CrypOutCount++;                            /* 执行语句 */
      i++;                                              /* 执行语句 */
    }                                                   /* 代码块结束 */

    if (hcryp->CrypOutCount == (hcryp->Size / 4U))      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable Computation Complete flag and errors interrupts */
      __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_CCFIE | CRYP_IT_ERRIE);  /* HAL辅助宏/函数 */

      /* Change the CRYP state */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Disable CRYP */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      /* Process Unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */

      /* Call Output transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /*Call registered Output complete callback*/
      hcryp->OutCpltCallback(hcryp);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call legacy weak Output complete callback*/
      HAL_CRYP_OutCpltCallback(hcryp);                  /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Write the input block in the IN FIFO */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */

      if (hcryp->CrypInCount == (hcryp->Size / 4U))     /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Input transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)             /* 条件编译 */
        /*Call registered Input complete callback*/
        hcryp->InCpltCallback(hcryp);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak Input complete callback*/
        HAL_CRYP_InCpltCallback(hcryp);                 /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
#endif /* End AES or CRYP  */

  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Busy error code field */
    hcryp->ErrorCode |= HAL_CRYP_ERROR_BUSY;            /* 赋值或初始化 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered error callback*/
    hcryp->ErrorCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak error callback*/
    HAL_CRYP_ErrorCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */
}                                                       /* 代码块结束 */

/**
  * @brief  Writes Key in Key registers.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  KeySize: Size of Key
  * @retval None
  */
static void CRYP_SetKey(CRYP_HandleTypeDef *hcryp, uint32_t KeySize)  /* 静态函数 */
{                                                       /* 代码块开始 */
#if defined (CRYP)                                      /* 条件编译 */

  switch (KeySize)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case CRYP_KEYSIZE_256B:                             /* case标签 */
      hcryp->Instance->K0LR = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
      hcryp->Instance->K0RR = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
      hcryp->Instance->K1LR = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
      hcryp->Instance->K1RR = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */
      hcryp->Instance->K2LR = *(uint32_t *)(hcryp->Init.pKey + 4);  /* 赋值或初始化 */
      hcryp->Instance->K2RR = *(uint32_t *)(hcryp->Init.pKey + 5);  /* 赋值或初始化 */
      hcryp->Instance->K3LR = *(uint32_t *)(hcryp->Init.pKey + 6);  /* 赋值或初始化 */
      hcryp->Instance->K3RR = *(uint32_t *)(hcryp->Init.pKey + 7);  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case CRYP_KEYSIZE_192B:                             /* case标签 */
      hcryp->Instance->K1LR = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
      hcryp->Instance->K1RR = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
      hcryp->Instance->K2LR = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
      hcryp->Instance->K2RR = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */
      hcryp->Instance->K3LR = *(uint32_t *)(hcryp->Init.pKey + 4);  /* 赋值或初始化 */
      hcryp->Instance->K3RR = *(uint32_t *)(hcryp->Init.pKey + 5);  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case CRYP_KEYSIZE_128B:                             /* case标签 */
      hcryp->Instance->K2LR = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
      hcryp->Instance->K2RR = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
      hcryp->Instance->K3LR = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
      hcryp->Instance->K3RR = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */

      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
#else /*AES*/
  switch (KeySize)                                      /* switch分支 */
  {                                                     /* 代码块开始 */
    case CRYP_KEYSIZE_256B:                             /* case标签 */
      hcryp->Instance->KEYR7 = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
      hcryp->Instance->KEYR6 = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
      hcryp->Instance->KEYR5 = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
      hcryp->Instance->KEYR4 = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */
      hcryp->Instance->KEYR3 = *(uint32_t *)(hcryp->Init.pKey + 4);  /* 赋值或初始化 */
      hcryp->Instance->KEYR2 = *(uint32_t *)(hcryp->Init.pKey + 5);  /* 赋值或初始化 */
      hcryp->Instance->KEYR1 = *(uint32_t *)(hcryp->Init.pKey + 6);  /* 赋值或初始化 */
      hcryp->Instance->KEYR0 = *(uint32_t *)(hcryp->Init.pKey + 7);  /* 赋值或初始化 */
      break;                                            /* 跳出 */
    case CRYP_KEYSIZE_128B:                             /* case标签 */
      hcryp->Instance->KEYR3 = *(uint32_t *)(hcryp->Init.pKey);  /* 赋值或初始化 */
      hcryp->Instance->KEYR2 = *(uint32_t *)(hcryp->Init.pKey + 1);  /* 赋值或初始化 */
      hcryp->Instance->KEYR1 = *(uint32_t *)(hcryp->Init.pKey + 2);  /* 赋值或初始化 */
      hcryp->Instance->KEYR0 = *(uint32_t *)(hcryp->Init.pKey + 3);  /* 赋值或初始化 */

      break;                                            /* 跳出 */
    default:                                            /* default分支 */
      break;                                            /* 跳出 */
  }                                                     /* 代码块结束 */
#endif /* End AES or CRYP  */
}                                                       /* 代码块结束 */

#if defined (CRYP_CR_ALGOMODE_AES_GCM)|| defined (AES)  /* 条件编译 */
/**
  * @brief  Encryption/Decryption process in AES GCM mode and prepare the authentication TAG
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Timeout: Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AESGCM_Process(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t wordsize = (uint32_t)(hcryp->Size) / 4U ;    /* 32位无符号变量/函数 */
  uint16_t outcount;  /* Temporary CrypOutCount Value */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
      hcryp->SizesSum += hcryp->Size; /* Compute message total payload length */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
      hcryp->SizesSum = hcryp->Size; /* Merely store payload length */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hcryp->SizesSum = hcryp->Size;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*  Reset CrypHeaderCount */
    hcryp->CrypHeaderCount = 0U;                        /* 赋值或初始化 */

    /****************************** Init phase **********************************/

    CRYP_SET_PHASE(hcryp, CRYP_PHASE_INIT);             /* 执行语句 */

    /* Set the key */
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

#if defined(CRYP)                                       /* 条件编译 */

    /* Set the initialization vector and the counter : Initial Counter Block (ICB)*/
    hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
    hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
    hcryp->Instance->IV1LR = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
    hcryp->Instance->IV1RR = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /*Wait for the CRYPEN bit to be cleared*/
    while ((hcryp->Instance->CR & CRYP_CR_CRYPEN) == CRYP_CR_CRYPEN)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

#else /* AES */
    /* Workaround 1 : only AES.
    Datatype configuration must be 32 bits during Init phase. Only, after Init, and before re
    enabling the IP, datatype different from 32 bits can be configured.*/
    /* Select DATATYPE 32  */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE, CRYP_DATATYPE_32B);  /* 执行语句 */

    /* Set the initialization vector and the counter : Initial Counter Block (ICB)*/
    hcryp->Instance->IVR3 = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
    hcryp->Instance->IVR2 = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
    hcryp->Instance->IVR1 = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
    hcryp->Instance->IVR0 = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /* just wait for hash computation */
    if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Change state */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;       /* 赋值或初始化 */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked & return error */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    /* Clear CCF flag */
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);       /* HAL辅助宏/函数 */

#endif /* End AES or CRYP  */

    /************************ Header phase *************************************/

    if (CRYP_GCMCCM_SetHeaderPhase(hcryp,  Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /*************************Payload phase ************************************/

    /* Set the phase */
    hcryp->Phase = CRYP_PHASE_PROCESS;                  /* 赋值或初始化 */

#if defined(CRYP)                                       /* 条件编译 */

    /* Disable the CRYP peripheral */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

    /* Select payload phase once the header phase is performed */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_PAYLOAD);          /* 执行语句 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

#else /* AES */

    /* Select payload phase once the header phase is performed */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_PAYLOAD);          /* 执行语句 */

#endif /* End AES or CRYP  */
  } /* if (DoKeyIVConfig == 1U) */

  if ((hcryp->Size % 16U) != 0U)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* recalculate  wordsize */
    wordsize = ((wordsize / 4U) * 4U) ;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */
  /*Temporary CrypOutCount Value*/
  outcount = hcryp->CrypOutCount;                       /* 赋值或初始化 */

  /* Write input data and get output Data */
  while ((hcryp->CrypInCount < wordsize) && (outcount < wordsize))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Write plain data and get cipher data */
    CRYP_AES_ProcessData(hcryp, Timeout);               /* 执行语句 */

    /*Temporary CrypOutCount Value*/
    outcount = hcryp->CrypOutCount;                     /* 赋值或初始化 */

    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state & error code */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if ((hcryp->Size % 16U) != 0U)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*  Workaround 2 :  CRYP1 & AES generates correct TAG for GCM mode only when input block size is multiple of
    128 bits. If lthe size of the last block of payload is inferior to 128 bits, when GCM encryption
    is selected, then the TAG message will be wrong.*/
    CRYP_Workaround(hcryp, Timeout);                    /* 执行语句 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Encryption/Decryption process in AES GCM mode and prepare the authentication TAG in interrupt mode
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AESGCM_Process_IT(CRYP_HandleTypeDef *hcryp)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */
#if defined(AES)                                        /* 条件编译 */
  uint32_t loopcounter;                                 /* 32位无符号变量/函数 */
  uint32_t lastwordsize;                                /* 32位无符号变量/函数 */
  uint32_t npblb;                                       /* 32位无符号变量/函数 */
#endif /* AES */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
      hcryp->SizesSum += hcryp->Size; /* Compute message total payload length */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
      hcryp->SizesSum = hcryp->Size; /* Merely store payload length */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hcryp->SizesSum = hcryp->Size;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Configure Key, IV and process message (header and payload) */
  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*  Reset CrypHeaderCount */
    hcryp->CrypHeaderCount = 0U;                        /* 赋值或初始化 */

    /******************************* Init phase *********************************/

    CRYP_SET_PHASE(hcryp, CRYP_PHASE_INIT);             /* 执行语句 */

    /* Set the key */
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

#if defined(CRYP)                                       /* 条件编译 */
    /* Set the initialization vector and the counter : Initial Counter Block (ICB)*/
    hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
    hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
    hcryp->Instance->IV1LR = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
    hcryp->Instance->IV1RR = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /*Wait for the CRYPEN bit to be cleared*/
    count = CRYP_TIMEOUT_GCMCCMINITPHASE;               /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count-- ;                                         /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    } while ((hcryp->Instance->CR & CRYP_CR_CRYPEN) == CRYP_CR_CRYPEN);  /* 代码块结束 */

#else /* AES */

    /* Workaround 1 : only AES
    Datatype configuration must be 32 bits during INIT phase. Only, after INIT, and before re
    enabling the IP, datatype different from 32 bits can be configured.*/
    /* Select DATATYPE 32  */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE, CRYP_DATATYPE_32B);  /* 执行语句 */

    /* Set the initialization vector and the counter : Initial Counter Block (ICB)*/
    hcryp->Instance->IVR3 = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
    hcryp->Instance->IVR2 = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
    hcryp->Instance->IVR1 = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
    hcryp->Instance->IVR0 = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /* just wait for hash computation */
    count = CRYP_TIMEOUT_GCMCCMINITPHASE;               /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count-- ;                                         /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */

    /* Clear CCF flag */
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);       /* HAL辅助宏/函数 */

#endif /* End AES or CRYP */

    /***************************** Header phase *********************************/

#if defined(CRYP)                                       /* 条件编译 */

    /* Select header phase */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);           /* 执行语句 */

    /* Enable interrupts */
    __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_INI);           /* HAL辅助宏/函数 */

    /* Enable CRYP */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

#else /* AES */

    /* Workaround 1: only AES , before re-enabling the IP, datatype can be configured*/
    MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE, hcryp->Init.DataType);  /* 执行语句 */

    /* Select header phase */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);           /* 执行语句 */

    /* Enable computation complete flag and error interrupts */
    __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_CCFIE | CRYP_IT_ERRIE);  /* HAL辅助宏/函数 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    if (hcryp->Init.HeaderSize == 0U) /*header phase is  skipped*/
    {                                                   /* 代码块开始 */
      /* Set the phase */
      hcryp->Phase = CRYP_PHASE_PROCESS;                /* 赋值或初始化 */

      /* Select payload phase once the header phase is performed */
      MODIFY_REG(hcryp->Instance->CR, AES_CR_GCMPH, CRYP_PHASE_PAYLOAD);  /* 执行语句 */

      /* Write the payload Input block in the IN FIFO */
      if (hcryp->Size == 0U)                            /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable interrupts */
        __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_CCFIE | CRYP_IT_ERRIE);  /* HAL辅助宏/函数 */

        /* Change the CRYP state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      else if (hcryp->Size >= 16U)                      /* 否则如果 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
        hcryp->CrypInCount++;                           /* 执行语句 */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
        hcryp->CrypInCount++;                           /* 执行语句 */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
        hcryp->CrypInCount++;                           /* 执行语句 */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
        hcryp->CrypInCount++;                           /* 执行语句 */
        if (hcryp->CrypInCount == (hcryp->Size / 4U))   /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Call Input transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
          /*Call registered Input complete callback*/
          hcryp->InCpltCallback(hcryp);                 /* 执行语句 */
#else                                                   /* 条件编译否则 */
          /*Call legacy weak Input complete callback*/
          HAL_CRYP_InCpltCallback(hcryp);               /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else /* Size < 16Bytes  : first block is the last block*/
      {                                                 /* 代码块开始 */
        /* Workaround not implemented*/
        /* Size should be %4  otherwise Tag will  be incorrectly generated for GCM Encryption:
        Workaround is implemented in polling mode, so if last block of
        payload <128bit don't use CRYP_Encrypt_IT otherwise TAG is incorrectly generated for GCM Encryption. */

        /* Compute the number of padding bytes in last block of payload */
        npblb = 16U - (uint32_t)(hcryp->Size);          /* 赋值或初始化 */

        /* Number of valid words (lastwordsize) in last block */
        if ((npblb % 4U) == 0U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          lastwordsize = (16U - npblb) / 4U;            /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        else                                            /* 否则 */
        {                                               /* 代码块开始 */
          lastwordsize = ((16U - npblb) / 4U) + 1U;     /* 赋值或初始化 */
        }                                               /* 代码块结束 */

        /*  last block optionally pad the data with zeros*/
        for (loopcounter = 0U; loopcounter < lastwordsize ; loopcounter++)  /* for循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
          hcryp->CrypInCount++;                         /* 执行语句 */
        }                                               /* 代码块结束 */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          /* pad the data with zeros to have a complete block */
          hcryp->Instance->DINR = 0x0U;                 /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if ((hcryp->Init.HeaderSize) < 4U)             /* 否则如果 */
    {                                                   /* 代码块开始 */
      for (loopcounter = 0U; loopcounter < hcryp->Init.HeaderSize ; loopcounter++)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
      }                                                 /* 代码块结束 */
      while (loopcounter < 4U)                          /* while循环 */
      {                                                 /* 代码块开始 */
        /* pad the data with zeros to have a complete block */
        hcryp->Instance->DINR = 0x0U;                   /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* Set the phase */
      hcryp->Phase = CRYP_PHASE_PROCESS;                /* 赋值或初始化 */

      /* Select payload phase once the header phase is performed */
      CRYP_SET_PHASE(hcryp, CRYP_PHASE_PAYLOAD);        /* 执行语句 */

      /* Call Input transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /*Call registered Input complete callback*/
      hcryp->InCpltCallback(hcryp);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call legacy weak Input complete callback*/
      HAL_CRYP_InCpltCallback(hcryp);                   /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    else if ((hcryp->Init.HeaderSize) >= 4U)            /* 否则如果 */
    {                                                   /* 代码块开始 */
      /* Write the input block in the IN FIFO */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
      hcryp->CrypHeaderCount++;                         /* 执行语句 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
      hcryp->CrypHeaderCount++;                         /* 执行语句 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
      hcryp->CrypHeaderCount++;                         /* 执行语句 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
      hcryp->CrypHeaderCount++;                         /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Nothing to do */
    }                                                   /* 代码块结束 */

#endif /* End AES or CRYP */
  } /* end of if (DoKeyIVConfig == 1U) */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Encryption/Decryption process in AES GCM mode and prepare the authentication TAG using DMA
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AESGCM_Process_DMA(CRYP_HandleTypeDef *hcryp)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t wordsize;                                    /* 32位无符号变量/函数 */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
      hcryp->SizesSum += hcryp->Size; /* Compute message total payload length */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
      hcryp->SizesSum = hcryp->Size; /* Merely store payload length */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hcryp->SizesSum = hcryp->Size;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*  Reset CrypHeaderCount */
    hcryp->CrypHeaderCount = 0U;                        /* 赋值或初始化 */

    /*************************** Init phase ************************************/

    CRYP_SET_PHASE(hcryp, CRYP_PHASE_INIT);             /* 执行语句 */

    /* Set the key */
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

#if defined(CRYP)                                       /* 条件编译 */
    /* Set the initialization vector and the counter : Initial Counter Block (ICB)*/
    hcryp->Instance->IV0LR = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
    hcryp->Instance->IV0RR = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
    hcryp->Instance->IV1LR = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
    hcryp->Instance->IV1RR = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /*Wait for the CRYPEN bit to be cleared*/
    count = CRYP_TIMEOUT_GCMCCMINITPHASE;               /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count-- ;                                         /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    } while ((hcryp->Instance->CR & CRYP_CR_CRYPEN) == CRYP_CR_CRYPEN);  /* 代码块结束 */

#else /* AES */

    /*Workaround 1 : only AES
    Datatype configuration must be 32 bits during Init phase. Only, after Init, and before re
    enabling the IP, datatype different from 32 bits can be configured.*/
    /* Select DATATYPE 32  */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE, CRYP_DATATYPE_32B);  /* 执行语句 */

    /* Set the initialization vector and the counter : Initial Counter Block (ICB)*/
    hcryp->Instance->IVR3 = *(uint32_t *)(hcryp->Init.pInitVect);  /* 赋值或初始化 */
    hcryp->Instance->IVR2 = *(uint32_t *)(hcryp->Init.pInitVect + 1);  /* 赋值或初始化 */
    hcryp->Instance->IVR1 = *(uint32_t *)(hcryp->Init.pInitVect + 2);  /* 赋值或初始化 */
    hcryp->Instance->IVR0 = *(uint32_t *)(hcryp->Init.pInitVect + 3);  /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /* just wait for hash computation */
    count = CRYP_TIMEOUT_GCMCCMINITPHASE;               /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count-- ;                                         /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */

    /* Clear CCF flag */
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);       /* HAL辅助宏/函数 */

#endif /* End AES or CRYP */

    /************************ Header phase *************************************/

    if (CRYP_GCMCCM_SetHeaderPhase_DMA(hcryp) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /************************ Payload phase ************************************/

    /* Set the phase */
    hcryp->Phase = CRYP_PHASE_PROCESS;                  /* 赋值或初始化 */

#if defined(CRYP)                                       /* 条件编译 */

    /* Disable the CRYP peripheral */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

#endif /* CRYP */

    /* Select payload phase once the header phase is performed */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_PAYLOAD);          /* 执行语句 */

  } /* if (DoKeyIVConfig == 1U) */

  if (hcryp->Size != 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* CRYP1 IP V < 2.2.1  Size should be %4  otherwise Tag will  be incorrectly generated for GCM Encryption:
    Workaround is implemented in polling mode, so if last block of
    payload <128bit don't use DMA mode otherwise TAG is incorrectly generated . */
    /* Set the input and output addresses and start DMA transfer */
    if ((hcryp->Size % 16U) == 0U)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), (hcryp->Size / 4U), (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else /*to compute last word<128bits, otherwise it will not be encrypted/decrypted */
    {                                                   /* 代码块开始 */
      wordsize = (uint32_t)(hcryp->Size) + (16U - ((uint32_t)(hcryp->Size) % 16U)) ;  /* 赋值或初始化 */

      /* Set the input and output addresses and start DMA transfer, pCrypOutBuffPtr size should be %4 */
      CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), ((uint16_t)wordsize / 4U),  /* 列表续行 */
                        (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unLocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Change the CRYP state and phase */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  AES CCM encryption/decryption processing in polling mode
  *         for TinyAES IP, no encrypt/decrypt performed, only authentication preparation.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Timeout: Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AESCCM_Process(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */
  uint32_t wordsize = (uint32_t)(hcryp->Size) / 4U;     /* 32位无符号变量/函数 */
  uint16_t outcount;  /* Temporary CrypOutCount Value */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */
#if defined(AES)                                        /* 条件编译 */
  uint32_t loopcounter;                                 /* 32位无符号变量/函数 */
  uint32_t npblb;                                       /* 32位无符号变量/函数 */
  uint32_t lastwordsize;                                /* 32位无符号变量/函数 */
#endif /* AES */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
      hcryp->SizesSum += hcryp->Size; /* Compute message total payload length */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
      hcryp->SizesSum = hcryp->Size; /* Merely store payload length */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hcryp->SizesSum = hcryp->Size;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */

    /*  Reset CrypHeaderCount */
    hcryp->CrypHeaderCount = 0U;                        /* 赋值或初始化 */

#if defined(CRYP)                                       /* 条件编译 */

    /********************** Init phase ******************************************/

    CRYP_SET_PHASE(hcryp, CRYP_PHASE_INIT);             /* 执行语句 */

    /* Set the key */
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

    /* Set the initialization vector (IV) with CTR1 information */
    hcryp->Instance->IV0LR = (hcryp->Init.B0[0]) & CRYP_CCM_CTR1_0;  /* 赋值或初始化 */
    hcryp->Instance->IV0RR = hcryp->Init.B0[1];         /* 赋值或初始化 */
    hcryp->Instance->IV1LR = hcryp->Init.B0[2];         /* 赋值或初始化 */
    hcryp->Instance->IV1RR = (hcryp->Init.B0[3] & CRYP_CCM_CTR1_1) |  CRYP_CCM_CTR1_2;  /* 赋值或初始化 */


    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /*Write  B0 packet into CRYP_DIN Register*/
    if (hcryp->Init.DataType == CRYP_DATATYPE_8B)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0 + 1));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0 + 2));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0 + 3));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_16B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0), 16);  /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0 + 1), 16);  /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0 + 2), 16);  /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0 + 3), 16);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_1B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0 + 1));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0 + 2));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0 + 3));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0);  /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0 + 1);  /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0 + 2);  /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0 + 3);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */

    /*Wait for the CRYPEN bit to be cleared*/
    while ((hcryp->Instance->CR & CRYP_CR_CRYPEN) == CRYP_CR_CRYPEN)  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
#else /* AES */
    /*AES2v1.1.1 : CCM authentication : no init phase, only header and final phase */
    /* Select header phase */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);           /* 执行语句 */

    /* configured encryption mode */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* Set the key */
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

    /* Set the initialization vector with zero values*/
    hcryp->Instance->IVR3 = 0U;                         /* 赋值或初始化 */
    hcryp->Instance->IVR2 = 0U;                         /* 赋值或初始化 */
    hcryp->Instance->IVR1 = 0U;                         /* 赋值或初始化 */
    hcryp->Instance->IVR0 = 0U;                         /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /*Write the B0 packet into CRYP_DIN*/
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0);  /* 赋值或初始化 */
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0 + 1);  /* 赋值或初始化 */
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0 + 2);  /* 赋值或初始化 */
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0 + 3);  /* 赋值或初始化 */

    /*  wait until the end of computation */
    if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Change state */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;       /* 赋值或初始化 */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked & return error */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */
    /* Clear CCF flag */
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);       /* HAL辅助宏/函数 */

    /* Set the phase */
    hcryp->Phase = CRYP_PHASE_PROCESS;                  /* 赋值或初始化 */

    /* From that point the whole message must be processed, first the Header then the payload.
    First the  Header block(B1) : associated data length expressed in bytes concatenated with Associated Data (A)*/

    if (hcryp->Init.HeaderSize != 0U)                   /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((hcryp->Init.HeaderSize % 4U) == 0U)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* HeaderSize %4, no padding */
        for (loopcounter = 0U; (loopcounter < hcryp->Init.HeaderSize); loopcounter += 4U)  /* for循环 */
        {                                               /* 代码块开始 */
          /* Write the Input block in the Data Input register */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */

          if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable the CRYP peripheral clock */
            __HAL_CRYP_DISABLE(hcryp);                  /* HAL辅助宏/函数 */

            /* Change state */
            hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
            hcryp->State = HAL_CRYP_STATE_READY;        /* 赋值或初始化 */

            /* Process unlocked */
            __HAL_UNLOCK(hcryp);                        /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
          /* Clear CCF Flag */
          __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);  /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /*Write Header block in the IN FIFO without last block */
        for (loopcounter = 0U; (loopcounter < ((hcryp->Init.HeaderSize) - (hcryp->Init.HeaderSize % 4U))); loopcounter += 4U)  /* for循环 */
        {                                               /* 代码块开始 */
          /* Write the input block in the data input register */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */

          if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable the CRYP peripheral clock */
            __HAL_CRYP_DISABLE(hcryp);                  /* HAL辅助宏/函数 */

            /* Change state */
            hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
            hcryp->State = HAL_CRYP_STATE_READY;        /* 赋值或初始化 */

            /* Process unlocked */
            __HAL_UNLOCK(hcryp);                        /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
          /* Clear CCF Flag */
          __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);  /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
        /*  Last block optionally pad the data with zeros*/
        for (loopcounter = 0U; (loopcounter < (hcryp->Init.HeaderSize % 4U)); loopcounter++)  /* for循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
        }                                               /* 代码块结束 */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          /* Pad the data with zeros to have a complete block */
          hcryp->Instance->DINR = 0x0U;                 /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */

        if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
        /* Clear CCF flag */
        __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);   /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  } /* if (DoKeyIVConfig == 1U) */
  /* Then the payload: cleartext payload (not the ciphertext payload).
  Write input Data, no output Data to get */
  if (hcryp->Size != 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hcryp->Size % 16U) != 0U)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* recalculate  wordsize */
      wordsize = ((wordsize / 4U) * 4U) ;               /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /* Get tick */
    tickstart = HAL_GetTick();                          /* 赋值或初始化 */
    /*Temporary CrypOutCount Value*/
    outcount = hcryp->CrypOutCount;                     /* 赋值或初始化 */

    while ((hcryp->CrypInCount < wordsize) && (outcount < wordsize))  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Write plain data and get cipher data */
      CRYP_AES_ProcessData(hcryp, Timeout);             /* 执行语句 */

      /*Temporary CrypOutCount Value*/
      outcount = hcryp->CrypOutCount;                   /* 赋值或初始化 */

      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)                     /* 条件判断 */
      {                                                 /* 代码块开始 */
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if ((hcryp->Size % 16U) != 0U)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Compute the number of padding bytes in last block of payload */
      npblb = ((((uint32_t)(hcryp->Size) / 16U) + 1U) * 16U) - (uint32_t)(hcryp->Size);  /* 赋值或初始化 */

      /* Number of valid words (lastwordsize) in last block */
      if ((npblb % 4U) == 0U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        lastwordsize = (16U - npblb) / 4U;              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        lastwordsize = ((16U - npblb) / 4U) + 1U;       /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      /*  Last block optionally pad the data with zeros*/
      for (loopcounter = 0U; loopcounter < lastwordsize; loopcounter ++)  /* for循环 */
      {                                                 /* 代码块开始 */
        /* Write the last input block in the IN FIFO */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
        hcryp->CrypInCount++;                           /* 执行语句 */
      }                                                 /* 代码块结束 */
      while (loopcounter < 4U)                          /* while循环 */
      {                                                 /* 代码块开始 */
        /* Pad the data with zeros to have a complete block */
        hcryp->Instance->DINR  = 0U;                    /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* Wait for CCF flag to be raised */
      if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Clear CCF flag */
      __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);     /* HAL辅助宏/函数 */

    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* End AES or CRYP */

#if defined(CRYP)                                       /* 条件编译 */

    /************************* Header phase *************************************/
    /* Header block(B1) : associated data length expressed in bytes concatenated
    with Associated Data (A)*/

    if (CRYP_GCMCCM_SetHeaderPhase(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /********************** Payload phase ***************************************/

    /* Set the phase */
    hcryp->Phase = CRYP_PHASE_PROCESS;                  /* 赋值或初始化 */

    /* Disable the CRYP peripheral */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

    /* Select payload phase once the header phase is performed */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_PAYLOAD);          /* 执行语句 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

  } /* if (DoKeyIVConfig == 1U) */

  if ((hcryp->Size % 16U) != 0U)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* recalculate  wordsize */
    wordsize = ((wordsize / 4U) * 4U) ;                 /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  /* Get tick */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */
  /*Temporary CrypOutCount Value*/
  outcount = hcryp->CrypOutCount;                       /* 赋值或初始化 */

  /* Write input data and get output data */
  while ((hcryp->CrypInCount < wordsize) && (outcount < wordsize))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Write plain data and get cipher data */
    CRYP_AES_ProcessData(hcryp, Timeout);               /* 执行语句 */

    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */

  if ((hcryp->Size % 16U) != 0U)                        /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* CRYP Workaround :  CRYP1 generates correct TAG  during CCM decryption only when ciphertext blocks size is multiple of
    128 bits. If lthe size of the last block of payload is inferior to 128 bits, when CCM decryption
    is selected, then the TAG message will be wrong.*/
    CRYP_Workaround(hcryp, Timeout);                    /* 执行语句 */
  }                                                     /* 代码块结束 */
#endif /* CRYP */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  AES CCM encryption/decryption process in interrupt mode
  *         for TinyAES IP, no encrypt/decrypt performed, only authentication preparation.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AESCCM_Process_IT(CRYP_HandleTypeDef *hcryp)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */
#if defined(CRYP)                                       /* 条件编译 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
#endif /* CRYP */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
      hcryp->SizesSum += hcryp->Size; /* Compute message total payload length */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
      hcryp->SizesSum = hcryp->Size; /* Merely store payload length */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hcryp->SizesSum = hcryp->Size;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /* Configure Key, IV and process message (header and payload) */
  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */
    /*  Reset CrypHeaderCount */
    hcryp->CrypHeaderCount = 0U;                        /* 赋值或初始化 */

#if defined(CRYP)                                       /* 条件编译 */

    /************ Init phase ************/

    CRYP_SET_PHASE(hcryp, CRYP_PHASE_INIT);             /* 执行语句 */

    /* Set the key */
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

    /* Set the initialization vector (IV) with CTR1 information */
    hcryp->Instance->IV0LR = (hcryp->Init.B0[0]) & CRYP_CCM_CTR1_0;  /* 赋值或初始化 */
    hcryp->Instance->IV0RR = hcryp->Init.B0[1];         /* 赋值或初始化 */
    hcryp->Instance->IV1LR = hcryp->Init.B0[2];         /* 赋值或初始化 */
    hcryp->Instance->IV1RR = (hcryp->Init.B0[3] & CRYP_CCM_CTR1_1) |  CRYP_CCM_CTR1_2;  /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /*Write the B0 packet into CRYP_DIN Register*/
    if (hcryp->Init.DataType == CRYP_DATATYPE_8B)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0 + 1));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0 + 2));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0 + 3));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_16B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0), 16);  /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0 + 1), 16);  /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0 + 2), 16);  /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0 + 3), 16);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_1B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0 + 1));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0 + 2));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0 + 3));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0);  /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0 + 1);  /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0 + 2);  /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0 + 3);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    /*Wait for the CRYPEN bit to be cleared*/
    count = CRYP_TIMEOUT_GCMCCMINITPHASE;               /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count-- ;                                         /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    } while ((hcryp->Instance->CR & CRYP_CR_CRYPEN) == CRYP_CR_CRYPEN);  /* 代码块结束 */

    /* Select header phase */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);           /* 执行语句 */

  } /* end of if (DoKeyIVConfig == 1U) */

  /* Enable interrupts */
  __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_INI);             /* HAL辅助宏/函数 */

  /* Enable CRYP */
  __HAL_CRYP_ENABLE(hcryp);                             /* HAL辅助宏/函数 */

#else /* AES */

    /*AES2v1.1.1 : CCM authentication : no init phase, only header and final phase */
    /* Select header phase */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);           /* 执行语句 */

    /* configured mode and encryption mode */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* Set the key */
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

    /* Set the initialization vector with zero values*/
    hcryp->Instance->IVR3 = 0U;                         /* 赋值或初始化 */
    hcryp->Instance->IVR2 = 0U;                         /* 赋值或初始化 */
    hcryp->Instance->IVR1 = 0U;                         /* 赋值或初始化 */
    hcryp->Instance->IVR0 = 0U;                         /* 赋值或初始化 */

    /* Enable interrupts */
    __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_CCFIE | CRYP_IT_ERRIE);  /* HAL辅助宏/函数 */
    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /*Write the B0 packet into CRYP_DIN*/
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0);  /* 赋值或初始化 */
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0 + 1);  /* 赋值或初始化 */
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0 + 2);  /* 赋值或初始化 */
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0 + 3);  /* 赋值或初始化 */

  } /* end of if (DoKeyIVConfig == 1U) */
#endif /* End AES or CRYP */

  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
/**
  * @brief  AES CCM encryption/decryption process in DMA mode
  *         for TinyAES IP, no encrypt/decrypt performed, only authentication preparation.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_AESCCM_Process_DMA(CRYP_HandleTypeDef *hcryp)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t wordsize;                                    /* 32位无符号变量/函数 */
  __IO uint32_t count = 0U;                             /* 赋值或初始化 */
  uint32_t DoKeyIVConfig = 1U; /* By default, carry out peripheral Key and IV configuration */
#if defined(AES)                                        /* 条件编译 */
  uint32_t loopcounter;                                 /* 32位无符号变量/函数 */
  uint32_t npblb;                                       /* 32位无符号变量/函数 */
  uint32_t lastwordsize;                                /* 32位无符号变量/函数 */
#endif                                                  /* 条件编译结束 */

  if (hcryp->Init.KeyIVConfigSkip == CRYP_KEYIVCONFIG_ONCE)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->KeyIVConfig == 1U)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has already been done, skip it */
      DoKeyIVConfig = 0U;                               /* 赋值或初始化 */
      hcryp->SizesSum += hcryp->Size; /* Compute message total payload length */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* If the Key and IV configuration has to be done only once
      and if it has not been done already, do it and set KeyIVConfig
      to keep track it won't have to be done again next time */
      hcryp->KeyIVConfig = 1U;                          /* 赋值或初始化 */
      hcryp->SizesSum = hcryp->Size; /* Merely store payload length */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    hcryp->SizesSum = hcryp->Size;                      /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (DoKeyIVConfig == 1U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */

    /*  Reset CrypHeaderCount */
    hcryp->CrypHeaderCount = 0U;                        /* 赋值或初始化 */

#if defined(CRYP)                                       /* 条件编译 */

    /************************** Init phase **************************************/

    CRYP_SET_PHASE(hcryp, CRYP_PHASE_INIT);             /* 执行语句 */

    /* Set the key */
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

    /* Set the initialization vector (IV) with CTR1 information */
    hcryp->Instance->IV0LR = (hcryp->Init.B0[0]) & CRYP_CCM_CTR1_0;  /* 赋值或初始化 */
    hcryp->Instance->IV0RR = hcryp->Init.B0[1];         /* 赋值或初始化 */
    hcryp->Instance->IV1LR = hcryp->Init.B0[2];         /* 赋值或初始化 */
    hcryp->Instance->IV1RR = (hcryp->Init.B0[3] & CRYP_CCM_CTR1_1) |  CRYP_CCM_CTR1_2;  /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /*Write the B0 packet into CRYP_DIN Register*/
    if (hcryp->Init.DataType == CRYP_DATATYPE_8B)       /* 条件判断 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0 + 1));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0 + 2));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __REV(*(uint32_t *)(hcryp->Init.B0 + 3));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_16B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0), 16);  /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0 + 1), 16);  /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0 + 2), 16);  /* 赋值或初始化 */
      hcryp->Instance->DIN = __ROR(*(uint32_t *)(hcryp->Init.B0 + 3), 16);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Init.DataType == CRYP_DATATYPE_1B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0 + 1));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0 + 2));  /* 赋值或初始化 */
      hcryp->Instance->DIN = __RBIT(*(uint32_t *)(hcryp->Init.B0 + 3));  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0);  /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0 + 1);  /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0 + 2);  /* 赋值或初始化 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.B0 + 3);  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /*Wait for the CRYPEN bit to be cleared*/
    count = CRYP_TIMEOUT_GCMCCMINITPHASE;               /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count-- ;                                         /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    } while ((hcryp->Instance->CR & CRYP_CR_CRYPEN) == CRYP_CR_CRYPEN);  /* 代码块结束 */

#else /* AES */

    /*AES2v1.1.1 : CCM authentication : no init phase, only header and final phase */
    /* Select header phase */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);           /* 执行语句 */

    /* configured encryption mode */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_MODE, CRYP_OPERATINGMODE_ENCRYPT);  /* 执行语句 */

    /* Set the key */
    CRYP_SetKey(hcryp, hcryp->Init.KeySize);            /* 执行语句 */

    /* Set the initialization vector with zero values*/
    hcryp->Instance->IVR3 = 0U;                         /* 赋值或初始化 */
    hcryp->Instance->IVR2 = 0U;                         /* 赋值或初始化 */
    hcryp->Instance->IVR1 = 0U;                         /* 赋值或初始化 */
    hcryp->Instance->IVR0 = 0U;                         /* 赋值或初始化 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    /*Write the B0 packet into CRYP_DIN*/
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0);  /* 赋值或初始化 */
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0 + 1);  /* 赋值或初始化 */
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0 + 2);  /* 赋值或初始化 */
    hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.B0 + 3);  /* 赋值或初始化 */

    count = CRYP_TIMEOUT_GCMCCMINITPHASE;               /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count-- ;                                         /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */
    /* Clear CCF flag */
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);       /* HAL辅助宏/函数 */

    /* Set the phase */
    hcryp->Phase = CRYP_PHASE_PROCESS;                  /* 赋值或初始化 */

    /* From that point the whole message must be processed, first the Header then the payload.
    First the  Header block(B1) : associated data length expressed in bytes concatenated with Associated Data (A)*/

    if (hcryp->Init.HeaderSize != 0U)                   /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((hcryp->Init.HeaderSize % 4U) == 0U)          /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* HeaderSize %4, no padding */
        for (loopcounter = 0U; (loopcounter < hcryp->Init.HeaderSize); loopcounter += 4U)  /* for循环 */
        {                                               /* 代码块开始 */
          /* Write the Input block in the Data Input register */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */

          /*  wait until the end of computation */
          count = CRYP_TIMEOUT_GCMCCMINITPHASE;         /* 赋值或初始化 */
          do                                            /* do-while循环 */
          {                                             /* 代码块开始 */
            count-- ;                                   /* 执行语句 */
            if (count == 0U)                            /* 条件判断 */
            {                                           /* 代码块开始 */
              /* Disable the CRYP peripheral clock */
              __HAL_CRYP_DISABLE(hcryp);                /* HAL辅助宏/函数 */

              /* Change state */
              hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
              hcryp->State = HAL_CRYP_STATE_READY;      /* 赋值或初始化 */

              /* Process Unlocked */
              __HAL_UNLOCK(hcryp);                      /* HAL辅助宏/函数 */
              return HAL_ERROR;                         /* 返回值 */
            }                                           /* 代码块结束 */
          } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */
          /* Clear CCF flag */
          __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);  /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /*Write Header block in the IN FIFO without last block */
        for (loopcounter = 0U; (loopcounter < ((hcryp->Init.HeaderSize) - (hcryp->Init.HeaderSize % 4U))); loopcounter += 4U)  /* for循环 */
        {                                               /* 代码块开始 */
          /* Write the input block in the data input register */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */

          count = CRYP_TIMEOUT_GCMCCMINITPHASE;         /* 赋值或初始化 */
          do                                            /* do-while循环 */
          {                                             /* 代码块开始 */
            count-- ;                                   /* 执行语句 */
            if (count == 0U)                            /* 条件判断 */
            {                                           /* 代码块开始 */
              /* Disable the CRYP peripheral clock */
              __HAL_CRYP_DISABLE(hcryp);                /* HAL辅助宏/函数 */

              /* Change state */
              hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
              hcryp->State = HAL_CRYP_STATE_READY;      /* 赋值或初始化 */

              /* Process Unlocked */
              __HAL_UNLOCK(hcryp);                      /* HAL辅助宏/函数 */
              return HAL_ERROR;                         /* 返回值 */
            }                                           /* 代码块结束 */
          } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */
          /* Clear CCF flag */
          __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);  /* HAL辅助宏/函数 */
        }                                               /* 代码块结束 */
        /*  Last block optionally pad the data with zeros*/
        for (loopcounter = 0U; (loopcounter < (hcryp->Init.HeaderSize % 4U)); loopcounter++)  /* for循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
          hcryp->CrypHeaderCount++ ;                    /* 执行语句 */
        }                                               /* 代码块结束 */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          /* Pad the data with zeros to have a complete block */
          hcryp->Instance->DINR = 0x0U;                 /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */

        count = CRYP_TIMEOUT_GCMCCMINITPHASE;           /* 赋值或初始化 */
        do                                              /* do-while循环 */
        {                                               /* 代码块开始 */
          count-- ;                                     /* 执行语句 */
          if (count == 0U)                              /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable the CRYP peripheral clock */
            __HAL_CRYP_DISABLE(hcryp);                  /* HAL辅助宏/函数 */

            /* Change state */
            hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
            hcryp->State = HAL_CRYP_STATE_READY;        /* 赋值或初始化 */

            /* Process Unlocked */
            __HAL_UNLOCK(hcryp);                        /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */
        /* Clear CCF flag */
        __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);   /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  } /* if (DoKeyIVConfig == 1U) */
  /* Then the payload: cleartext payload (not the ciphertext payload).
  Write input Data, no output Data to get */
  if (hcryp->Size != 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    if (hcryp->Size >= 16U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((hcryp->Size % 16U) == 0U)                    /* 条件判断 */
      {                                                 /* 代码块开始 */
        CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), (hcryp->Size / 4U), (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
      }                                                 /* 代码块结束 */
      else /*to compute last word<128bits, otherwise it will not be encrypted/decrypted */
      {                                                 /* 代码块开始 */
        wordsize = (uint32_t)(hcryp->Size) + (16U - ((uint32_t)(hcryp->Size) % 16U)) ;  /* 赋值或初始化 */

        /* Set the input and output addresses and start DMA transfer, pCrypOutBuffPtr size should be %4 */
        CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), ((uint16_t)wordsize / 4U),  /* 列表续行 */
                          (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    if ((hcryp->Size < 16U) != 0U)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Compute the number of padding bytes in last block of payload */
      npblb = ((((uint32_t)(hcryp->Size) / 16U) + 1U) * 16U) - (uint32_t)(hcryp->Size);  /* 赋值或初始化 */

      /* Number of valid words (lastwordsize) in last block */
      if ((npblb % 4U) == 0U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        lastwordsize = (16U - npblb) / 4U;              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        lastwordsize = ((16U - npblb) / 4U) + 1U;       /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      /*  Last block optionally pad the data with zeros*/
      for (loopcounter = 0U; loopcounter < lastwordsize; loopcounter ++)  /* for循环 */
      {                                                 /* 代码块开始 */
        /* Write the last input block in the IN FIFO */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
        hcryp->CrypInCount++;                           /* 执行语句 */
      }                                                 /* 代码块结束 */
      while (loopcounter < 4U)                          /* while循环 */
      {                                                 /* 代码块开始 */
        /* Pad the data with zeros to have a complete block */
        hcryp->Instance->DINR  = 0U;                    /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
      }                                                 /* 代码块结束 */
      count = CRYP_TIMEOUT_GCMCCMINITPHASE;             /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        count-- ;                                       /* 执行语句 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process Unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */
      /* Clear CCF flag */
      __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);     /* HAL辅助宏/函数 */

      /* Process unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */

      /* Change the CRYP state and phase */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Process unLocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Change the CRYP state and phase */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* AES */
#if defined(CRYP)                                       /* 条件编译 */
    /********************* Header phase *****************************************/

    if (CRYP_GCMCCM_SetHeaderPhase_DMA(hcryp) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

    /******************** Payload phase *****************************************/

    /* Set the phase */
    hcryp->Phase = CRYP_PHASE_PROCESS;                  /* 赋值或初始化 */

    /* Disable the CRYP peripheral */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

    /* Select payload phase once the header phase is performed */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_PAYLOAD);          /* 执行语句 */

  } /* if (DoKeyIVConfig == 1U) */
  if (hcryp->Size != 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Size should be %4  otherwise Tag will  be incorrectly generated for GCM Encryption & CCM Decryption
    Workaround is implemented in polling mode, so if last block of
    payload <128bit don't use HAL_CRYP_AESGCM_DMA otherwise TAG is incorrectly generated for GCM Encryption. */
    /* Set the input and output addresses and start DMA transfer */
    if ((hcryp->Size % 16U) == 0U)                      /* 条件判断 */
    {                                                   /* 代码块开始 */
      CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), hcryp->Size / 4U, (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      wordsize = (uint32_t)(hcryp->Size) + 16U - ((uint32_t)(hcryp->Size) % 16U) ;  /* 赋值或初始化 */

      /* Set the input and output addresses and start DMA transfer, pCrypOutBuffPtr size should be %4*/
      CRYP_SetDMAConfig(hcryp, (uint32_t)(hcryp->pCrypInBuffPtr), (uint16_t)wordsize / 4U,  /* 列表续行 */
                        (uint32_t)(hcryp->pCrypOutBuffPtr));  /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else /*Size = 0*/
  {                                                     /* 代码块开始 */
    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Change the CRYP state and phase */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
#endif /* CRYP */
  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets the payload phase in iterrupt mode
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @retval state
  */
static void CRYP_GCMCCM_SetPayloadPhase_IT(CRYP_HandleTypeDef *hcryp)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t loopcounter;                                 /* 32位无符号变量/函数 */
  uint32_t temp[4];  /* Temporary CrypOutBuff */
  uint32_t lastwordsize;                                /* 32位无符号变量/函数 */
  uint32_t npblb;                                       /* 32位无符号变量/函数 */
  uint32_t i;                                           /* 32位无符号变量/函数 */
#if defined(AES)                                        /* 条件编译 */
  uint16_t outcount;  /* Temporary CrypOutCount Value */
#endif /* AES */

  /***************************** Payload phase *******************************/

#if defined(CRYP)                                       /* 条件编译 */
  if (hcryp->Size == 0U)                                /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable interrupts */
    __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_INI | CRYP_IT_OUTI);  /* HAL辅助宏/函数 */

    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Change the CRYP state */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  else if (((hcryp->Size / 4U) - (hcryp->CrypInCount)) >= 4U)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Write the input block in the IN FIFO */
    hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    if (((hcryp->Size / 4U) == hcryp->CrypInCount) && ((hcryp->Size % 16U) == 0U))  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable interrupts */
      __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_INI);        /* HAL辅助宏/函数 */

      /* Call the input data transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)             /* 条件编译 */
      /*Call registered Input complete callback*/
      hcryp->InCpltCallback(hcryp);                     /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call legacy weak Input complete callback*/
      HAL_CRYP_InCpltCallback(hcryp);                   /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    if (hcryp->CrypOutCount < (hcryp->Size / 4U))       /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Read the output block from the Output FIFO and put them in temporary buffer then get CrypOutBuff from temporary buffer  */
      for (i = 0U; i < 4U; i++)                         /* for循环 */
      {                                                 /* 代码块开始 */
        temp[i] = hcryp->Instance->DOUT;                /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      i = 0U;                                           /* 赋值或初始化 */
      while (((hcryp->CrypOutCount < ((hcryp->Size) / 4U))) && (i < 4U))  /* while循环 */
      {                                                 /* 代码块开始 */
        *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
        hcryp->CrypOutCount++;                          /* 执行语句 */
        i++;                                            /* 执行语句 */
      }                                                 /* 代码块结束 */
      if (((hcryp->Size / 4U) == hcryp->CrypOutCount) && ((hcryp->Size % 16U) == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable interrupts */
        __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_OUTI);     /* HAL辅助宏/函数 */

        /* Change the CRYP state */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Disable CRYP */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */

        /* Call output transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered Output complete callback*/
        hcryp->OutCpltCallback(hcryp);                  /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak Output complete callback*/
        HAL_CRYP_OutCpltCallback(hcryp);                /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if ((hcryp->Size % 16U) != 0U)                   /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Size should be %4 in word and %16 in byte  otherwise TAG will  be incorrectly generated for GCM Encryption & CCM Decryption
    Workaround is implemented in polling mode, so if last block of
    payload <128bit don't use CRYP_AESGCM_Encrypt_IT otherwise TAG is incorrectly generated. */

    /* Compute the number of padding bytes in last block of payload */
    npblb = ((((uint32_t)(hcryp->Size) / 16U) + 1U) * 16U) - (uint32_t)(hcryp->Size);  /* 赋值或初始化 */

    /* Number of valid words (lastwordsize) in last block */
    if ((npblb % 4U) == 0U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      lastwordsize = (16U - npblb) / 4U;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      lastwordsize = ((16U - npblb) / 4U) + 1U;         /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /*  Last block optionally pad the data with zeros*/
    for (loopcounter = 0U; loopcounter < lastwordsize; loopcounter++)  /* for循环 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
    }                                                   /* 代码块结束 */
    while (loopcounter < 4U)                            /* while循环 */
    {                                                   /* 代码块开始 */
      /* Pad the data with zeros to have a complete block */
      hcryp->Instance->DIN = 0x0U;                      /* 赋值或初始化 */
      loopcounter++;                                    /* 执行语句 */
    }                                                   /* 代码块结束 */
    __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_INI);          /* HAL辅助宏/函数 */

    if ((hcryp->Instance->SR & CRYP_FLAG_OFNE) != 0x0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      for (i = 0U; i < 4U; i++)                         /* for循环 */
      {                                                 /* 代码块开始 */
        temp[i] = hcryp->Instance->DOUT;                /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if (((hcryp->Size) / 4U) == 0U)                   /* 条件判断 */
      {                                                 /* 代码块开始 */
        for (i = 0U; i < ((uint32_t)(hcryp->Size) % 4U); i++)  /* for循环 */
        {                                               /* 代码块开始 */
          *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
          hcryp->CrypOutCount++;                        /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      i = 0x0U;                                         /* 赋值或初始化 */
      while (((hcryp->CrypOutCount < ((hcryp->Size) / 4U))) && (i < 4U))  /* while循环 */
      {                                                 /* 代码块开始 */
        *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
        hcryp->CrypOutCount++;                          /* 执行语句 */
        i++;                                            /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    if (hcryp->CrypOutCount >= (hcryp->Size / 4U))      /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable interrupts */
      __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_OUTI | CRYP_IT_INI);  /* HAL辅助宏/函数 */

      /* Change the CRYP peripheral state */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */

      /* Call output transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /*Call registered Output complete callback*/
      hcryp->OutCpltCallback(hcryp);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call legacy weak Output complete callback*/
      HAL_CRYP_OutCpltCallback(hcryp);                  /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Nothing to do */
  }                                                     /* 代码块结束 */
#else /* AES */

  /* Read the output block from the output FIFO and put them in temporary buffer then get CrypOutBuff from temporary buffer*/
  for (i = 0U; i < 4U; i++)                             /* for循环 */
  {                                                     /* 代码块开始 */
    temp[i] = hcryp->Instance->DOUTR;                   /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  i = 0U;                                               /* 赋值或初始化 */
  while ((hcryp->CrypOutCount < ((hcryp->Size + 3U) / 4U)) && (i < 4U))  /* while循环 */
  {                                                     /* 代码块开始 */
    *(uint32_t *)(hcryp->pCrypOutBuffPtr + hcryp->CrypOutCount) = temp[i];  /* 赋值或初始化 */
    hcryp->CrypOutCount++;                              /* 执行语句 */
    i++;                                                /* 执行语句 */
  }                                                     /* 代码块结束 */
  /*Temporary CrypOutCount Value*/
  outcount = hcryp->CrypOutCount;                       /* 赋值或初始化 */

  if ((hcryp->CrypOutCount >= (hcryp->Size / 4U)) && ((outcount * 4U) >=  hcryp->Size))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable computation complete flag and errors interrupts */
    __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_CCFIE | CRYP_IT_ERRIE);  /* HAL辅助宏/函数 */

    /* Change the CRYP state */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */

    /* Process unlocked */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */

    /* Call output transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
    /*Call registered Output complete callback*/
    hcryp->OutCpltCallback(hcryp);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak Output complete callback*/
    HAL_CRYP_OutCpltCallback(hcryp);                    /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  else if (((hcryp->Size / 4U) - (hcryp->CrypInCount)) >= 4U)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Write the input block in the IN FIFO */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
    if ((hcryp->CrypInCount == (hcryp->Size / 4U)) && ((hcryp->Size % 16U) == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call Input transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered Input complete callback*/
        hcryp->InCpltCallback(hcryp);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak Input complete callback*/
        HAL_CRYP_InCpltCallback(hcryp);                 /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else /* Last block of payload < 128bit*/
  {                                                     /* 代码块开始 */
    /* Workaround not implemented, Size should be %4  otherwise Tag will  be incorrectly
    generated for GCM Encryption & CCM Decryption. Workaround is implemented in polling mode, so if last block of
    payload <128bit don't use CRYP_Encrypt_IT otherwise TAG is incorrectly generated for GCM Encryption & CCM Decryption. */

    /* Compute the number of padding bytes in last block of payload */
    npblb = ((((uint32_t)(hcryp->Size) / 16U) + 1U) * 16U) - (uint32_t)(hcryp->Size);  /* 赋值或初始化 */

    /* Number of valid words (lastwordsize) in last block */
    if ((npblb % 4U) == 0U)                             /* 条件判断 */
    {                                                   /* 代码块开始 */
      lastwordsize = (16U - npblb) / 4U;                /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      lastwordsize = ((16U - npblb) / 4U) + 1U;         /* 赋值或初始化 */
    }                                                   /* 代码块结束 */

    /*  Last block optionally pad the data with zeros*/
    for (loopcounter = 0U; loopcounter < lastwordsize; loopcounter++)  /* for循环 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DINR = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
    }                                                   /* 代码块结束 */
    while (loopcounter < 4U)                            /* while循环 */
    {                                                   /* 代码块开始 */
      /* pad the data with zeros to have a complete block */
      hcryp->Instance->DINR = 0x0U;                     /* 赋值或初始化 */
      loopcounter++;                                    /* 执行语句 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* AES */

}                                                       /* 代码块结束 */


/**
  * @brief  Sets the header phase in polling mode
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module(Header & HeaderSize)
  * @param  Timeout: Timeout value
  * @retval state
  */
static HAL_StatusTypeDef CRYP_GCMCCM_SetHeaderPhase(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t loopcounter;                                 /* 32位无符号变量/函数 */
  uint32_t size_in_bytes;                               /* 32位无符号变量/函数 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t mask[12] = {0x0U, 0xFF000000U, 0xFFFF0000U, 0xFFFFFF00U,  /* 32-bit data type */
                       0x0U, 0x0000FF00U, 0x0000FFFFU, 0xFF00FFFFU,  /* 16-bit data type */
                       0x0U, 0x000000FFU, 0x0000FFFFU, 0x00FFFFFFU}; /*  8-bit data type */

  /***************************** Header phase for GCM/GMAC or CCM *********************************/

  if (hcryp->Init.HeaderWidthUnit == CRYP_HEADERWIDTHUNIT_WORD)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    size_in_bytes = hcryp->Init.HeaderSize * 4U;        /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    size_in_bytes = hcryp->Init.HeaderSize;             /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (size_in_bytes != 0U)                              /* 条件判断 */
  {                                                     /* 代码块开始 */

#if defined(CRYP)                                       /* 条件编译 */

    /* Select header phase */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);           /* 执行语句 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    if ((size_in_bytes % 16U) == 0U)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* HeaderSize %4, no padding */
      for (loopcounter = 0U; (loopcounter < (size_in_bytes / 4U)); loopcounter += 4U)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */

        /* Wait for IFEM to be raised */
        if (CRYP_WaitOnIFEMFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*Write header block in the IN FIFO without last block */
      for (loopcounter = 0U; (loopcounter < ((size_in_bytes / 16U) * 4U)); loopcounter += 4U)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */

        /* Wait for IFEM to be raised */
        if (CRYP_WaitOnIFEMFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      /*  Last block optionally pad the data with zeros*/
      for (loopcounter = 0U; (loopcounter < ((size_in_bytes / 4U) % 4U)); loopcounter++)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* If the header size is a multiple of words */
      if ((size_in_bytes % 4U) == 0U)                   /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Pad the data with zeros to have a complete block */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DIN = 0x0U;                  /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Enter last bytes, padded with zeroes */
        tmp =  *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        tmp &= mask[(hcryp->Init.DataType * 2U) + (size_in_bytes % 4U)];  /* 赋值或初始化 */
        hcryp->Instance->DIN = tmp;                     /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
        /* Pad the data with zeros to have a complete block */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DIN = 0x0U;                  /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      /* Wait for CCF IFEM to be raised */
      if (CRYP_WaitOnIFEMFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Wait until the complete message has been processed */
    if (CRYP_WaitOnBUSYFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the CRYP peripheral clock */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      /* Change state */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;       /* 赋值或初始化 */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked & return error */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
      return HAL_ERROR;                                 /* 返回值 */
    }                                                   /* 代码块结束 */

#else /* AES */

    if (hcryp->Init.Algorithm == CRYP_AES_GCM_GMAC)     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Workaround 1 :only AES before re-enabling the IP, datatype can be configured.*/
      MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE, hcryp->Init.DataType);  /* 执行语句 */

      /* Select header phase */
      CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);         /* 执行语句 */

      /* Enable the CRYP peripheral */
      __HAL_CRYP_ENABLE(hcryp);                         /* HAL辅助宏/函数 */

    }                                                   /* 代码块结束 */
    /* If size_in_bytes is a multiple of blocks (a multiple of four 32-bits words ) */
    if ((size_in_bytes % 16U) == 0U)                    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* No padding */
      for (loopcounter = 0U; (loopcounter < (size_in_bytes / 4U)); loopcounter += 4U)  /* for循环 */
      {                                                 /* 代码块开始 */
        /* Write the input block in the data input register */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */

        if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
        /* Clear CCF flag */
        __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);   /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*Write header block in the IN FIFO without last block */
      for (loopcounter = 0U; (loopcounter < ((size_in_bytes / 16U) * 4U)); loopcounter += 4U)  /* for循环 */
      {                                                 /* 代码块开始 */
        /* Write the input block in the data input register */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */

        if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
        /* Clear CCF flag */
        __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);   /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      /* Write last complete words */
      for (loopcounter = 0U; (loopcounter < ((size_in_bytes / 4U) % 4U)); loopcounter++)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* If the header size is a multiple of words */
      if ((size_in_bytes % 4U) == 0U)                   /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Pad the data with zeros to have a complete block */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DINR = 0x0U;                 /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Enter last bytes, padded with zeroes */
        tmp =  *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        tmp &= mask[(hcryp->Init.DataType * 2U) + (size_in_bytes % 4U)];  /* 赋值或初始化 */
        hcryp->Instance->DINR = tmp;                    /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
        /* Pad the data with zeros to have a complete block */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DINR = 0x0U;                 /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */

      if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
      /* Clear CCF flag */
      __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);     /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
#endif /* End AES or CRYP */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
#if defined(AES)                                        /* 条件编译 */
    if (hcryp->Init.Algorithm == CRYP_AES_GCM_GMAC)     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /*Workaround 1: only AES, before re-enabling the IP, datatype can be configured.*/
      MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE, hcryp->Init.DataType);  /* 执行语句 */

      /* Select header phase */
      CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);         /* 执行语句 */

      /* Enable the CRYP peripheral */
      __HAL_CRYP_ENABLE(hcryp);                         /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
#endif /* AES */
  }                                                     /* 代码块结束 */
  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets the header phase when using DMA in process
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module(Header & HeaderSize)
  * @retval None
  */
static HAL_StatusTypeDef CRYP_GCMCCM_SetHeaderPhase_DMA(CRYP_HandleTypeDef *hcryp)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  __IO uint32_t count  = 0U;                            /* 赋值或初始化 */
  uint32_t loopcounter;                                 /* 32位无符号变量/函数 */
  uint32_t headersize_in_bytes;                         /* 32位无符号变量/函数 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t mask[12] = {0x0U, 0xFF000000U, 0xFFFF0000U, 0xFFFFFF00U,  /* 32-bit data type */
                       0x0U, 0x0000FF00U, 0x0000FFFFU, 0xFF00FFFFU,  /* 16-bit data type */
                       0x0U, 0x000000FFU, 0x0000FFFFU, 0x00FFFFFFU}; /*  8-bit data type */

  /***************************** Header phase for GCM/GMAC or CCM *********************************/
  if (hcryp->Init.HeaderWidthUnit == CRYP_HEADERWIDTHUNIT_WORD)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    headersize_in_bytes = hcryp->Init.HeaderSize * 4U;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    headersize_in_bytes = hcryp->Init.HeaderSize;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  if (headersize_in_bytes != 0U)                        /* 条件判断 */
  {                                                     /* 代码块开始 */

#if defined(CRYP)                                       /* 条件编译 */

    /* Select header phase */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);           /* 执行语句 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */

    if ((headersize_in_bytes % 16U) == 0U)              /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* HeaderSize %4, no padding */
      for (loopcounter = 0U; (loopcounter < (headersize_in_bytes / 4U)); loopcounter += 4U)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */

        /* Wait for IFEM to be raised */
        count = CRYP_TIMEOUT_GCMCCMHEADERPHASE;         /* 赋值或初始化 */
        do                                              /* do-while循环 */
        {                                               /* 代码块开始 */
          count-- ;                                     /* 执行语句 */
          if (count == 0U)                              /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable the CRYP peripheral clock */
            __HAL_CRYP_DISABLE(hcryp);                  /* HAL辅助宏/函数 */

            /* Change state */
            hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
            hcryp->State = HAL_CRYP_STATE_READY;        /* 赋值或初始化 */

            /* Process unlocked */
            __HAL_UNLOCK(hcryp);                        /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, CRYP_FLAG_IFEM));  /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*Write header block in the IN FIFO without last block */
      for (loopcounter = 0U; (loopcounter < ((headersize_in_bytes / 16U) * 4U)); loopcounter += 4U)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */

        /* Wait for IFEM to be raised */
        count = CRYP_TIMEOUT_GCMCCMHEADERPHASE;         /* 赋值或初始化 */
        do                                              /* do-while循环 */
        {                                               /* 代码块开始 */
          count-- ;                                     /* 执行语句 */
          if (count == 0U)                              /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable the CRYP peripheral clock */
            __HAL_CRYP_DISABLE(hcryp);                  /* HAL辅助宏/函数 */

            /* Change state */
            hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
            hcryp->State = HAL_CRYP_STATE_READY;        /* 赋值或初始化 */

            /* Process unlocked */
            __HAL_UNLOCK(hcryp);                        /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, CRYP_FLAG_IFEM));  /* 代码块结束 */
      }                                                 /* 代码块结束 */
      /*  Last block optionally pad the data with zeros*/
      for (loopcounter = 0U; (loopcounter < ((headersize_in_bytes / 4U) % 4U)); loopcounter++)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* If the header size is a multiple of words */
      if ((headersize_in_bytes % 4U) == 0U)             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Pad the data with zeros to have a complete block */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DIN = 0x0U;                  /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Enter last bytes, padded with zeroes */
        tmp =  *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        tmp &= mask[(hcryp->Init.DataType * 2U) + (headersize_in_bytes % 4U)];  /* 赋值或初始化 */
        hcryp->Instance->DIN = tmp;                     /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
        /* Pad the data with zeros to have a complete block */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DIN = 0x0U;                  /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      /* Wait for IFEM to be raised */
      count = CRYP_TIMEOUT_GCMCCMHEADERPHASE;           /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        count-- ;                                       /* 执行语句 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, CRYP_FLAG_IFEM));  /* 代码块结束 */
    }                                                   /* 代码块结束 */
    /* Wait until the complete message has been processed */
    count = CRYP_TIMEOUT_GCMCCMHEADERPHASE;             /* 赋值或初始化 */
    do                                                  /* do-while循环 */
    {                                                   /* 代码块开始 */
      count-- ;                                         /* 执行语句 */
      if (count == 0U)                                  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    } while (HAL_IS_BIT_SET(hcryp->Instance->SR, CRYP_FLAG_BUSY));  /* 代码块结束 */

#else /* AES */

    if (hcryp->Init.Algorithm == CRYP_AES_GCM_GMAC)     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Workaround 1: only AES, before re-enabling the IP, datatype can be configured.*/
      MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE, hcryp->Init.DataType);  /* 执行语句 */

      /* Select header phase */
      CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);         /* 执行语句 */

      /* Enable the CRYP peripheral */
      __HAL_CRYP_ENABLE(hcryp);                         /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    if ((headersize_in_bytes % 16U) == 0U)              /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* HeaderSize %4, no padding */
      for (loopcounter = 0U; (loopcounter < (headersize_in_bytes / 4U)); loopcounter += 4U)  /* for循环 */
      {                                                 /* 代码块开始 */
        /* Write the input block in the data input register */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */

        /*Wait on CCF flag*/
        count = CRYP_TIMEOUT_GCMCCMHEADERPHASE;         /* 赋值或初始化 */
        do                                              /* do-while循环 */
        {                                               /* 代码块开始 */
          count-- ;                                     /* 执行语句 */
          if (count == 0U)                              /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable the CRYP peripheral clock */
            __HAL_CRYP_DISABLE(hcryp);                  /* HAL辅助宏/函数 */

            /* Change state */
            hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
            hcryp->State = HAL_CRYP_STATE_READY;        /* 赋值或初始化 */

            /* Process unlocked */
            __HAL_UNLOCK(hcryp);                        /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */

        /* Clear CCF flag */
        __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);   /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /*Write header block in the IN FIFO without last block */
      for (loopcounter = 0U; (loopcounter < ((headersize_in_bytes / 16U) * 4U)); loopcounter += 4U)  /* for循环 */
      {                                                 /* 代码块开始 */
        /* Write the Input block in the Data Input register */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */

        /*Wait on CCF flag*/
        count = CRYP_TIMEOUT_GCMCCMHEADERPHASE;         /* 赋值或初始化 */
        do                                              /* do-while循环 */
        {                                               /* 代码块开始 */
          count-- ;                                     /* 执行语句 */
          if (count == 0U)                              /* 条件判断 */
          {                                             /* 代码块开始 */
            /* Disable the CRYP peripheral clock */
            __HAL_CRYP_DISABLE(hcryp);                  /* HAL辅助宏/函数 */

            /* Change state */
            hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;  /* 赋值或初始化 */
            hcryp->State = HAL_CRYP_STATE_READY;        /* 赋值或初始化 */

            /* Process unlocked */
            __HAL_UNLOCK(hcryp);                        /* HAL辅助宏/函数 */
            return HAL_ERROR;                           /* 返回值 */
          }                                             /* 代码块结束 */
        } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */

        /* Clear CCF flag */
        __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);   /* HAL辅助宏/函数 */
      }                                                 /* 代码块结束 */
      /*  Last block optionally pad the data with zeros*/
      for (loopcounter = 0U; (loopcounter < ((headersize_in_bytes /4U) % 4U)); loopcounter++)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        hcryp->CrypHeaderCount++ ;                      /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* If the header size is a multiple of words */
      if ((headersize_in_bytes % 4U) == 0U)             /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Pad the data with zeros to have a complete block */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DINR = 0x0U;                 /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        /* Enter last bytes, padded with zeroes */
        tmp =  *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
        tmp &= mask[(hcryp->Init.DataType * 2U) + (headersize_in_bytes % 4U)];  /* 赋值或初始化 */
        hcryp->Instance->DINR = tmp;                    /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
        /* Pad the data with zeros to have a complete block */
        while (loopcounter < 4U)                        /* while循环 */
        {                                               /* 代码块开始 */
          hcryp->Instance->DINR = 0x0U;                 /* 赋值或初始化 */
          loopcounter++;                                /* 执行语句 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      /*Wait on CCF flag*/
      count = CRYP_TIMEOUT_GCMCCMHEADERPHASE;           /* 赋值或初始化 */
      do                                                /* do-while循环 */
      {                                                 /* 代码块开始 */
        count-- ;                                       /* 执行语句 */
        if (count == 0U)                                /* 条件判断 */
        {                                               /* 代码块开始 */
          /* Disable the CRYP peripheral clock */
          __HAL_CRYP_DISABLE(hcryp);                    /* HAL辅助宏/函数 */

          /* Change state */
          hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;   /* 赋值或初始化 */
          hcryp->State = HAL_CRYP_STATE_READY;          /* 赋值或初始化 */

          /* Process unlocked */
          __HAL_UNLOCK(hcryp);                          /* HAL辅助宏/函数 */
          return HAL_ERROR;                             /* 返回值 */
        }                                               /* 代码块结束 */
      } while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF));  /* 代码块结束 */

      /* Clear CCF flag */
      __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);     /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
#endif /* End AES or CRYP  */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
#if defined(AES)                                        /* 条件编译 */
    if (hcryp->Init.Algorithm == CRYP_AES_GCM_GMAC)     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /*Workaround 1: only AES, before re-enabling the IP, datatype can be configured.*/
      MODIFY_REG(hcryp->Instance->CR, AES_CR_DATATYPE, hcryp->Init.DataType);  /* 执行语句 */

      /* Select header phase */
      CRYP_SET_PHASE(hcryp, CRYP_PHASE_HEADER);         /* 执行语句 */

      /* Enable the CRYP peripheral */
      __HAL_CRYP_ENABLE(hcryp);                         /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
#endif /* AES */
  }                                                     /* 代码块结束 */
  /* Return function status */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

/**
  * @brief  Sets the header phase in interrupt mode
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module(Header & HeaderSize)
  * @retval None
  */
static void CRYP_GCMCCM_SetHeaderPhase_IT(CRYP_HandleTypeDef *hcryp)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t loopcounter;                                 /* 32位无符号变量/函数 */
#if defined(AES)                                        /* 条件编译 */
  uint32_t lastwordsize;                                /* 32位无符号变量/函数 */
  uint32_t npblb;                                       /* 32位无符号变量/函数 */
#endif                                                  /* 条件编译结束 */
  uint32_t headersize_in_bytes;                         /* 32位无符号变量/函数 */
  uint32_t tmp;                                         /* 32位无符号变量/函数 */
  uint32_t mask[12] = {0x0U, 0xFF000000U, 0xFFFF0000U, 0xFFFFFF00U,  /* 32-bit data type */
                       0x0U, 0x0000FF00U, 0x0000FFFFU, 0xFF00FFFFU,  /* 16-bit data type */
                       0x0U, 0x000000FFU, 0x0000FFFFU, 0x00FFFFFFU}; /*  8-bit data type */

  if (hcryp->Init.HeaderWidthUnit == CRYP_HEADERWIDTHUNIT_WORD)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    headersize_in_bytes = hcryp->Init.HeaderSize * 4U;  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    headersize_in_bytes = hcryp->Init.HeaderSize;       /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

  /***************************** Header phase *********************************/

#if defined(CRYP)                                       /* 条件编译 */
  if (headersize_in_bytes <= ((uint32_t)(hcryp->CrypHeaderCount) * 4U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Disable interrupts */
    __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_INI);          /* HAL辅助宏/函数 */

    /* Disable the CRYP peripheral */
    __HAL_CRYP_DISABLE(hcryp);                          /* HAL辅助宏/函数 */

    /* Set the phase */
    hcryp->Phase = CRYP_PHASE_PROCESS;                  /* 赋值或初始化 */

    /* Select payload phase once the header phase is performed */
    CRYP_SET_PHASE(hcryp, CRYP_PHASE_PAYLOAD);          /* 执行语句 */

    /* Enable Interrupts */
    __HAL_CRYP_ENABLE_IT(hcryp, CRYP_IT_INI | CRYP_IT_OUTI);  /* HAL辅助宏/函数 */

    /* Enable the CRYP peripheral */
    __HAL_CRYP_ENABLE(hcryp);                           /* HAL辅助宏/函数 */
  }                                                     /* 代码块结束 */
  else if (((headersize_in_bytes / 4U) - (hcryp->CrypHeaderCount)) >= 4U)  /* 否则如果 */

  {                                                     /* 代码块开始 */
    /* HeaderSize %4, no padding */
    hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
    hcryp->CrypHeaderCount++ ;                          /* 执行语句 */
    hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
    hcryp->CrypHeaderCount++  ;                         /* 执行语句 */
    hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
    hcryp->CrypHeaderCount++ ;                          /* 执行语句 */
    hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
    hcryp->CrypHeaderCount++ ;                          /* 执行语句 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /*  Last block optionally pad the data with zeros*/
    for (loopcounter = 0U; loopcounter < ((headersize_in_bytes / 4U) % 4U); loopcounter++)  /* for循环 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
      hcryp->CrypHeaderCount++ ;                        /* 执行语句 */
    }                                                   /* 代码块结束 */
    if ((headersize_in_bytes % 4U) == 0U)               /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Pad the data with zeros to have a complete block */
      while (loopcounter < 4U)                          /* while循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DIN = 0x0U;                    /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
        hcryp->CrypHeaderCount++;                       /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enter last bytes, padded with zeros */
      tmp =  *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
      tmp &= mask[(hcryp->Init.DataType * 2U) + (headersize_in_bytes % 4U)];  /* 赋值或初始化 */
      hcryp->Instance->DIN = tmp;                       /* 赋值或初始化 */
      loopcounter++;                                    /* 执行语句 */
      hcryp->CrypHeaderCount++;                         /* 执行语句 */
      /* Pad the data with zeros to have a complete block */
    while (loopcounter < 4U)                            /* while循环 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DIN = 0x0U;                      /* 赋值或初始化 */
      loopcounter++;                                    /* 执行语句 */
        hcryp->CrypHeaderCount++;                       /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#else /* AES */

  if (headersize_in_bytes <= ((uint32_t)(hcryp->CrypHeaderCount) * 4U))  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* Set the phase */
    hcryp->Phase = CRYP_PHASE_PROCESS;                  /* 赋值或初始化 */

    /*  Payload phase not supported in CCM AES2  */
    if (hcryp->Init.Algorithm == CRYP_AES_GCM_GMAC)     /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Select payload phase once the header phase is performed */
      MODIFY_REG(hcryp->Instance->CR, AES_CR_GCMPH, CRYP_PHASE_PAYLOAD);  /* 执行语句 */
    }                                                   /* 代码块结束 */
    if (hcryp->Init.Algorithm == CRYP_AES_CCM)          /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Increment CrypHeaderCount to pass in CRYP_GCMCCM_SetPayloadPhase_IT */
      hcryp->CrypHeaderCount++;                         /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* Write the payload Input block in the IN FIFO */
    if (hcryp->Size == 0U)                              /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable interrupts */
      __HAL_CRYP_DISABLE_IT(hcryp, CRYP_IT_CCFIE | CRYP_IT_ERRIE);  /* HAL辅助宏/函数 */

      /* Change the CRYP state */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    else if (hcryp->Size >= 16U)                        /* 否则如果 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
      hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */

      if ((hcryp->CrypInCount == (hcryp->Size / 4U)) && ((hcryp->Size % 16U) == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Call the input data transfer complete callback */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered Input complete callback*/
        hcryp->InCpltCallback(hcryp);                   /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak Input complete callback*/
        HAL_CRYP_InCpltCallback(hcryp);                 /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else /* Size < 4 words  : first block is the last block*/
    {                                                   /* 代码块开始 */
      /* Workaround not implemented, Size should be %4  otherwise Tag will  be incorrectly
      generated for GCM Encryption. Workaround is implemented in polling mode, so if last block of
      payload <128bit don't use CRYP_Encrypt_IT otherwise TAG is incorrectly generated for GCM Encryption. */

      /* Compute the number of padding bytes in last block of payload */
      npblb = ((((uint32_t)(hcryp->Size) / 16U) + 1U) * 16U) - (uint32_t)(hcryp->Size);  /* 赋值或初始化 */

      /* Number of valid words (lastwordsize) in last block */
      if ((npblb % 4U) == 0U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        lastwordsize = (16U - npblb) / 4U;              /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      else                                              /* 否则 */
      {                                                 /* 代码块开始 */
        lastwordsize = ((16U - npblb) / 4U) + 1U;       /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /*  Last block optionally pad the data with zeros*/
      for (loopcounter = 0U; loopcounter < lastwordsize; loopcounter++)  /* for循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DINR = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
        hcryp->CrypInCount++;                           /* 执行语句 */
      }                                                 /* 代码块结束 */
      while (loopcounter < 4U)                          /* while循环 */
      {                                                 /* 代码块开始 */
        /* Pad the data with zeros to have a complete block */
        hcryp->Instance->DINR = 0x0U;                   /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  else if (((headersize_in_bytes / 4U) - (hcryp->CrypHeaderCount)) >= 4U)  /* 否则如果 */
  {                                                     /* 代码块开始 */
    /* Write the input block in the IN FIFO */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
    hcryp->CrypHeaderCount++;                           /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
    hcryp->CrypHeaderCount++;                           /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
    hcryp->CrypHeaderCount++;                           /* 执行语句 */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
    hcryp->CrypHeaderCount++;                           /* 执行语句 */
  }                                                     /* 代码块结束 */
  else /*HeaderSize < 4 or HeaderSize >4 & HeaderSize %4 != 0*/
  {                                                     /* 代码块开始 */
    /*  Last block optionally pad the data with zeros*/
    for (loopcounter = 0U; loopcounter < ((headersize_in_bytes / 4U) % 4U); loopcounter++)  /* for循环 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DINR = *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
      hcryp->CrypHeaderCount++ ;                        /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* If the header size is a multiple of words */
    if ((headersize_in_bytes % 4U) == 0U)               /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Pad the data with zeros to have a complete block */
      while (loopcounter < 4U)                          /* while循环 */
      {                                                 /* 代码块开始 */
        hcryp->Instance->DINR = 0x0U;                   /* 赋值或初始化 */
        loopcounter++;                                  /* 执行语句 */
        hcryp->CrypHeaderCount++;                       /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else                                                /* 否则 */
    {                                                   /* 代码块开始 */
      /* Enter last bytes, padded with zeros */
      tmp =  *(uint32_t *)(hcryp->Init.Header + hcryp->CrypHeaderCount);  /* 赋值或初始化 */
      tmp &= mask[(hcryp->Init.DataType * 2U) + (headersize_in_bytes % 4U)];  /* 赋值或初始化 */
      hcryp->Instance->DINR = tmp;                      /* 赋值或初始化 */
      loopcounter++;                                    /* 执行语句 */
      hcryp->CrypHeaderCount++;                         /* 执行语句 */
      /* Pad the data with zeros to have a complete block */
    while (loopcounter < 4U)                            /* while循环 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DINR = 0x0U;                     /* 赋值或初始化 */
      loopcounter++;                                    /* 执行语句 */
        hcryp->CrypHeaderCount++;                       /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
#endif /* End AES or CRYP */
}                                                       /* 代码块结束 */


/**
  * @brief  Workaround used for GCM/CCM mode.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module
  * @param  Timeout: specify Timeout value
  * @retval None
  */
static void CRYP_Workaround(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态函数 */
{                                                       /* 代码块开始 */
  uint32_t lastwordsize;                                /* 32位无符号变量/函数 */
  uint32_t npblb;                                       /* 32位无符号变量/函数 */
#if defined(CRYP)                                       /* 条件编译 */
  uint32_t  iv1temp;                                    /* 32位无符号变量/函数 */
  uint32_t  temp[4] = {0};                              /* 32位无符号变量/函数 */
  uint32_t  temp2[4] = {0};                             /* 32位无符号变量/函数 */
#endif /* CRYP */
  uint32_t intermediate_data[4] = {0};                  /* 32位无符号变量/函数 */
  uint32_t index;                                       /* 32位无符号变量/函数 */

  /* Compute the number of padding bytes in last block of payload */
  npblb = ((((uint32_t)(hcryp->Size) / 16U) + 1U) * 16U) - (uint32_t)(hcryp->Size);  /* 赋值或初始化 */

  /* Number of valid words (lastwordsize) in last block */
  if ((npblb % 4U) == 0U)                               /* 条件判断 */
  {                                                     /* 代码块开始 */
    lastwordsize = (16U - npblb) / 4U;                  /* 赋值或初始化 */
  }                                                     /* 代码块结束 */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    lastwordsize = ((16U - npblb) / 4U) + 1U;           /* 赋值或初始化 */
  }                                                     /* 代码块结束 */

#if defined(CRYP)                                       /* 条件编译 */

  /* Workaround 2, case GCM encryption */
  if (hcryp->Init.Algorithm == CRYP_AES_GCM)            /* 条件判断 */
  {                                                     /* 代码块开始 */
    if ((hcryp->Instance->CR & CRYP_CR_ALGODIR) == CRYP_OPERATINGMODE_ENCRYPT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /*Workaround in order to properly compute authentication tags while doing
       a GCM encryption with the last block of payload size inferior to 128 bits*/
      /* Disable CRYP to start the final phase */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      /*Update CRYP_IV1R register and ALGOMODE*/
      hcryp->Instance->IV1RR = ((hcryp->Instance->CSGCMCCM7R) - 1U);  /* 赋值或初始化 */
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, CRYP_AES_CTR);  /* 执行语句 */

      /* Enable CRYP to start the final phase */
      __HAL_CRYP_ENABLE(hcryp);                         /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    /*  Last block optionally pad the data with zeros*/
    for (index = 0; index < lastwordsize; index ++)     /* for循环 */
    {                                                   /* 代码块开始 */
      /* Write the last input block in the IN FIFO */
      hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
    }                                                   /* 代码块结束 */
    while (index < 4U)                                  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Pad the data with zeros to have a complete block */
      hcryp->Instance->DIN  = 0U;                       /* 赋值或初始化 */
      index++;                                          /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* Wait for OFNE flag to be raised */
    if (CRYP_WaitOnOFNEFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the CRYP peripheral clock */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      /* Change state */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;       /* 赋值或初始化 */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
      /*Call registered error callback*/
      hcryp->ErrorCallback(hcryp);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call legacy weak error callback*/
      HAL_CRYP_ErrorCallback(hcryp);                    /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    if ((hcryp->Instance->SR & CRYP_FLAG_OFNE) != 0x0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      for (index = 0U; index < 4U; index++)             /* for循环 */
      {                                                 /* 代码块开始 */
        /* Read the output block from the output FIFO */
        intermediate_data[index] = hcryp->Instance->DOUT;  /* 赋值或初始化 */

        /* Intermediate data buffer to be used in for the workaround*/
        *(uint32_t *)(hcryp->pCrypOutBuffPtr + (hcryp->CrypOutCount)) = intermediate_data[index];  /* 赋值或初始化 */
        hcryp->CrypOutCount++;                          /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if ((hcryp->Instance->CR & CRYP_CR_ALGODIR) == CRYP_OPERATINGMODE_ENCRYPT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /*workaround in order to properly compute authentication tags while doing
      a GCM encryption with the last block of payload size inferior to 128 bits*/
      /* Change the AES mode to GCM mode and Select Final phase */
      /* configured  CHMOD GCM   */
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, CRYP_AES_GCM);  /* 执行语句 */

      /* configured  final phase  */
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_GCM_CCMPH, CRYP_PHASE_FINAL);  /* 执行语句 */

      if ((hcryp->Instance->CR & CRYP_CR_DATATYPE) == CRYP_DATATYPE_32B)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        if ((npblb % 4U) == 1U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= 0xFFFFFF00U;  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((npblb % 4U) == 2U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= 0xFFFF0000U;  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((npblb % 4U) == 3U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= 0xFF000000U;  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if ((hcryp->Instance->CR & CRYP_CR_DATATYPE) == CRYP_DATATYPE_8B)  /* 否则如果 */
      {                                                 /* 代码块开始 */
        if ((npblb % 4U) == 1U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= __REV(0xFFFFFF00U);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((npblb % 4U) == 2U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= __REV(0xFFFF0000U);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((npblb % 4U) == 3U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= __REV(0xFF000000U);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else if ((hcryp->Instance->CR & CRYP_CR_DATATYPE) == CRYP_DATATYPE_16B)  /* 否则如果 */
      {                                                 /* 代码块开始 */
        if ((npblb % 4U) == 1U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= __ROR((0xFFFFFF00U), 16);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((npblb % 4U) == 2U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= __ROR((0xFFFF0000U), 16);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((npblb % 4U) == 3U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= __ROR((0xFF000000U), 16);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      else /*CRYP_DATATYPE_1B*/
      {                                                 /* 代码块开始 */
        if ((npblb % 4U) == 1U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= __RBIT(0xFFFFFF00U);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((npblb % 4U) == 2U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= __RBIT(0xFFFF0000U);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
        if ((npblb % 4U) == 3U)                         /* 条件判断 */
        {                                               /* 代码块开始 */
          intermediate_data[lastwordsize - 1U] &= __RBIT(0xFF000000U);  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
      for (index = 0U; index < lastwordsize; index ++)  /* for循环 */
      {                                                 /* 代码块开始 */
        /*Write the intermediate_data in the IN FIFO */
        hcryp->Instance->DIN = intermediate_data[index];  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      while (index < 4U)                                /* while循环 */
      {                                                 /* 代码块开始 */
        /* Pad the data with zeros to have a complete block */
        hcryp->Instance->DIN  = 0x0U;                   /* 赋值或初始化 */
        index++;                                        /* 执行语句 */
      }                                                 /* 代码块结束 */
      /* Wait for OFNE flag to be raised */
      if (CRYP_WaitOnOFNEFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1)              /* 条件编译 */
        /*Call registered error callback*/
        hcryp->ErrorCallback(hcryp);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak error callback*/
        HAL_CRYP_ErrorCallback(hcryp);                  /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */

      if ((hcryp->Instance->SR & CRYP_FLAG_OFNE) != 0x0U)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        for (index = 0U; index < 4U; index++)           /* for循环 */
        {                                               /* 代码块开始 */
          intermediate_data[index] = hcryp->Instance->DOUT;  /* 赋值或初始化 */
        }                                               /* 代码块结束 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  } /* End of GCM encryption */
  else                                                  /* 否则 */
  {                                                     /* 代码块开始 */
    /* Workaround 2, case CCM decryption, in order to properly compute
      authentication tags while doing a CCM decryption with the last block
      of payload size inferior to 128 bits*/

    if ((hcryp->Instance->CR & CRYP_CR_ALGODIR) == CRYP_OPERATINGMODE_DECRYPT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      iv1temp = hcryp->Instance->CSGCMCCM7R;            /* 赋值或初始化 */

      /* Disable CRYP to start the final phase */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      temp[0] =  hcryp->Instance->CSGCMCCM0R;           /* 赋值或初始化 */
      temp[1] =  hcryp->Instance->CSGCMCCM1R;           /* 赋值或初始化 */
      temp[2] =  hcryp->Instance->CSGCMCCM2R;           /* 赋值或初始化 */
      temp[3] =  hcryp->Instance->CSGCMCCM3R;           /* 赋值或初始化 */

      hcryp->Instance->IV1RR = iv1temp;                 /* 赋值或初始化 */

      /* Configured  CHMOD CTR   */
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, CRYP_AES_CTR);  /* 执行语句 */

      /* Enable CRYP to start the final phase */
      __HAL_CRYP_ENABLE(hcryp);                         /* HAL辅助宏/函数 */
    }                                                   /* 代码块结束 */
    /*  Last block optionally pad the data with zeros*/
    for (index = 0; index < lastwordsize; index ++)     /* for循环 */
    {                                                   /* 代码块开始 */
      /* Write the last Input block in the IN FIFO */
      hcryp->Instance->DIN  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
      hcryp->CrypInCount++;                             /* 执行语句 */
    }                                                   /* 代码块结束 */
    while (index < 4U)                                  /* while循环 */
    {                                                   /* 代码块开始 */
      /* Pad the data with zeros to have a complete block */
      hcryp->Instance->DIN  = 0U;                       /* 赋值或初始化 */
      index++;                                          /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* Wait for OFNE flag to be raised */
    if (CRYP_WaitOnOFNEFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the CRYP peripheral clock */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      /* Change state */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;       /* 赋值或初始化 */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)             /* 条件编译 */
      /*Call registered error callback*/
      hcryp->ErrorCallback(hcryp);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call legacy weak error callback*/
      HAL_CRYP_ErrorCallback(hcryp);                    /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */

    if ((hcryp->Instance->SR & CRYP_FLAG_OFNE) != 0x0U)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      for (index = 0U; index < 4U; index++)             /* for循环 */
      {                                                 /* 代码块开始 */
        /* Read the Output block from the Output FIFO */
        intermediate_data[index] = hcryp->Instance->DOUT;  /* 赋值或初始化 */

        /*intermediate data buffer to be used in for the workaround*/
        *(uint32_t *)(hcryp->pCrypOutBuffPtr + (hcryp->CrypOutCount)) = intermediate_data[index];  /* 赋值或初始化 */
        hcryp->CrypOutCount++;                          /* 执行语句 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    if ((hcryp->Instance->CR & CRYP_CR_ALGODIR) == CRYP_OPERATINGMODE_DECRYPT)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      temp2[0] =  hcryp->Instance->CSGCMCCM0R;          /* 赋值或初始化 */
      temp2[1] =  hcryp->Instance->CSGCMCCM1R;          /* 赋值或初始化 */
      temp2[2] =  hcryp->Instance->CSGCMCCM2R;          /* 赋值或初始化 */
      temp2[3] =  hcryp->Instance->CSGCMCCM3R;          /* 赋值或初始化 */

      /* configured  CHMOD CCM   */
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_ALGOMODE, CRYP_AES_CCM);  /* 执行语句 */

      /* configured  Header phase  */
      MODIFY_REG(hcryp->Instance->CR, CRYP_CR_GCM_CCMPH, CRYP_PHASE_HEADER);  /* 执行语句 */

      /*set to zero the bits corresponding to the padded bits*/
      for (index = lastwordsize; index < 4U; index ++)  /* for循环 */
      {                                                 /* 代码块开始 */
        intermediate_data[index] = 0U;                  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 1U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= 0xFFFFFF00U;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 2U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= 0xFFFF0000U;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 3U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= 0xFF000000U;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      for (index = 0U; index < 4U ; index ++)           /* for循环 */
      {                                                 /* 代码块开始 */
        intermediate_data[index] ^=  temp[index];       /* 赋值或初始化 */
        intermediate_data[index] ^=  temp2[index];      /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      for (index = 0U; index < 4U; index ++)            /* for循环 */
      {                                                 /* 代码块开始 */
        /* Write the last Input block in the IN FIFO */
        hcryp->Instance->DIN  = intermediate_data[index] ;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */

      /* Wait for BUSY flag to be raised */
      if (CRYP_WaitOnBUSYFlag(hcryp, Timeout) != HAL_OK)  /* 条件判断 */
      {                                                 /* 代码块开始 */
        /* Disable the CRYP peripheral clock */
        __HAL_CRYP_DISABLE(hcryp);                      /* HAL辅助宏/函数 */

        /* Change state */
        hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;     /* 赋值或初始化 */
        hcryp->State = HAL_CRYP_STATE_READY;            /* 赋值或初始化 */

        /* Process Unlocked */
        __HAL_UNLOCK(hcryp);                            /* HAL辅助宏/函数 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)             /* 条件编译 */
        /*Call registered error callback*/
        hcryp->ErrorCallback(hcryp);                    /* 执行语句 */
#else                                                   /* 条件编译否则 */
        /*Call legacy weak error callback*/
        HAL_CRYP_ErrorCallback(hcryp);                  /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  } /* End of CCM WKA*/

  /* Process Unlocked */
  __HAL_UNLOCK(hcryp);                                  /* HAL辅助宏/函数 */

#else /* AES */

  /*Workaround 2: case GCM encryption, during payload phase and before inserting
  the last block of paylaod, which size is inferior to  128 bits  */

  if ((hcryp->Instance->CR & AES_CR_MODE) == CRYP_OPERATINGMODE_ENCRYPT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* configured  CHMOD CTR   */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_CHMOD, CRYP_AES_CTR);  /* 执行语句 */
  }                                                     /* 代码块结束 */
  /*  last block optionally pad the data with zeros*/
  for (index = 0U; index < lastwordsize; index ++)      /* for循环 */
  {                                                     /* 代码块开始 */
    /* Write the last Input block in the IN FIFO */
    hcryp->Instance->DINR  = *(uint32_t *)(hcryp->pCrypInBuffPtr + hcryp->CrypInCount);  /* 赋值或初始化 */
    hcryp->CrypInCount++;                               /* 执行语句 */
  }                                                     /* 代码块结束 */
  while (index < 4U)                                    /* while循环 */
  {                                                     /* 代码块开始 */
    /* pad the data with zeros to have a complete block */
    hcryp->Instance->DINR  = 0U;                        /* 赋值或初始化 */
    index++;                                            /* 执行语句 */
  }                                                     /* 代码块结束 */
  /* Wait for CCF flag to be raised */
  if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)      /* 条件判断 */
  {                                                     /* 代码块开始 */
    hcryp->State = HAL_CRYP_STATE_READY;                /* 赋值或初始化 */
    __HAL_UNLOCK(hcryp);                                /* HAL辅助宏/函数 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)             /* 条件编译 */
    /*Call registered error callback*/
    hcryp->ErrorCallback(hcryp);                        /* 执行语句 */
#else                                                   /* 条件编译否则 */
    /*Call legacy weak error callback*/
    HAL_CRYP_ErrorCallback(hcryp);                      /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
  }                                                     /* 代码块结束 */

  /* Clear CCF Flag */
  __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);         /* HAL辅助宏/函数 */

  for (index = 0U; index < 4U; index++)                 /* for循环 */
  {                                                     /* 代码块开始 */
    /* Read the Output block from the Output FIFO */
    intermediate_data[index] = hcryp->Instance->DOUTR;  /* 赋值或初始化 */

    /*intermediate data buffer to be used in  the workaround*/
    *(uint32_t *)(hcryp->pCrypOutBuffPtr + (hcryp->CrypOutCount)) = intermediate_data[index];  /* 赋值或初始化 */
    hcryp->CrypOutCount++;                              /* 执行语句 */
  }                                                     /* 代码块结束 */

  if ((hcryp->Instance->CR & AES_CR_MODE) == CRYP_OPERATINGMODE_ENCRYPT)  /* 条件判断 */
  {                                                     /* 代码块开始 */
    /* configured  CHMOD GCM   */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_CHMOD, CRYP_AES_GCM_GMAC);  /* 执行语句 */

    /* Select final phase */
    MODIFY_REG(hcryp->Instance->CR, AES_CR_GCMPH, CRYP_PHASE_FINAL);  /* 执行语句 */

    if ((hcryp->Instance->CR & AES_CR_DATATYPE) == CRYP_DATATYPE_32B)  /* 条件判断 */
    {                                                   /* 代码块开始 */
      if ((npblb % 4U) == 1U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= 0xFFFFFF00U;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 2U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= 0xFFFF0000U;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 3U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= 0xFF000000U;  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if ((hcryp->Instance->CR & AES_CR_DATATYPE) == CRYP_DATATYPE_8B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if ((npblb % 4U) == 1U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= __REV(0xFFFFFF00U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 2U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= __REV(0xFFFF0000U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 3U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= __REV(0xFF000000U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else if ((hcryp->Instance->CR & AES_CR_DATATYPE) == CRYP_DATATYPE_16B)  /* 否则如果 */
    {                                                   /* 代码块开始 */
      if ((npblb % 4U) == 1U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= __ROR((0xFFFFFF00U), 16);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 2U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= __ROR((0xFFFF0000U), 16);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 3U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= __ROR((0xFF000000U), 16);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
    else /*CRYP_DATATYPE_1B*/
    {                                                   /* 代码块开始 */
      if ((npblb % 4U) == 1U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= __RBIT(0xFFFFFF00U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 2U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= __RBIT(0xFFFF0000U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
      if ((npblb % 4U) == 3U)                           /* 条件判断 */
      {                                                 /* 代码块开始 */
        intermediate_data[lastwordsize - 1U] &= __RBIT(0xFF000000U);  /* 赋值或初始化 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */

    /*Write the intermediate_data in the IN FIFO */
    for (index = 0U; index < lastwordsize; index ++)    /* for循环 */
    {                                                   /* 代码块开始 */
      hcryp->Instance->DINR  = intermediate_data[index];  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
    while (index < 4U)                                  /* while循环 */
    {                                                   /* 代码块开始 */
      /* pad the data with zeros to have a complete block */
      hcryp->Instance->DINR = 0U;                       /* 赋值或初始化 */
      index++;                                          /* 执行语句 */
    }                                                   /* 代码块结束 */
    /* Wait for CCF flag to be raised */
    if (CRYP_WaitOnCCFlag(hcryp, Timeout) != HAL_OK)    /* 条件判断 */
    {                                                   /* 代码块开始 */
      /* Disable the CRYP peripheral clock */
      __HAL_CRYP_DISABLE(hcryp);                        /* HAL辅助宏/函数 */

      /* Change state */
      hcryp->ErrorCode |= HAL_CRYP_ERROR_TIMEOUT;       /* 赋值或初始化 */
      hcryp->State = HAL_CRYP_STATE_READY;              /* 赋值或初始化 */

      /* Process Unlocked */
      __HAL_UNLOCK(hcryp);                              /* HAL辅助宏/函数 */
#if (USE_HAL_CRYP_REGISTER_CALLBACKS == 1U)             /* 条件编译 */
      /*Call registered error callback*/
      hcryp->ErrorCallback(hcryp);                      /* 执行语句 */
#else                                                   /* 条件编译否则 */
      /*Call legacy weak error callback*/
      HAL_CRYP_ErrorCallback(hcryp);                    /* HAL库函数调用 */
#endif /* USE_HAL_CRYP_REGISTER_CALLBACKS */
    }                                                   /* 代码块结束 */
    /* Clear CCF Flag */
    __HAL_CRYP_CLEAR_FLAG(hcryp, CRYP_CCF_CLEAR);       /* HAL辅助宏/函数 */

    for (index = 0U; index < 4U; index++)               /* for循环 */
    {                                                   /* 代码块开始 */
      intermediate_data[index] = hcryp->Instance->DOUTR;  /* 赋值或初始化 */
    }                                                   /* 代码块结束 */
  }/*End of Workaround 2*/
#endif /* End AES or CRYP */
}                                                       /* 代码块结束 */
#endif /* AES or GCM CCM defined*/
#if defined (CRYP)                                      /* 条件编译 */
#if defined (CRYP_CR_ALGOMODE_AES_GCM)                  /* 条件编译 */
/**
  * @brief  Handle CRYP hardware block Timeout when waiting for IFEM flag to be raised.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @param  Timeout: Timeout duration.
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_WaitOnIFEMFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Get timeout */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  while (HAL_IS_BIT_CLR(hcryp->Instance->SR, CRYP_FLAG_IFEM))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */
#endif /* GCM CCM defined*/
/**
  * @brief  Handle CRYP hardware block Timeout when waiting for BUSY flag to be raised.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @param  Timeout: Timeout duration.
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_WaitOnBUSYFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Get timeout */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  while (HAL_IS_BIT_SET(hcryp->Instance->SR, CRYP_FLAG_BUSY))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */


/**
  * @brief  Handle CRYP hardware block Timeout when waiting for OFNE flag to be raised.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @param  Timeout: Timeout duration.
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_WaitOnOFNEFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Get timeout */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  while (HAL_IS_BIT_CLR(hcryp->Instance->SR, CRYP_FLAG_OFNE))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#else /* AES */

/**
  * @brief  Handle CRYP hardware block Timeout when waiting for CCF flag to be raised.
  * @param  hcryp: pointer to a CRYP_HandleTypeDef structure that contains
  *         the configuration information for CRYP module.
  * @param  Timeout: Timeout duration.
  * @retval HAL status
  */
static HAL_StatusTypeDef CRYP_WaitOnCCFlag(CRYP_HandleTypeDef *hcryp, uint32_t Timeout)  /* 静态变量/函数 */
{                                                       /* 代码块开始 */
  uint32_t tickstart;                                   /* 32位无符号变量/函数 */

  /* Get timeout */
  tickstart = HAL_GetTick();                            /* 赋值或初始化 */

  while (HAL_IS_BIT_CLR(hcryp->Instance->SR, AES_SR_CCF))  /* while循环 */
  {                                                     /* 代码块开始 */
    /* Check for the Timeout */
    if (Timeout != HAL_MAX_DELAY)                       /* 条件判断 */
    {                                                   /* 代码块开始 */
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))  /* 条件判断 */
      {                                                 /* 代码块开始 */
        return HAL_ERROR;                               /* 返回值 */
      }                                                 /* 代码块结束 */
    }                                                   /* 代码块结束 */
  }                                                     /* 代码块结束 */
  return HAL_OK;                                        /* 返回值 */
}                                                       /* 代码块结束 */

#endif /* End AES or CRYP  */


/**
  * @}
  */



/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_CRYP_MODULE_ENABLED */


/**
  * @}
  */
#endif /* TinyAES or CRYP*/
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
