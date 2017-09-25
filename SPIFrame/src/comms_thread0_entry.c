#include "comms_thread0.h"
#include<stdio.h>

void comms_thread0_entry(void);

uint8_t src[]= "Hello World\r\n";
uint8_t open[] = "Open SPI Failed\r\n";
uint8_t openfail[] = "Open SPI Success\r\n";

uint8_t write[] = "SPI  Write success\r\n";
uint8_t writefailed[] = "SPI write failed\r\n";

uint8_t read[] = "SPI  Read success\r\n";
uint8_t readfailed[] = "SPI read failed\r\n";

uint8_t lock[] = "SPI  lock success\r\n";
uint8_t lockfailed[] = "SPI lock failed\r\n";

uint8_t unlock[] = "SPI  unlock success\r\n";
uint8_t unlockfailed[] = "SPI unlock failed\r\n";


uint8_t address = 0x76;
uint8_t data[1];

ssp_err_t error;

volatile bool transmitComplete=false;
void user_spi_callback(spi_callback_args_t * p_args)
{
// Get Event Type
switch (p_args->event)
{
// Transmission Complete
    case SPI_EVENT_TRANSFER_COMPLETE:
        transmitComplete = true;
        break;
    default:
        break;


}
}


/* Comms Thread entry function */
void comms_thread0_entry(void)
{
    /* TODO: add your own code here */

    error = g_sf_spi_device0.p_api->open(g_sf_spi_device0.p_ctrl,g_sf_spi_device0.p_cfg);

        if(error != SSP_SUCCESS){

            tx_thread_sleep(100);
            g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,open,sizeof(open)-1,TX_NO_WAIT);

        }
        else{

            tx_thread_sleep(100);
            g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,openfail,sizeof(openfail)-1,TX_NO_WAIT);

        }
     //  while(transmitComplete !=true);

        error = g_sf_spi_device0.p_api->lock(g_sf_spi_device0.p_ctrl);

        if(error != SSP_SUCCESS){

                 tx_thread_sleep(100);
                 g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,lockfailed,sizeof(lockfailed)-1,TX_NO_WAIT);

             }
             else{

                 tx_thread_sleep(100);
                 g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,lock,sizeof(lock)-1,TX_NO_WAIT);

             }


        error = g_sf_spi_device0.p_api->write(g_sf_spi_device0.p_ctrl,&address,1,SPI_BIT_WIDTH_8_BITS,TX_WAIT_FOREVER);

           if(error != SSP_SUCCESS){

//               tx_thread_sleep(100);
               g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,writefailed,sizeof(writefailed)-1,TX_NO_WAIT);

           }
           else{


//               tx_thread_sleep(100);
               g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,write,sizeof(write)-1,TX_NO_WAIT);
           }

           error = g_sf_spi_device0.p_api->read(g_sf_spi_device0.p_ctrl,data,1,SPI_BIT_WIDTH_8_BITS,TX_WAIT_FOREVER);

                      if(error != SSP_SUCCESS){

                        //  tx_thread_sleep(100);

                         g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,readfailed,sizeof(readfailed)-1,TX_NO_WAIT);
                          //g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,data,sizeof(data),TX_NO_WAIT);

                      }
                      else{


                        //  tx_thread_sleep(100);

                          g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,data,1,TX_NO_WAIT);
                      //    tx_thread_sleep(100);
                          g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,read,sizeof(read)-1,TX_NO_WAIT);

                      }
                      g_sf_spi_device0.p_api->unlock(g_sf_spi_device0.p_ctrl);

 /*   while (1)
    {
        tx_thread_sleep (1000);
        g_sf_comms0.p_api->write(g_sf_comms0.p_ctrl,src,sizeof(src)-1,TX_NO_WAIT);

    }*/
}
