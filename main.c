#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "MFRC522.h"
#include "esp_log.h"
static const char* TAG = "RC522";
uint8_t buffer_to_write[16] = {0x10,0x11,0x12,0x13,0x10,0x11,0x12,0x13,0x10,0x11,0x12,0x13,0x10,0x11,0x12,0x13};
char username[16] = {0};
char password[16] = {0};
uint8_t card_rx_buffer[20];
uint8_t card_rx_len = 20;
MIFARE_Key key = 
{
 .keyByte = {0xff,0xff,0xff,0xff,0xff,0xff}
};
uint8_t req_buffer[16];
uint8_t req_len = 16;
uint8_t status = 0;
void app_main(void)
{
     esp_err_t ret;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg={
        .miso_io_num=3,
        .mosi_io_num=4,
        .sclk_io_num=5,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1
    };
    spi_device_interface_config_t devcfg={
        .clock_speed_hz=5000000,               //Clock out at 5 MHz
        .mode=0,                                //SPI mode 0
        .spics_io_num=8,               //CS pin
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        //.pre_cb=ili_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
    };
    //Initialize the SPI bus
    ret=spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    assert(ret==ESP_OK);
    //Attach the RFID to the SPI bus
    ret=spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    assert(ret==ESP_OK);
    // PICC_HaltA(spi);
    // PCD_StopCrypto1(spi);
    PCD_Init(spi);
    while(1)
    {
        if(PICC_IsNewCardPresent(spi))                   //Checking for new card
    	{

            printf("***card detected!***\n");
    		GetStatusCodeName(PICC_Select(spi,&uid,0));
    		PICC_DumpToSerial(spi,&uid);                  //DETAILS OF UID ALONG WITH SECTORS
            
            // GetStatusCodeName(PICC_RequestA(spi,req_buffer,&req_len));
            
            
            //   GetStatusCodeName(PICC_Select(spi,&uid,0));
            //   GetStatusCodeName(PCD_Authenticate(spi,PICC_CMD_MF_AUTH_KEY_A,5,&key, &(uid)));
            //   GetStatusCodeName(MIFARE_Write(spi,4,(uint8_t*)username,16));
            //    GetStatusCodeName(MIFARE_Write(spi,5,(uint8_t*)password,16));
            // //  MIFARE_Read(spi,4,card_rx_buffer,&card_rx_len);
            //   PCD_StopCrypto1(spi);
            // ESP_LOGI(TAG,"MIFARE block %d : %s",4,(char*)card_rx_buffer);
    		vTaskDelay(100 / portTICK_PERIOD_MS);

    	}
    	vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
