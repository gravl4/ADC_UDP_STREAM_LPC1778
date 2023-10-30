/*
 * @brief LWIP HTTP Webserver example
 *
 *
 *  Плата DKST 51.1.10 или DKST 51.1.11
 *
 *
 *
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "lwip/init.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/memp.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
//#include "lwip/timers.h"
#include "netif/etharp.h"
#include "lwip/udp.h"

#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif

#include <string.h>
#include "board.h"
#include "lpc_phy.h"
#include "lpc17xx_40xx_emac.h"
#include "lpc_arch.h"
#include "sys_arch.h"
#include "lpc_phy.h"/* For the PHY monitor support */
#include "ringbuffer.h"

#define _1000HZ
//#define _100HZ
//#define _10HZ

#ifdef _1000HZ
#define TICKRATE_HZ1 (1000)
#endif
#ifdef _100HZ
#define TICKRATE_HZ1 (100)
#endif
#ifdef _10HZ
#define TICKRATE_HZ1 (10)
#endif


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* NETIF data */
static struct netif lpc_netif;

//---------------------------------------------------------------
struct AMessage
{
    uint16_t ucMessageID;
    uint16_t adcData[8];
    uint16_t digitalData;
};

/* Create and initialize ring buffer */
ring_buffer_t ring_buffer;
char buf_arr[1024];
static send_cnt;
static int cable_state = 0;
unsigned char send_buffer[1500];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

void udp_command_init(void);
void udp_command_receive_callback(void *arg, struct udp_pcb *upcb,
		struct pbuf *p, const ip_addr_t *addr, u16_t port);
void udp_data_init(void);
extern void httpd_init(void);

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	/* LED0 is used for the link status, on = PHY cable detected */
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial LED state is off to show an unconnected cable state */
	Board_LED_Set(0, false);
	/* Setup a 1mS sysTick for the primary time base */
	SysTick_Enable(1);
}

/**
 * @brief	Handle interrupt from 32-bit timer
 * @return	Nothing
 */
volatile uint16_t count = 0;
/*канал 1 - 0..4095 пила  100гц
2 -  0..4095 пила  10гц
3 -  0..4095 пила  1гц
4 -  0..4095 пила  0,1гц
5 -  0..4095 пила  0,01гц*/
static double adcData[8] = {0};
const double step[8] = {(4096.0 / (1000.0 / 100.0)), (4096.0 / (1000.0 / 10.0)), (4096.0 / (1000.0 / 1.0)),
					(4096.0 / (1000.0 / 0.1)), (4096.0 / (1000.0 / 0.01)), 0.0, 0.0, 0.0};

volatile uint16_t time_counter = 0;
volatile uint16_t adc_data[8] = {0};
volatile uint16_t dig_data = 0;
volatile uint8_t freq_flag = 0;
void TIMER0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER0, 1)) {
		Chip_TIMER_ClearMatch(LPC_TIMER0, 1);
		Board_LED_Toggle(0);

		freq_flag =1 ;
	 	//struct AMessage xRxedStructure;
	 	//xRxedStructure.ucMessageID = count++;
	 	time_counter++;

	 	for (int i = 0; i < 8; i++) {
	 		adcData[i] = (double)adcData[i] + step[i];
	 		if (adcData[i] > 4095)
	 			adcData[i] = 0;
	 		adc_data[i] = adcData[i];
	 	}

	 	static uint16_t add = 0;
	 	add++;
	 	if (add > 3000) {
	 		dig_data = dig_data << 1;
	 		add = 0;
	 	}
	 	dig_data++;

	 //	ring_buffer_queue_arr(&ring_buffer, &xRxedStructure, sizeof(struct AMessage));
	}
}

static struct udp_pcb *upcb_command;
static struct udp_pcb *upcb_data;

/*********************************************************************//**
 * @brief
 *
 * @return	None
 **********************************************************************/
void udp_command_receive_callback(void *arg, struct udp_pcb *upcb,
		struct pbuf *p, const ip_addr_t *addr, u16_t port) {
	struct pbuf *p1;
	uint8_t data[64] = { 0 };

	sprintf((char*) data, "Hello world!");
	/* allocate pbuf from pool*/
	p1 = pbuf_alloc(PBUF_TRANSPORT, strlen((char*) data), PBUF_RAM);
	if (p1 != NULL) {
		/* copy data to pbuf */
		pbuf_take(p1, (char*) data, strlen((char*) data));

		/* send udp data */
		udp_sendto(upcb, p1, addr, port);
		//DEBUGOUT("*");
		/* free pbuf */
		pbuf_free(p1);
	}

	/* Free the p buffer */
	pbuf_free(p);

}

/*********************************************************************//**
 * @brief
 *
 * @return	None
 **********************************************************************/
void udp_command_init(void) {
	/* Create a new UDP control block  */
	upcb_command = udp_new();

	if (upcb_command != NULL) {
		err_t err;

		err = udp_bind(upcb_command, IP_ADDR_ANY, 2002);
		if (err == ERR_OK) {
			udp_recv(upcb_command, udp_command_receive_callback, NULL);
		} else {
			/* abort? output diagnostic? */
		}
	} else {
		/* abort? output diagnostic? */
	}
}

/**
 * @brief	main routine for example_lwip_tcpecho_freertos_17xx40xx
 * @return	Function should not exit
 */

int main(void) {
	uint32_t physts;
	ip_addr_t ipaddr, netmask, gw;
	static int prt_ip = 0;
	struct AMessage xTxedStructure;
	ip_addr_t destIPAddr;
	int total_cnt;
	uint32_t timerFreq;

	prvSetupHardware();

	ring_buffer_init(&ring_buffer, buf_arr, 1024);

	/* Enable timer 1 clock */
	Chip_TIMER_Init(LPC_TIMER0);

	/* Timer rate is system clock rate */
	timerFreq = Chip_Clock_GetSystemClockRate();

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 1);
	Chip_TIMER_SetMatch(LPC_TIMER0, 1, (timerFreq / TICKRATE_HZ1));
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 1);
	Chip_TIMER_Enable(LPC_TIMER0);

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);

	/* Initialize LWIP */
	lwip_init();

	LWIP_DEBUGF(LWIP_DBG_ON, ("Starting LWIP WEB server...\n"));

	/* Static IP assignment */
#if LWIP_DHCP
	IP4_ADDR(&gw, 0, 0, 0, 0);
	IP4_ADDR(&ipaddr, 0, 0, 0, 0);
	IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
	IP4_ADDR(&gw, 192, 168, 0, 1);	//10, 1, 10, 1);
	IP4_ADDR(&ipaddr, 192, 168, 0, 103);	//10, 1, 10, 234);
	IP4_ADDR(&netmask, 255, 255, 255, 0);
#endif

	/* Add netif interface for lpc17xx_8x */
	netif_add(&lpc_netif, &ipaddr, &netmask, &gw, NULL, lpc_enetif_init, ethernet_input);
	netif_set_default(&lpc_netif);
	netif_set_up(&lpc_netif);

#if LWIP_DHCP
	dhcp_start(&lpc_netif);
#endif

	/* Initialize and start application */
	httpd_init();

	udp_command_init();

#ifdef _1000HZ
	total_cnt = 50 * 22;
#endif
#ifdef _100HZ
	total_cnt = 20 * 22;
#endif
#ifdef _10HZ
	total_cnt = 5 * 22;
#endif

	IP4_ADDR(&destIPAddr, 192, 168, 0, 5);
	/* Create a new UDP control block  */
	upcb_data = udp_new();

	/* This could be done in the sysTick ISR, but may stay in IRQ context
	   too long, so do this stuff with a background loop. */
	while (1) {
		/* Handle packets as part of this loop, not in the IRQ handler */
		lpc_enetif_input(&lpc_netif);

		/* lpc_rx_queue will re-qeueu receive buffers. This normally occurs
		   automatically, but in systems were memory is constrained, pbufs
		   may not always be able to get allocated, so this function can be
		   optionally enabled to re-queue receive buffers. */
#if 0
		while (lpc_rx_queue(&lpc_netif)) {}
#endif

		/* Free TX buffers that are done sending */
		lpc_tx_reclaim(&lpc_netif);

		/* LWIP timers - ARP, DHCP, TCP, etc. */
		sys_check_timeouts();

		/* Call the PHY status update state machine once in a while
		   to keep the link status up-to-date */
		physts = lpcPHYStsPoll();

		/* Only check for connection state when the PHY status has changed */
		if (physts & PHY_LINK_CHANGED) {
			if (physts & PHY_LINK_CONNECTED) {
				Board_LED_Set(0, true);
				prt_ip = 0;

				/* Set interface speed and duplex */
				if (physts & PHY_LINK_SPEED100) {
					Chip_ENET_Set100Mbps(LPC_ETHERNET);
//					NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 100000000);
				}
				else {
					Chip_ENET_Set10Mbps(LPC_ETHERNET);
//					NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 10000000);
				}
				if (physts & PHY_LINK_FULLDUPLX) {
					Chip_ENET_SetFullDuplex(LPC_ETHERNET);
				}
				else {
					Chip_ENET_SetHalfDuplex(LPC_ETHERNET);
				}

				netif_set_link_up(&lpc_netif);
			}
			else {
				Board_LED_Set(0, false);
				Board_LED_Set(1, false);
				netif_set_link_down(&lpc_netif);
			}

			DEBUGOUT("Link connect status: %d\r\n", ((physts & PHY_LINK_CONNECTED) != 0));
		}

		/* Print IP address info */
		if (!prt_ip) {
			if (lpc_netif.ip_addr.addr) {
				static char tmp_buff[16];
				DEBUGOUT("IP_ADDR    : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *) &lpc_netif.ip_addr, tmp_buff, 16));
				DEBUGOUT("NET_MASK   : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *) &lpc_netif.netmask, tmp_buff, 16));
				DEBUGOUT("GATEWAY_IP : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *) &lpc_netif.gw, tmp_buff, 16));
				prt_ip = 1;
				Board_LED_Set(1, true);
			}
		}

		//while (ring_buffer_dequeue_arr(&ring_buffer, &xTxedStructure, sizeof(struct AMessage)) == sizeof(struct AMessage)) {
		while (freq_flag) {
			freq_flag = 0;
			unsigned char out_buffer[32];
			out_buffer[0] = 0x55;
			out_buffer[1] = 0x33;
			out_buffer[2] = time_counter >> 8;
			out_buffer[3] = time_counter;
			for (int i = 0; i < 8; i++) {
				out_buffer[4 + i * 2] = adc_data[i] >> 8;
				out_buffer[4 + 1 + i * 2] = adc_data[i];
			}
/*
			for (int i = 0; i < 2; i++) {
				out_buffer[20 + i * 2] = xTxedStructure.digitalData[i]
						>> 8;
				out_buffer[20 + 1 + i * 2] =
						xTxedStructure.digitalData[i];
			}*/
			out_buffer[20] = dig_data >> 8;
			out_buffer[21] = dig_data & 0x00ff;

			for (int i = 0; i < 22; i++) {
				send_buffer[send_cnt++] = out_buffer[i];
			}

			if (send_cnt >= total_cnt) {
				struct pbuf *p;
				//strcpy((char*) out_buffer, "hello!\r\n");
				p = pbuf_alloc(PBUF_TRANSPORT, total_cnt, PBUF_RAM);
				if (p != NULL) {
					pbuf_take(p, (void*) send_buffer, total_cnt);

					udp_sendto(upcb_data, p, &destIPAddr, 2001);
					send_cnt = 0;

					pbuf_free(p);
				}
			}
		}
	}

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
