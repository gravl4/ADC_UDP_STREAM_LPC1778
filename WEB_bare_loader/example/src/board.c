/*
 * @brief
 */

#include "board.h"
#include "string.h"

#include "retarget.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define BUTTONS_BUTTON1_GPIO_PORT_NUM           2
#define BUTTONS_BUTTON1_GPIO_BIT_NUM            10

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* System oscillator rate and RTC oscillator rate */
const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Very simple (inaccurate) delay function */
static void delay(uint32_t i)
{
	while (i--) {}
}

/* Delay in miliseconds  (cclk = 120MHz) */
static void delayMs(uint32_t ms)
{
	delay(ms * 40000);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize UART pins */
void Board_UART_Init(LPC_USART_T *pUART)
{
	if (pUART == LPC_UART0) {
		/*
		 * Initialize UART0 pin connect
		 * P0.2: TXD
		 * P0.3: RXD
		 */
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 2, (IOCON_FUNC1 | IOCON_MODE_INACT));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 3, (IOCON_FUNC1 | IOCON_MODE_INACT));
	}
	else if (pUART == LPC_UART2) {
		/* Initialize UART2 pin connect */
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 10, (IOCON_FUNC1 | IOCON_MODE_INACT));
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 11, (IOCON_FUNC1 | IOCON_MODE_INACT));
	}
}

/* Initialize debug output via UART for board */
void Board_Debug_Init(void)
{
#if defined(DEBUG_UART)
	Board_UART_Init(DEBUG_UART);

	Chip_UART_Init(DEBUG_UART);
	Chip_UART_SetBaud(DEBUG_UART, 115200);
	Chip_UART_ConfigData(DEBUG_UART, UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS);

	/* Enable UART Transmit */
	Chip_UART_TXEnable(DEBUG_UART);
#endif
}

/* Sends a character on the UART */
void Board_UARTPutChar(char ch)
{
#if defined(DEBUG_UART)
	while ((Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_THRE) == 0) {}
	Chip_UART_SendByte(DEBUG_UART, (uint8_t) ch);
#endif
}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(void)
{
#if defined(DEBUG_UART)
	if (Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_RDR) {
		return (int) Chip_UART_ReadByte(DEBUG_UART);
	}
#endif
	return EOF;
}

/* Outputs a string on the debug UART */
void Board_UARTPutSTR(char *str)
{
#if defined(DEBUG_UART)
	while (*str != '\0') {
		Board_UARTPutChar(*str++);
	}
#endif
}

#define MAXLEDS 2
static const uint8_t ledports[MAXLEDS] = {4, 4};	//{2, 2};
static const uint8_t ledpins[MAXLEDS] = {30, 31};	//{26, 27};

/* Initializes board LED(s) */
static void Board_LED_Init(void)
{
	int i;

	/* Setup port direction and initial output state */
	for (i = 0; i < MAXLEDS; i++) {
		Chip_GPIO_WriteDirBit(LPC_GPIO, ledports[i], ledpins[i], true);
		Chip_GPIO_WritePortBit(LPC_GPIO, ledports[i], ledpins[i], true);
	}
}

/* Sets the state of a board LED to on or off */
void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber < MAXLEDS) {
		/* Set state, low is on, high is off */
		Chip_GPIO_SetPinState(LPC_GPIO, ledports[LEDNumber], ledpins[LEDNumber], !On);
	}
}

/* Returns the current state of a board LED */
bool Board_LED_Test(uint8_t LEDNumber)
{
	bool state = false;

	if (LEDNumber < MAXLEDS) {
		state = Chip_GPIO_GetPinState(LPC_GPIO, ledports[LEDNumber], ledpins[LEDNumber]);
	}

	/* These LEDs are reverse logic. */
	return !state;
}

/* Toggles the current state of a board LED */
void Board_LED_Toggle(uint8_t LEDNumber)
{
	if (LEDNumber < MAXLEDS) {
		Chip_GPIO_SetPinToggle(LPC_GPIO, ledports[LEDNumber], ledpins[LEDNumber]);
	}
}

/* Returns the MAC address assigned to this board */
void Board_ENET_GetMacADDR(uint8_t *mcaddr)
{
	const uint8_t boardmac[] = {0x00, 0x60, 0x37, 0x12, 0x34, 0x56};

	memcpy(mcaddr, boardmac, 6);
}

/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* Sets up DEBUG UART */
	DEBUGINIT();

	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	/* Initialize LEDs */
	Board_LED_Init();
}


void Board_Buttons_Init(void)
{
	Chip_GPIO_WriteDirBit(LPC_GPIO, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM, false);
}

uint32_t Buttons_GetStatus(void)
{
	uint8_t ret = NO_BUTTON_PRESSED;
	if (Chip_GPIO_ReadPortBit(LPC_GPIO, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM) == 0x00) {
		ret |= BUTTONS_BUTTON1;
	}
	return ret;
}


