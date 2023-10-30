/*
 * @brief Boot loader LWIP HTTP Webserver
 * Ten Vyacheslav
 * 30.03.2023
 *  Плата DKST 51.1.10 или DKST 51.1.11
 *
 */

#include "lwip/init.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/memp.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "netif/etharp.h"

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

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* NETIF data */
static struct netif lpc_netif;

//---------------------------------------------------------------

//static int cable_state = 0;

#define SMALL_PAGE_SIZE			4096	//4 KB.
#define LARGE_PAGE_SIZE			32768	//32 KB.

typedef enum
 {
 	PREPARE_SECTOR_FOR_WRITE	=50,
 	COPY_RAM_TO_FLASH			=51,
 	ERASE_SECTOR				=52,
 	BLANK_CHECK_SECTOR			=53,
 	READ_PART_ID				=54,
 	READ_BOOT_VER				=55,
 	COMPARE						=56,
 	REINVOKE_ISP				=57
 }IAP_Command_Code;

 #define CMD_SUCCESS 0

 /**    Table for start adress of sectors
  *
  *        LPC1768 internal flash memory sector numbers and addresses
  *
  *        LPC1768 flash memory are and sector number/size
  *        Table 568 "Sectors in a LPC17xx device", Section 5. "Sector numbers", usermanual
  *
  *        0x00000000 - 0x0007FFFF        flash (29 sectors)
  *
  *      Sector0:     0x00000000 - 0x00000FFF        4K
  *      Sector1:     0x00001000 - 0x00001FFF        4K
  *      Sector2:     0x00002000 - 0x00002FFF        4K
  *      Sector3:     0x00003000 - 0x00003FFF        4K
  *      Sector4:     0x00004000 - 0x00004FFF        4K
  *      Sector5:     0x00005000 - 0x00005FFF        4K
  *      Sector6:     0x00006000 - 0x00006FFF        4K
  *      Sector7:     0x00007000 - 0x00007FFF        4K
  *      Sector8:     0x00008000 - 0x00008FFF        4K
  *      Sector9:     0x00009000 - 0x00009FFF        4K
  *      Sector10:    0x0000A000 - 0x0000AFFF        4K
  *      Sector11:    0x0000B000 - 0x0000BFFF        4K
  *      Sector12:    0x0000C000 - 0x0000CFFF        4K
  *      Sector13:    0x0000D000 - 0x0000DFFF        4K
  *      Sector14:    0x0000E000 - 0x0000EFFF        4K
  *      Sector15:    0x0000F000 - 0x0000FFFF        4K
  *
  *      Sector16:    0x00010000 - 0x00017FFF        32K
  *      Sector17:    0x00018000 - 0x0001FFFF        32K
  *      Sector18:    0x00020000 - 0x00027FFF        32K
  *      Sector19:    0x00028000 - 0x0002FFFF        32K
  *      Sector20:    0x00030000 - 0x00037FFF        32K
  *      Sector21:    0x00038000 - 0x0003FFFF        32K
  *      Sector22:    0x00040000 - 0x00047FFF        32K
  *      Sector23:    0x00048000 - 0x0004FFFF        32K
  *      Sector24:    0x00050000 - 0x00057FFF        32K
  *      Sector25:    0x00058000 - 0x0005FFFF        32K
  *      Sector26:    0x00060000 - 0x00067FFF        32K
  *      Sector27:    0x00068000 - 0x0006FFFF        32K
  *      Sector28:    0x00070000 - 0x00077FFF        32K
  *      Sector29:    0x00078000 - 0x0007FFFF        32K
  */

#define SECTOR_START(sector)	((sector < 16)?( sector * 0x1000)         :( (sector - 14) * 0x8000)          )
#define SECTOR_END(sector)		((sector < 16)?((sector * 0x1000) + 0xFFF):(((sector - 14) * 0x8000) + 0x7FFF))

#define USER_START_SECTOR 16
#define MAX_USER_SECTOR 29

#define FLASH_BUF_SIZE 			1024	//4096	//512
#define USER_FLASH_START SECTOR_START(USER_START_SECTOR)
#define USER_FLASH_END	 SECTOR_END(MAX_USER_SECTOR)
#define USER_FLASH_SIZE  ((USER_FLASH_END - USER_FLASH_START) + 1)
#define MAX_FLASH_SECTOR 30

unsigned long command[5];
unsigned param_table[5];
unsigned result_table[5];
char flash_buf[1024];
uint32_t dataArrIndex = 0;

unsigned *flash_address = 0;
unsigned byte_ctr = 0;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

extern void httpd_init(void);

void find_erase_prepare_sector(unsigned cclk, unsigned flash_address);
void write_data(unsigned cclk, unsigned flash_address, unsigned *flash_data_buf, unsigned count);
void erase_sector(unsigned start_sector, unsigned end_sector, unsigned cclk);
void prepare_sector(unsigned start_sector, unsigned end_sector, unsigned cclk);
void compare_mem(void *flash_add, void *ram_add, int count);
void test_flash(unsigned sectorNumber);
unsigned write_flash(unsigned *dst, char *src, unsigned no_of_bytes);
void jump_to_app_section();

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

/*********************************************************************//**
 * @brief
 *
 * @return	None
 **********************************************************************/
unsigned write_flash(unsigned *dst, char *src, unsigned no_of_bytes) {
	unsigned i;

	if (flash_address == 0) {
		/* Store flash start address */
		flash_address = (unsigned*) dst;
	}
	//return (CMD_SUCCESS);

	for (i = 0; i < no_of_bytes; i++) {
		flash_buf[(byte_ctr + i)] = *(src + i);
	}
	byte_ctr = byte_ctr + no_of_bytes;

	if (byte_ctr == FLASH_BUF_SIZE) {
		/* We have accumulated enough bytes to trigger a flash write */
		find_erase_prepare_sector(SystemCoreClock / 1000,
				(unsigned) flash_address);
		if (result_table[0] != CMD_SUCCESS)
			return result_table[0];

		write_data(SystemCoreClock / 1000, (unsigned) flash_address,
				(unsigned*) flash_buf, FLASH_BUF_SIZE);
		if (result_table[0] != CMD_SUCCESS)
			return result_table[0];

		/* Reset byte counter and flash address */
		byte_ctr = 0;
		flash_address = 0;
	}
	return (CMD_SUCCESS);
}

void find_erase_prepare_sector(unsigned cclk, unsigned flash_address) {
	unsigned i;

	__disable_irq();
	for (i = USER_START_SECTOR; i <= MAX_USER_SECTOR; i++) {
		if (flash_address < SECTOR_END(i)) {
			if (flash_address == SECTOR_START(i)) {
				prepare_sector(i, i, cclk);
				erase_sector(i, i, cclk);
			}
			prepare_sector(i, i, cclk);
			break;
		}
	}
	__enable_irq();
}

void write_data(unsigned cclk, unsigned flash_address, unsigned *flash_data_buf,
		unsigned count) {
	__disable_irq();
	param_table[0] = COPY_RAM_TO_FLASH;
	param_table[1] = flash_address;
	param_table[2] = (unsigned) flash_data_buf;
	param_table[3] = count;
	param_table[4] = cclk;
	iap_entry(param_table, result_table);
	__enable_irq();
}

void erase_sector(unsigned start_sector, unsigned end_sector, unsigned cclk) {
	param_table[0] = ERASE_SECTOR;
	param_table[1] = start_sector;
	param_table[2] = end_sector;
	param_table[3] = cclk;
	iap_entry(param_table, result_table);
}

void prepare_sector(unsigned start_sector, unsigned end_sector, unsigned cclk) {
	param_table[0] = PREPARE_SECTOR_FOR_WRITE;
	param_table[1] = start_sector;
	param_table[2] = end_sector;
	param_table[3] = cclk;
	iap_entry(param_table, result_table);
}

void compare_mem(void *flash_add, void *ram_add, int count) {
	command[0] = COMPARE;
	command[1] = (unsigned int) flash_add;
	command[2] = (unsigned int) ram_add;
	command[3] = count;
	command[4] = 0x30e58e; //SystemCoreClock / 1000;
	iap_entry(command, result_table);
}
/*
 void test_flash(unsigned sectorNumber) {
 prepare_sector(sectorNumber, sectorNumber, 14748);
 if (result_table[0] != CMD_SUCCESS)
 return;

 erase_sector(sectorNumber, sectorNumber, 14748);
 if (result_table[0] != CMD_SUCCESS)
 return;

 prepare_sector(sectorNumber, sectorNumber, 14748);
 if (result_table[0] != CMD_SUCCESS)
 return;

 for (int i = 0; i < 4096; i++) {
 dataArr[i] = i;
 }
 write_flash(0x00008000, &dataArr[0], 4096);
 if (result_table[0] != CMD_SUCCESS)
 return;

 compare_mem(0x00008000, &dataArr[0], 4096);
 if (result_table[0] != CMD_SUCCESS)
 return;
 }
 */

int user_code_present(void) {
	param_table[0] = BLANK_CHECK_SECTOR;
	param_table[1] = USER_START_SECTOR;
	param_table[2] = USER_START_SECTOR;
	iap_entry(param_table, result_table);
	if (result_table[0] == CMD_SUCCESS) {

		return (false);
	}

	return (true);
}

/*********************************************************************//**
 * @brief
 *
 * @return	None
 **********************************************************************/
void jump_to_app_section() {
	uint32_t *jump_code;
	static void (*farewellBootloader)(void) = 0;
	uint8_t tmp;

	__disable_irq();

	NVIC->ICER[0] = 0xFFFFFFFF; //Disable all interrupts
	NVIC->ICER[1] = 0x00000001;

	NVIC->ICPR[0] = 0xFFFFFFFF; //Clear all pending interrupts
	NVIC->ICPR[1] = 0x00000001;
	for (tmp = 0; tmp < 32; tmp++) // Clear all interrupt priority
			{
		NVIC->IP[tmp] = 0x00;
	}

	/*	LPC_SC->CCLKSEL = 0x01; // set sysclk (12MHz) as clock source
	 LPC_SC->PLL0CON = 0; // disable PLL
	 LPC_SC->PLL0FEED = 0xAA;
	 LPC_SC->PLL0FEED = 0x55;
	 */
	jump_code = 0x10004;

	int APP_SECTION_ADDR = *jump_code;
	SCB->VTOR = APP_SECTION_ADDR & 0x1FFFFF80;

	__set_CONTROL(0); // Change from PSP to MSP

	uint32_t app_start_address = *(uint32_t*) (jump_code);
	farewellBootloader = (void (*)(void)) app_start_address;

	__enable_irq();
	__ASM volatile ("mov r3, 0x10000" : : : );
	__ASM volatile ("ldr sp, [r3]" : : : );
	__ASM volatile ("mov r2, 0x4" : : : );
	__ASM volatile ("add r3, r2" : : : );
	__ASM volatile ("ldr r2, [r3]" : : : );
	//__ASM volatile ("blx r2" : : : ); // запуск как подпрограммы
	__ASM volatile ("ldr pc, [r3]" : : : ); // прямое указание счетчика программ

	//farewellBootloader();	// Jump to the application.
	//NVIC_SystemReset();
}

/**
 * @brief	main routine for example_lwip_tcpecho_freertos_17xx40xx
 * @return	Function should not exit
 */
#define run_boot         (*((volatile uint32_t *) 0x20007000))
uint32_t EEPROM_buffer[EEPROM_PAGE_SIZE / sizeof(uint32_t)];

int main(void) {
	uint32_t physts;
	ip_addr_t ipaddr, netmask, gw;
	static int prt_ip = 0;

	prvSetupHardware();

	Chip_IOCON_PinMuxSet(LPC_IOCON, 3, 6, IOCON_FUNC0); // LED_POWER
	Chip_IOCON_PinMuxSet(LPC_IOCON, 3, 7, IOCON_FUNC0); // LED_ERROR
	Chip_GPIO_WriteDirBit(LPC_GPIO, 3, 6, true);
	Chip_GPIO_WritePortBit(LPC_GPIO, 3, 6, 0); //LED_POWER
	Chip_GPIO_WriteDirBit(LPC_GPIO, 3, 7, true);
	Chip_GPIO_WritePortBit(LPC_GPIO, 3, 7, 0); //LED_ERROR

	// перед запуском task сервера нужно сбросить ethernet switch
	Chip_GPIO_WriteDirBit(LPC_GPIO, 1, 18, true); //  ENET_reset
	Chip_GPIO_WritePortBit(LPC_GPIO, 1, 18, 0);

	msDelay(100);
	Chip_GPIO_WritePortBit(LPC_GPIO, 1, 18, 1);
	msDelay(500);

#define PAGE_ADDR       0x01/* Page number */
#define EEPROM_PAGE_SIZE                64		/*!< EEPROM byes per page */
	uint8_t *ptr = (uint8_t*) EEPROM_buffer;
	/* Init EEPROM */
	Chip_EEPROM_Init(LPC_EEPROM);
	/* Read all data from EEPROM page */
	Chip_EEPROM_Read(LPC_EEPROM, 0, PAGE_ADDR, ptr, EEPROM_RWSIZE_8BITS, EEPROM_PAGE_SIZE);

#if 0
  	// setup Default settings button
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 13, false);

	if (Chip_GPIO_GetPinState(LPC_GPIO, 2, 13)) {
		jump_to_app_section();
	} else {

	}
#endif
#if 1
  	// setup Default settings button
	// setup Default settings button
	Chip_IOCON_PinMuxSet(LPC_IOCON, 2, 13, IOCON_FUNC0 | IOCON_MODE_PULLUP);
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 13, false);
	msDelay(200);
  	// setup Default settings button
	if (Chip_GPIO_GetPinState(LPC_GPIO, 2, 13)) {
		jump_to_app_section();
	} else {

	}
#endif

	prepare_sector(USER_START_SECTOR, MAX_USER_SECTOR, 14748);
	if (result_table[0] != CMD_SUCCESS) {
		DEBUGSTR("Error prepare_sector...\r\n");
	}

	erase_sector(USER_START_SECTOR, MAX_USER_SECTOR, 14748);
	if (result_table[0] != CMD_SUCCESS) {
		DEBUGSTR("Error erase_sector...\r\n");
	}

	prepare_sector(USER_START_SECTOR, MAX_USER_SECTOR, 14748);
	if (result_table[0] != CMD_SUCCESS) {
		DEBUGSTR("Error prepare_sector...\r\n");
	}

	/* Initialize LWIP */
	lwip_init();

	LWIP_DEBUGF(LWIP_DBG_ON, ("Starting LWIP WEB server...\n"));

	/* Static IP assignment */
#if LWIP_DHCP
	IP4_ADDR(&gw, 0, 0, 0, 0);
	IP4_ADDR(&ipaddr, 0, 0, 0, 0);
	IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
	IP4_ADDR(&gw, 192, 168, 0, 1);
	IP4_ADDR(&ipaddr, 192, 168, 0, 101);
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

	//Chip_GPIO_WritePortBit(LPC_GPIO, 3, 6, 1); //LED_POWER

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
				//Board_LED_Set(0, true);
				prt_ip = 0;
				//Chip_GPIO_WritePortBit(LPC_GPIO, 3, 7, 1); //LED_ERROR

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
				//Board_LED_Set(0, false);
				//Board_LED_Set(1, false);
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
				//Board_LED_Set(1, true);
			}
		}
	}

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
