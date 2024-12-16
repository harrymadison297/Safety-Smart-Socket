#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"

#include "sp_ade9153.h"
#include "ade9153a.h"

#define RX_SIZE (1500)
#define TX_SIZE (1460)

#define QUEUE_SEND_SIZE 200

/******************************************************************
 * GPIO Define
 */
#define CTRL_ISO_PIN_OUT 26
#define STATE_LED_PIN_OUT 14

/******************************************************************
 * Definition ADE9153
 */
/* ADE9153 configuration */
#define ADE9153_PIN_RESET 22

#define RSHUNT 0.002 // Ohm
#define PGAGAIN 16
#define RBIG 1000000 // Ohm
#define RSMALL 1000	 // Ohm

/* SPI with ADE9153 */
#define xSPI_HOST VSPI_HOST
#define xSPI_CS -1
#define xSPI_CLK (1 * 1000 * 1000)

/* Ade9153 data */
RMSRegs_t RMSRegs;
PowRegs_t PowRegs;
PQRegs_t PQRegs;
ACALRegs_t ACALRegs;

typedef struct
{
	char *pNameDevice;
	char *version;
	uint8_t error;
	uint32_t sampleMSec;
	uint32_t sampleSecCalib;
	uint8_t voltSecCalib;
	uint8_t curtSecCalib;

	RMSRegs_t RMSValues;
	PowRegs_t PowValues;
	PQRegs_t PQValues;
	ACALRegs_t ACALValues;
} device_data_t;

static device_data_t devData;
uint16_t periodMeasure = 500;

void measurementADE(void *pvParameter)
{
	float vHeadRoom;

	/* compute targetXCC */
	RMSRegs.targetAICC = calculate_target_aicc(RSHUNT, PGAGAIN);
	printf("targetAICC: %f\n", RMSRegs.targetAICC);
	RMSRegs.targetAVCC = calculate_target_avcc(RBIG, RSMALL,
											   &vHeadRoom);
	printf("targetAVCC: %f\n", RMSRegs.targetAVCC);
	PowRegs.targetPowCC = calculate_target_powCC(RMSRegs.targetAICC,
												 RMSRegs.targetAVCC);
	printf("targetPowCC: %f\n", PowRegs.targetPowCC);
	while (true)
	{
		printf("\nADE Mesurement: \n");
		ADE9153_read_RMSRegs(&RMSRegs);
		ADE9153_read_PowRegs(&PowRegs);
		ADE9153_read_PQRegs(&PQRegs);
		printf("|- RMSRegs:\n");
		printf("|---- AIReg: %lx and AIValue: %f\n", RMSRegs.AIReg,
			   RMSRegs.AIValue);
		printf("|---- AVReg: %lx and AVValue: %f\n", RMSRegs.AVReg,
			   RMSRegs.AVValue);
		printf("|- PowRegs:\n");
		printf("|---- ActiveReg: %lx and activeValue: %f\n", PowRegs.activeReg,
			   PowRegs.activeValue);
		printf("|---- ReactiveReg: %lx and reactiveValue: %f\n",
			   PowRegs.reactiveReg, PowRegs.reactiveValue);
		printf("|---- ApparentReg: %lx and apparentValue: %f\n",
			   PowRegs.apparentReg, PowRegs.apparentValue);
		printf("|- PF compute: %f\n",
			   PowRegs.activeValue / PowRegs.apparentValue);
		printf("|- PQRegs:\n");
		printf("|---- pwFactorReg: %lx and pwFactorValue: %f\n",
			   PQRegs.pwFactorReg, PQRegs.pwFactorValue);
		printf("|---- periodReg: %lx and freqValue: %f\n", PQRegs.periodReg,
			   PQRegs.freqValue);
		printf("|---- angleAV_AIReg: %lx and angleAV_AIValue: %f\n",
			   PQRegs.angleAV_AIReg, PQRegs.angleAV_AIValue);
		devData.RMSValues = RMSRegs;
		devData.PowValues = PowRegs;
		devData.PQValues = PQRegs;
		vTaskDelay(periodMeasure / portTICK_PERIOD_MS);
	}
}

void esp_output_create(int pin)
{
	gpio_config_t pin_config = {
		1LL << pin,
		GPIO_MODE_OUTPUT,
		GPIO_PULLUP_DISABLE,
		GPIO_PULLDOWN_DISABLE,
		GPIO_INTR_DISABLE};
	gpio_config(&pin_config);
	gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

void app_main(void)
{

	esp_output_create(ADE9153_PIN_RESET);
	esp_output_create(PIN_VSPI_CS);
	esp_output_create(PIN_VSPI_CLK);
	esp_output_create(CTRL_ISO_PIN_OUT);

	gpio_set_level(ADE9153_PIN_RESET, 1);

	while (true)
	{
		gpio_set_level(CTRL_ISO_PIN_OUT, 1);
		printf("ON\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		gpio_set_level(CTRL_ISO_PIN_OUT, 0);
		printf("OFF\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	gpio_set_level(PIN_VSPI_CS, 0);
	gpio_set_level(PIN_VSPI_CLK, 1);

	vTaskDelay(100 / portTICK_PERIOD_MS);

	gpio_set_level(ADE9153_PIN_RESET, 0);

	bool checkSPI = false;
	do
	{
		/* Initiation SPI to ADE9153 */
		checkSPI = init_spiADE9153(xSPI_HOST, xSPI_CS, xSPI_CLK);
		if (!checkSPI)
		{
			printf("SPI config fail\n");
		}
		else
		{
			printf("SPI config success\n");
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	} while (!checkSPI);

	/* Start mesurement */
	measurementADE(NULL);
}
