/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "asm_func.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define E1_ARR_SIZE 20U
#define E2_ARR_SIZE 20U
#define E3_ARR_SIZE 20U
#define E4_ARR_SIZE 20U
#define E5_ARR_SIZE 20U
#define E6_ARR_SIZE 20U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/**
 * @fn void zeros(uint32_t*, uint32_t)
 * @brief Ejercicio 1 en C - Declaración de función
 *
 * @param vector
 * @param longitud
 */
void zeros(uint32_t *vector, uint32_t longitud);

/**
 * @fn void productoEscalar32(uint32_t*, uint32_t*, uint32_t, uint32_t)
 * @brief Ejercicio 2 en C - Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitud
 * @param escalar
 */
void productoEscalar32(uint32_t *vectorIn, uint32_t *vectorOut,
		uint32_t longitud, uint32_t escalar);

/**
 * @fn void productoEscalar16(uint16_t*, uint16_t*, uint32_t, uint16_t)
 * @brief Ejercicio 3 en C - Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitud
 * @param escalar
 */
void productoEscalar16(uint16_t *vectorIn, uint16_t *vectorOut,
		uint32_t longitud, uint16_t escalar);

/**
 * @fn void productoEscalar12(uint16_t*, uint16_t*, uint32_t, uint16_t)
 * @brief Ejercicio 4 en C- Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitud
 * @param escalar
 */
void productoEscalar12(uint16_t *vectorIn, uint16_t *vectorOut,
		uint32_t longitud, uint16_t escalar);

/**
 * @fn void filtroVentana10(uint16_t*, uint16_t*, uint32_t)
 * @brief Ejercicio 5 en C- Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitudVectorIn
 */
void filtroVentana10(uint16_t *vectorIn, uint16_t *vectorOut,
		uint32_t longitudVectorIn);

/**
 * @fn void pack32to16(int32_t*, int16_t*, uint32_t)
 * @brief Ejercicio 6 en C - Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitud
 */
void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void PrivilegiosSVC(void) {
	// Obtiene valor del registro de 32 bits del procesador llamado "control".
	// El registro guarda los siguientes estados:
	// bit 2: Uso de FPU en el contexto actual. Usado=1, no usado=0.
	// bit 1: Mapeo del stack pointer(sp). MSP=0, PSP=1.
	// bit 0: Modo de ejecucion en Thread. Privilegiado=0, No privilegiado=1.
	//        Recordar que este valor solo se usa en modo Thread. Las
	//        interrupciones siempre se ejecutan en modo Handler con total
	//        privilegio.
	uint32_t x = __get_CONTROL();

	// Actividad de debug: Ver registro "control" y valor de variable "x".
	//__BKPT (0);

	x |= 1;
	// bit 0 a modo No privilegiado.
	__set_CONTROL(x);

	// En este punto se estaria ejecutando en modo No privilegiado.
	// Lectura del registro "control" para confirmar.
	x = __get_CONTROL();

	// Actividad de debug: Ver registro "control" y valor de variable "x".
	//__BKPT (0);

	x &= ~1u;
	// Se intenta volver a modo Privilegiado (bit 0, valor 0).
	__set_CONTROL(x);

	// Confirma que esta operacion es ignorada por estar ejecutandose en modo
	// Thread no privilegiado.
	x = __get_CONTROL();

	// Actividad de debug: Ver registro "control" y valor de variable "x".
	//__BKPT (0);

	// En este punto, ejecutando en modo Thread no privilegiado, la unica forma
	// de volver a modo privilegiado o de realizar cualquier cambio que requiera
	// modo privilegiado, es pidiendo ese servicio a un hipotetico sistema
	// opertivo de tiempo real.
	// Para esto se invoca por software a la interrupcion SVC (Supervisor Call)
	// utilizando la instruccion "svc".
	// No hay intrinsics para realizar esta tarea. Para utilizar la instruccion
	// es necesario implementar una funcion en assembler. Ver el archivo
	// asm_func.S.
	asm_svc();

	// El sistema operativo (el handler de SVC) deberia haber devuelto el modo
	// de ejecucion de Thread a privilegiado (bit 0 en valor 0).
	x = __get_CONTROL();

	// Fin del ejemplo de SVC
}

/*---------- Funciones para resolución de ejercicios en C ----------*/

// Ejercicio 1 en C - Definición de función
void zeros(uint32_t *vector, uint32_t longitud) {

	memset(vector, 0, longitud * sizeof(*vector));
}

// Ejercicio 2 en C - Definición de función
void productoEscalar32(uint32_t *vectorIn, uint32_t *vectorOut,
		uint32_t longitud, uint32_t escalar) {

	/** Chequea que los punteros estén inicializados
	 */
	if (vectorIn != NULL && vectorOut != NULL) {

		/** Multiplica un arreglo por un escalar y guarda el
		 * resultado en otro arreglo de igual tamaño
		 * */
		for (int32_t i = 0; i < longitud; i++) {

			vectorOut[i] = vectorIn[i] * escalar;
		}
	}
}

// Ejercicio 3 en C - Definición de función
void productoEscalar16(uint16_t *vectorIn, uint16_t *vectorOut,
		uint32_t longitud, uint16_t escalar) {

	/** Chequea que los punteros estén inicializados
	 */
	if (vectorIn != NULL && vectorOut != NULL) {

		/** Multiplica un arreglo por un escalar y guarda el
		 * resultado en otro arreglo de igual tamaño
		 * */
		for (int32_t i = 0; i < longitud; i++) {
			vectorOut[i] = vectorIn[i] * escalar;
		}
	}
}

// Ejercicio 4 en C - Definición de función
/**
 * @def MAX_UINT12
 * @brief Mayor valor que entra en un número de 12 bits
 */
#define MAX_UINT12 0x0FFF

void productoEscalar12(uint16_t *vectorIn, uint16_t *vectorOut,
		uint32_t longitud, uint16_t escalar) {

	/** Chequea que los punteros estén inicializados
	 */
	if (vectorIn != NULL && vectorOut != NULL) {

		/** Valor a partir del cual se produce desborde al multiplicar
		 *  por "escalar"
		 */
		uint16_t max_mult = 0;

		if (escalar != 0) {
			/** Cualquier número multiplicado por un valor mayor que
			 * max_mult va a producir un desborde
			 */
			max_mult = MAX_UINT12 / escalar;

			for (int32_t i = 0; i < longitud; i++) {

				/** Si se  el valor en vectorIn[i] producirá desborde,
				 *  pone como resultado el valor máximo en 12 bits (saturación)
				 */
				vectorOut[i] =
						vectorIn[i] < max_mult ?
								vectorIn[i] * escalar : MAX_UINT12;
			}
		}
	}
}

// Ejercicio 5 en C en C - Definición de función
void filtroVentana10(uint16_t *vectorIn, uint16_t *vectorOut,
		uint32_t longitudVectorIn) {

	uint16_t acumulador = 0;

	/** Parametrizar el tamaño de ventana permite hacer pruebas
	 * con ventanas de pequeños tamaños
	 */
	uint8_t tam_ventana = 10;

	/** Chequea que los punteros estén inicializados
	 */
	if (vectorIn != NULL && vectorOut != NULL) {

		/** La ventana se va solapando de a un elemento con el vector de
		 *  entrada, comenzando de la siguiente manera:
		 *  i == 0 --> promedio = vectorIn[0] / 10
		 *  i == 1 --> promedio = (vectorIn[0] + vectorIn[1]) / 10
		 *  i == 2 --> promedio = (vectorIn[0] + vectorIn[1] + vectorIn[2]) / 10
		 *
		 *  Cuando el índice del último elemento es mayor al tamaño de ventana,
		 *  cada paso que se avanza se debe restar el elemento de menor índice,
		 *  que queda fuera de la ventana
		 *
		 *  El proceso se detiene cuando el índice de la ventana llega al tamaño
		 *  del vector (la ventana no está centrada)
				 */
		for (uint32_t i = 0; i < longitudVectorIn; i++) {

			acumulador += vectorIn[i];	// Agregar una nueva muestra

			if (i >= tam_ventana) {						// A partir del tamaño de ventana
				acumulador -= vectorIn[i - tam_ventana];// al avanzar resta la muestra que sobra
			}

			vectorOut[i] = acumulador / tam_ventana;// Guarda el promedio actualizado
		}
	}
}

// Ejercicio 6 en C en C - Definición de función
void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud){
	for(uint32_t i = 0; i < longitud; i++){
		vectorOut[i] = (int16_t)vectorIn[i];
	}
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_ETH_Init();
	MX_USART3_UART_Init();
	MX_USB_OTG_FS_PCD_Init();
	/* USER CODE BEGIN 2 */
	PrivilegiosSVC();

	// Función en Assembly de ejemplo, comentada para eliminar WARNING
	// const uint32_t Resultado = asm_sum(5, 3);

	/** Aplicaciones para comprobar el correcto funcionamiento de
	 *  las funciones desarrolladas
	 *
	 *  CÓDIGO EN C - FUNCIONES EN C
	 *
	 */

	/*---------- INICIO Ejercicio 1 en C ----------*/

	/** Valores de prueba para chequear que sean borrados
	 */
	uint32_t ej1_c_vector32[] = { 15, 99, 0xFFFFFFFF, 0xDEADBEEF };

	uint32_t tam_vec_ej1_c = sizeof(ej1_c_vector32) / sizeof(*ej1_c_vector32);

	zeros(ej1_c_vector32, tam_vec_ej1_c);

	/*------------ FIN Ejercicio 1 en C -----------*/

	/*---------- INICIO Ejercicio 2 en C ----------*/

	/** Valores de prueba para chequear el reslutado del producto
	 */
	uint32_t ej2_c_vector32In[] = { 5, 9, 25, 37, 1 };

	uint32_t tam_vec_ej2_c = sizeof(ej2_c_vector32In) / sizeof(*ej2_c_vector32In);

	uint32_t ej2_c_vector32Out[tam_vec_ej2_c];

	productoEscalar32(ej2_c_vector32In, ej2_c_vector32Out, tam_vec_ej2_c, 7);

	/*------------ FIN Ejercicio 2 en C -----------*/

	/*---------- INICIO Ejercicio 3 en C ----------*/

	/** Valores de prueba para chequear el reslutado del producto
	 */
	uint16_t ej3_c_vector16In[] = { 5, 9, 25, 37, 1 };
	uint32_t tam_vec_ej3_c = sizeof(ej3_c_vector16In)
			/ sizeof(*ej3_c_vector16In);

	uint16_t ej3_c_vector16Out[tam_vec_ej3_c];

	productoEscalar16(ej3_c_vector16In, ej3_c_vector16Out, tam_vec_ej3_c, 3);

	/*------------ FIN Ejercicio 3 en C -----------*/

	/*---------- INICIO Ejercicio 4 en C ----------*/

	/** Valores de prueba para chequear el reslutado del producto
	 *  y si se producen desbordes
	 */
	uint16_t ej4_c_vector16In[] = { 0xFF0, 9, 25, 37, 1 };

	uint32_t tam_vec_ej4_c = sizeof(ej4_c_vector16In)
			/ sizeof(*ej4_c_vector16In);

	uint16_t ej4_c_vector16Out[tam_vec_ej4_c];

	productoEscalar12(ej4_c_vector16In, ej4_c_vector16Out, tam_vec_ej4_c, 32);

	/*------------ FIN Ejercicio 4 en C -----------*/

	/*---------- INICIO Ejercicio 5 en C ----------*/

#define VEC_SIZE_EJ5 14U
	uint16_t ej5_c_vector16In[VEC_SIZE_EJ5] = { 2, 10, 10, 2, 2, 0, 0, 20, 20,
			2, 2, 2, 2, 2 };
	uint16_t ej5_c_vector16Out[VEC_SIZE_EJ5] = { 0 };

	filtroVentana10(ej5_c_vector16In, ej5_c_vector16Out, VEC_SIZE_EJ5);

	/*------------ FIN Ejercicio 5 en C -----------*/

	/*---------- INICIO Ejercicio 6 en C ----------*/
	int32_t ej6_c_vector32In[VEC_SIZE_EJ5] = { -1, 500000000, -500000000,
			65534, 65535, 65536, -65534, -65535, -65536, 36868, 36863, 36864, -36863, -36864 };
	int16_t ej6_c_vector16Out[VEC_SIZE_EJ5] = { 0 };


	pack32to16(ej6_c_vector32In, ej6_c_vector16Out, VEC_SIZE_EJ5);

	/*------------ FIN Ejercicio 6 en C -----------*/

	/** Aplicaciones para comprobar el correcto funcionamiento de
	 *  las funciones desarrolladas
	 *
	 *  CÓDIGO EN C - FUNCIONES EN ASSEMBLY
	 *
	 */

	/*---------- INICIO Ejercicio 1 en Assembly ----------*/

	/** Valores de prueba para chequear que sean borrados
	 */
	uint32_t ej1_asm_vector32[] = { 15, 99, 0xFFFFFFFF, 0xDEADBEEF };

	uint32_t tam_vec_ej1_asm = sizeof(ej1_asm_vector32)
			/ sizeof(*ej1_asm_vector32);

	zeros(ej1_asm_vector32, tam_vec_ej1_asm);

	/*------------ FIN Ejercicio 1 en Assembly -----------*/

	/*---------- INICIO Ejercicio 2 en Assembly ----------*/
	/** Valores de prueba para chequear el reslutado del producto
	 */
	uint32_t ej2_asm_vector32In[] = { 35, 9, 7, 8 };
	uint32_t ej2_asm_vector32Out[] = { 35, 9, 7, 8 };

	asm_zeros(ej2_asm_vector32Out, 4);

	/** Se multiplica por 5 para comprobar fácilmente los resultados
	 */
	asm_productoEscalar32(ej2_asm_vector32In, ej2_asm_vector32Out, 4, 5);

	/*------------ FIN Ejercicio 2 en Assembly -----------*/

	/*---------- INICIO Ejercicio 3 en Assembly ----------*/

	/** Valores de prueba para chequear el reslutado del producto
	 *  y si se produce desborde
	 */
	uint16_t ej3_asm_vector16In[] = { 0xFFF0, 0xFFFF, 7, 8 };
	uint16_t ej3_asm_vector16Out[] = { 35, 9, 7, 8 };

	/** Se multiplica por 2 para comprobar fácilmente si hay desborde
	 */
	asm_productoEscalar16(ej3_asm_vector16In, ej3_asm_vector16Out, 4, 2);

	/*------------ FIN Ejercicio 3 en Assembly -----------*/

	/*---------- INICIO Ejercicio 4 en Assembly ----------*/

	/** Valores de prueba para chequear el reslutado del producto
	 *  y si satura al producirse desborde
	 */
	uint16_t ej4_asm_vector16In[4] = { 0xFFF0, 0xFFFF, 7, 8 };
	uint16_t ej4_asm_vector16Out[4] = { 0 };

	asm_productoEscalar12(ej4_asm_vector16In, ej4_asm_vector16Out, 4, 2);

	/*------------ FIN Ejercicio 4 en Assembly -----------*/

	/*---------- INICIO Ejercicio 5 en Assembly ----------*/

	uint16_t ej5_asm_vector16In[VEC_SIZE_EJ5] = { 2, 10, 10, 2, 2, 0, 0, 20, 20,
			2, 2, 2, 2, 2 };
	uint16_t ej5_asm_vector16Out[VEC_SIZE_EJ5] = { 0 };

	asm_filtroVentana10(ej5_asm_vector16In, ej5_asm_vector16Out, VEC_SIZE_EJ5);

	/*------------ FIN Ejercicio 5 en Assembly -----------*/

	/*---------- INICIO Ejercicio 6 en Assembly ----------*/
	int32_t ej6_asm_vector32In[VEC_SIZE_EJ5] = { -1, 500000000, -500000000, 65534,
			65535, 65536, -65534, -65535, -65536, 36868, 36863, 36864, -36863,
			-36864 };
	int16_t ej6_asm_vector16Out[VEC_SIZE_EJ5] = { 0 };

	pack32to16(ej6_asm_vector32In, ej6_asm_vector16Out, VEC_SIZE_EJ5);

	/*------------ FIN Ejercicio 6 en Assembly -----------*/

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ETH Initialization Function
 * @param None
 * @retval None
 */
static void MX_ETH_Init(void) {

	/* USER CODE BEGIN ETH_Init 0 */

	/* USER CODE END ETH_Init 0 */

	static uint8_t MACAddr[6];

	/* USER CODE BEGIN ETH_Init 1 */

	/* USER CODE END ETH_Init 1 */
	heth.Instance = ETH;
	MACAddr[0] = 0x00;
	MACAddr[1] = 0x80;
	MACAddr[2] = 0xE1;
	MACAddr[3] = 0x00;
	MACAddr[4] = 0x00;
	MACAddr[5] = 0x00;
	heth.Init.MACAddr = &MACAddr[0];
	heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
	heth.Init.TxDesc = DMATxDscrTab;
	heth.Init.RxDesc = DMARxDscrTab;
	heth.Init.RxBuffLen = 1524;

	/* USER CODE BEGIN MACADDRESS */

	/* USER CODE END MACADDRESS */

	if (HAL_ETH_Init(&heth) != HAL_OK) {
		Error_Handler();
	}

	memset(&TxConfig, 0, sizeof(ETH_TxPacketConfig));
	TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM
			| ETH_TX_PACKETS_FEATURES_CRCPAD;
	TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
	TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
	/* USER CODE BEGIN ETH_Init 2 */

	/* USER CODE END ETH_Init 2 */

}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void) {

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

/**
 * @brief USB_OTG_FS Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_OTG_FS_PCD_Init(void) {

	/* USER CODE BEGIN USB_OTG_FS_Init 0 */

	/* USER CODE END USB_OTG_FS_Init 0 */

	/* USER CODE BEGIN USB_OTG_FS_Init 1 */

	/* USER CODE END USB_OTG_FS_Init 1 */
	hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
	hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
	hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
	hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
	hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
	hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
	hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
	hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
	if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USB_OTG_FS_Init 2 */

	/* USER CODE END USB_OTG_FS_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, LD1_Pin | LD3_Pin | LD2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin : USER_Btn_Pin */
	GPIO_InitStruct.Pin = USER_Btn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
	GPIO_InitStruct.Pin = LD1_Pin | LD3_Pin | LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : USB_PowerSwitchOn_Pin */
	GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : USB_OverCurrent_Pin */
	GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
