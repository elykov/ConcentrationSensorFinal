/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Net_Config.c
 * Purpose: Network Configuration
 * Rev.:    V5.00
 *----------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>System Definitions
// <i>Global Network System definitions
//   <s.15>Local Host Name
//   <i>This is the name under which embedded host can be
//   <i>accessed on a local area network.
//   <i>Default: "my_host"
#define NET_HOST_NAME           "my_host"

//   <o>Memory Pool size <1536-262144:4><#/4>
//   <i>This is the size of a memory pool in bytes. Buffers for
//   <i>Network packets are allocated from this memory pool.
//   <i>Default: 12000 bytes
#define NET_MEM_SIZE            3000

// </h>

#include "RTE_Components.h"

#ifdef  RTE_Network_Interface_ETH_0
#include "Net_Config_ETH_0.h"
#endif
#ifdef  RTE_Network_Interface_ETH_1
#include "Net_Config_ETH_1.h"
#endif

#ifdef  RTE_Network_Interface_PPP
#include "Net_Config_PPP.h"
#endif

#ifdef  RTE_Network_Interface_SLIP
#include "Net_Config_SLIP.h"
#endif

#ifdef  RTE_Network_Socket_TCP
#include "Net_Config_TCP.h"
#endif

#include "net_config.h"
#include "stm32f7xx_hal.h"

/**
\addtogroup net_genFunc
@{
*/
/**
  \fn          void net_sys_error (ERROR_CODE error)
  \ingroup     net_cores
  \brief       Network system error handler.
*/

extern void Exit(void);

void net_sys_error (ERROR_CODE error) {
  /* This function is called when a fatal error is encountered. */
  /* The normal program execution is not possible anymore.      */

	Exit();

  switch (error) {
    case ERR_MEM_ALLOC:
      /* Out of memory */
      break;

    case ERR_MEM_FREE:
      /* Trying to release non existing memory block */
      break;

    case ERR_MEM_CORRUPT:
      /* Memory Link pointer Corrupted */
      /* More data written than the size of allocated mem block */
      break;

    case ERR_MEM_LOCK:
      /* Locked Memory management function (alloc/free) re-entered */
      break;

    case ERR_UDP_ALLOC:
      /* Out of UDP Sockets */
      break;

    case ERR_TCP_ALLOC:
      /* Out of TCP Sockets */
      break;

    case ERR_TCP_STATE:
      /* TCP State machine in undefined state */
      break;
  }
}
/**
@}
*/
