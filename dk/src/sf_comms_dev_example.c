/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
 * display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
 * purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
 * SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
 * NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
 * INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
 * be subject to different terms.
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : sf_comms_dev_example.c
 * Description  :
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           TODO  1.00    Initial Release
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
 ***********************************************************************************************************************/
#include "dev_example_thread.h"
#include "dev_example_common.h"
#include "kermit/kermit.h"
#include "kermit/platform.h"
#include "fx_api.h"

/***********************************************************************************************************************
Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
 ***********************************************************************************************************************/

void dodebug(int fc, UCHAR * label, UCHAR * sval, long nval);
int devopen(char *device);
int pktmode(short on);
int devsettings(char * s);
int devrestore(void);
int devclose(void);
int inchk(struct k_data * k);
int readpkt(struct k_data * k, UCHAR *p, int len);
int tx_data(struct k_data * k, UCHAR *p, int n);
int openfile(struct k_data * k, UCHAR * s, int mode);
ULONG fileinfo(struct k_data * k, UCHAR * filename, UCHAR * buf,
int buflen, short * type, short mode);
void doexit(int status);

int readfile(struct k_data * k);
int writefile(struct k_data * k, UCHAR * s, int n);
int closefile(struct k_data * k, UCHAR c, int mode);

void sf_comms_txfr(sf_console_cb_args_t * p_args);
void sf_comms_recv(sf_console_cb_args_t * p_args);

int do_kermit(int action);
bool validate_file_list(char** file_list);

/***********************************************************************************************************************
Private global variables
 ***********************************************************************************************************************/

const sf_console_command_t g_sf_comms_commands[] =
{

 {
  (uint8_t*)"txFile",
  (uint8_t*)"Transmit files from board to host PC(using kermit)\r\n\tSynopsis:-txFile <file1> <file2>..<filen>"
          "\r\n\tExample:-txFile sample.txt",
  sf_comms_txfr,
  NULL
 },

 {
  (uint8_t*)"rxFile",
  (uint8_t*)"Receive a file from host PC to board (using kermit)",
  sf_comms_recv,
  NULL
 },
};

/** Previous menu */
extern const sf_console_menu_t g_sf_console_root_menu;


/** comms menu */
const sf_console_menu_t g_sf_comms_menu =
{
 .menu_prev = &g_sf_console_root_menu,
 .menu_name = (uint8_t*)"sf_el_ux_comms",
 .num_commands = (sizeof(g_sf_comms_commands)) / (sizeof(g_sf_comms_commands[0])),
 .command_list = g_sf_comms_commands

};

/** Input and output buffers */
extern UCHAR o_buf[];                   /* Must be defined in io.c */
extern UCHAR i_buf[];                   /* Must be defined in io.c */

/** Kermit data structure */
struct k_data k =
{
 .xfermode = 0,
 .remote = 1,
 .binary = 1,
 .parity = 'o',
 .bct = 1,
 .ikeep = 0,
 .filelist = NULL,
 .cancel = 0,

 /*  Fill in the i/o pointers  */

 .zinbuf = i_buf,           /* File input buffer */
 .zinlen = IBUFLEN,         /* File input buffer length */
 .zincnt = 0,           /* File input buffer position */
 .obuf = o_buf,         /* File output buffer */
 .obuflen = OBUFLEN,        /* File output buffer length */
 .obufpos = 0,          /* File output buffer position */

 /* Fill in function pointers */
 .rxd    = readpkt,         /* for reading packets */
 .txd    = tx_data,         /* for sending packets */
 .ixd    = inchk,           /* for checking connection */
 .openf  = openfile,                /* for opening files */
 .finfo  = fileinfo,                /* for getting file info */
 .readf  = readfile,        /* for reading files */
 .writef = writefile,               /* for writing to output file */
 .closef = closefile,               /* for closing files */

};

/** Kermit response structure */
struct k_response r;

/** Argument processing buffer */
static char process_buffer[128];

/***********************************************************************************************************************
Functions
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Initialize the comms API framework
 *
 * @retval  void
 ***********************************************************************************************************************/
void sf_comms_recv(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);

    k.filelist = NULL;

    int status = do_kermit(A_RECV);

    if(status == X_DONE)
    {
        fx_media_flush(&g_fx_media);

        print_to_console("File received successfully\r\n");
    }
    else
    {
        print_to_console("Failed to receive file \r\n");
    }
}


/*******************************************************************************************************************//**
 * @brief  Perform a kermit transfer
 *
 * @retval  void
 ***********************************************************************************************************************/
void sf_comms_txfr(sf_console_cb_args_t * p_args)
{
    char* file_list[8] = {0};
    int count = 0;

    strncpy(process_buffer, (char*)p_args->p_remaining_string, 128);
    file_list[count] = strtok(process_buffer," ");
    while(file_list[count] != NULL)
    {
        count++;
        file_list[count] = strtok(NULL," ");
    }

    if(!validate_file_list(file_list))
    {
        print_to_console("Invalid Files\r\n");
    }
    else
    {
        print_to_console("Waiting few seconds to setup Transfer\r\n");
        tx_thread_sleep(2000);

        k.filelist = (UCHAR**)file_list;

        int status = do_kermit(A_SEND);

        if(status == X_DONE)
        {
            print_to_console("Files Transferred successfully\r\n");
        }
        else
        {
            print_to_console("Failed to Transfer file \r\n");
        }
    }


}


/*******************************************************************************************************************//**
 * @brief  Do kermit file transfer/receive
 *
 * @arg[in] action to be performed
 * @retval  status
 ***********************************************************************************************************************/
int do_kermit(int action)
{
    int status = X_OK;
    int rx_len;
    UCHAR *inbuf;
    short r_slot;

    status = kermit(K_INIT, &k, 0, 0, "", &r);

    if (status != X_ERROR)
    {
        if(action == A_SEND )
        {
            status = kermit(K_SEND, &k, 0, 0, "", &r);
        }

        while (1)
        {
            /* Allocate a window slot */
            inbuf = getrslot(&k,&r_slot);

            /* Try to read a packet */
            rx_len = k.rxd(&k,inbuf,P_PKTLEN);

            /* No data was read */
            if (rx_len < 1)
            {
                /* So free the window slot */
                freerslot(&k,r_slot);

                /* Fatal error */
                if (rx_len < 0)
                {
                    /* give up */
                    status = X_ERROR;
                    break;
                }

            }

            /* Handle the input */
            status = kermit(K_RUN, &k, r_slot, rx_len, "", &r);
            if((status == X_DONE) || (status == X_ERROR))
            {
                    break;
            }
        }
    }

    /* Close the files */
    if(action == A_SEND )
    {
        closefile(&k,0,1);
    }
    else
    {
        /* If transfer is failed, make sure that the file is deleted  */
        if (status == X_ERROR)
        {
            closefile(&k,'D',2);
        }
        else
        {
            closefile(&k,0,2);
        }
    }
    return status;
}

/*******************************************************************************************************************//**
 * @brief  validate the list of file names
 *
 * @arg[in] list of file names
 * @retval  status
 ***********************************************************************************************************************/
bool validate_file_list(char** file_list)
{
    bool retval = true;
    int count   = 0;
    FX_FILE file;

    while(file_list[count] != NULL)
    {
        if(FX_SUCCESS != fx_file_open(&g_fx_media, &file,
                                      (CHAR*)file_list[count],FX_OPEN_FOR_READ))
        {
            retval = false;
            break;
        }
        fx_file_close(&file);
        count ++;
    }

    return ( (count == 0) ? false : retval );
}





