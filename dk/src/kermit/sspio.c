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
* File Name    : sspio.c
* Description  : IO routines to enable kermit protocol on top of SSP
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           TODO       1.00    Initial Release.
***********************************************************************************************************************/




/*
  Device i/o:

    int devopen()    Communications device - open
    int pktmode()    Communications device - enter/exit packet mode
    int readpkt()    Communications device - read a packet
    int tx_data()    Communications device - send data
    int devclose()   Communications device - close
    int inchk()      Communications device - check if bytes are ready to read

  File i/o:

    int openfile()   File - open for input or output
    ULONG fileinfo() Get input file modtime and size
    int readfile()   Input file - read data
    int writefile()  Output file - write data
    int closefile()  Input or output file - close

  Full definitions below, prototypes in kermit.h.

  These routines must handle speed setting, parity, flow control, file i/o,
  and similar items without the kermit() routine knowing anything about it.
  If parity is in effect, these routines must add it to outbound characters
  and strip it from inbound characters.
*/

#include <errno.h>
#ifndef O_WRONLY
#ifdef X_OK
#undef X_OK
#endif /* X_OK */
#endif /* O_WRONLY */

#include "cdefs.h"
#include "debug.h"
#include "platform.h"
#include "kermit.h"

#include "dev_example_thread.h"
#include "dev_example_common.h"


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
int readfile(struct k_data * k);
int writefile(struct k_data * k, UCHAR * s, int n);
int closefile(struct k_data * k, UCHAR c, int mode);

int xerror();

UCHAR o_buf[OBUFLEN+8];			/* File output buffer */
UCHAR i_buf[IBUFLEN+8];			/* File output buffer */

FX_FILE ifile;
FX_FILE ofile;


/* Debugging */

#ifdef DEBUG

/*******************************************************************************************************************//**
 * @brief  Debug logging
 *
 * @param[in]  fc - handle to debug logger
 * @param[in]  label - debug label
 * @param[in]  sval  - string value to be written
 * @param[in]  nval  - numeric value to be written
***********************************************************************************************************************/
void dodebug(int fc, UCHAR * label, UCHAR * sval, long nval)
{
  UNUSED(fc);
  UNUSED(label);
  UNUSED(sval);
  UNUSED(nval);

}
#endif /* DEBUG */


/*******************************************************************************************************************//**
 * @brief  Open communications device.
 * @param[in]  string pointer to device name
 * @return  0 on failure, 1 on success.
***********************************************************************************************************************/
int devopen(char *device)
{
    /* For SSP, the device open and close are done separately. Hence NO-OP */
    UNUSED(device);
    return(1);
}


/*******************************************************************************************************************//**
 * @brief  Put communications device into or out of packet mode
 * @param[in]  0 to put in normal (cooked) mode, 1 to put in packet (raw) mode.

 * @return  0 on failure, 1 on success.
***********************************************************************************************************************/
int pktmode(short on)
{
    /* NO-OP in this implementation. */
    UNUSED(on);
    return(1);
}


/*******************************************************************************************************************//**
 * @brief  Performs device settings.
 * @param[in]  string pointer to device settings string
 * @return  0 on failure, 1 on success.
***********************************************************************************************************************/
int devsettings(char * s)
{
    /* NO-OP in this implementation.*/
    UNUSED(s);
    return(1);
}

/*******************************************************************************************************************//**
 * @brief  Restore the device settings.
 *
 * @return  0 on failure, 1 on success.
***********************************************************************************************************************/
int devrestore(void)
{
    /* Put device back as we found it */
    pktmode(OFF);
    return(1);
}


/*******************************************************************************************************************//**
 * @brief  Closes the current open communications device.
 *
 * @return  0 on failure, 1 on success.
***********************************************************************************************************************/
int devclose(void)
{
    /* For SSP, the device open and close are done separately. Hence NO-OP */
    return(1);
}


/*******************************************************************************************************************//**
 * @brief   Check if input is waiting to be read, needed for sliding windows.
 *
 * @param[in]  pointer to kermit data
 * @return  -1 on failure, number of characters waiting to be read on success.
***********************************************************************************************************************/
int inchk(struct k_data * k)
{
    UNUSED(k);
    /*Currently the comms framework of SSP does not offer an API for this.
     * Hence return -1 */
    return(-1);
}

/*******************************************************************************************************************//**
 * @brief  Read a Kermit packet from the communications device
 * When reading a packet, this function looks for start of Kermit packet
 * (k->r_soh), then reads everything between it and the end of the packet
 * (k->r_eom) into the indicated buffer.
 *
 * @param[in]  k   - Kermit struct pointer
 * @param[in]  p   - pointer to read buffer
 * @param[in]  len - length of read buffer
 *
 * @return  The number of bytes read, or:
 *    0   - timeout or other possibly correctable error;
 *   -1   - fatal error, such as loss of connection, or no buffer to read into.
***********************************************************************************************************************/
int readpkt(struct k_data * k, UCHAR *p, int len)
{
    int x, n;
    short flag;
    UCHAR c;

    UNUSED(len);
/*
  Timeout not implemented in this sample.
  It should not be needed.  All non-embedded Kermits that are capable of
  making connections are also capable of timing out, and only one Kermit
  needs to time out.  NOTE: This simple example waits for SOH and then
  reads everything up to the negotiated packet terminator.  A more robust
  version might be driven by the value of the packet-length field.
*/
#ifdef F_CTRLC
    short ccn;
    ccn = 0;
#endif /* F_CTRLC */

    if (!p) {		/* Device not open or no buffer */
	return(-1);
    }
    flag = 0;
    n = 0;                       /* Init local variables */

    while (1)
    {
        g_sf_comms.p_api->read(g_sf_comms.p_ctrl,(uint8_t*)&x,1,TX_WAIT_FOREVER);
        c = (k->parity) ? (UCHAR)x & 0x7f : (UCHAR)x & 0xff; /* Strip parity */

#ifdef F_CTRLC
	/* In remote mode only: three consecutive ^C's to quit */
        if (k->remote && c == (UCHAR) 3)
        {
            if (++ccn > 2)
            {
                return(-1);
            }
        }
        else
        {
            ccn = 0;
        }
#endif /* F_CTRLC */

        if (!flag && c != k->r_soh)	/* No start of packet yet */
          continue;                     /* so discard these bytes. */
        if (c == k->r_soh)
        {		/* Start of packet */
            flag = 1;                   /* Remember */
            continue;                   /* But discard. */
        }
        else if (c == k->r_eom	/* Packet terminator */
		   || c == '\012'	/* 1.3: For HyperTerminal */
		   )
        {
            return(n);
        }
        else
        {                        /* Contents of packet */
            if (n++ > k->r_maxlen)	/* Check length */
              return(0);
            else
              *p++ = (UCHAR)x & 0xff;
        }
    }
    return(-1);
}


/*******************************************************************************************************************//**
 * @brief  Writes n bytes of data to communication device.
 *
 * @param[in]  k - Kermit struct pointer
 * @param[in]  p - pointer to data to transmit.
 * @param[in]  n - length
 *
 * @return  X_OK on success.
 *          X_ERROR on failure to write - i/o error.
***********************************************************************************************************************/
int tx_data(struct k_data * k, UCHAR *p, int n)
{

    UNUSED(k);

    ssp_err_t ssp_err = g_sf_comms.p_api->write(g_sf_comms.p_ctrl,
                                                    p,(uint32_t)n,TX_WAIT_FOREVER);

    return ((ssp_err == SSP_SUCCESS) ? X_OK : X_ERROR );


}

/*******************************************************************************************************************//**
 * @brief  Open file in the file system
 *
 * @param[in]  k - Kermit struct pointer
 * @param[in]  s - pointer to file name
 * @param[in]  mode - 1 = read, 2 = create, 3 = append.
 *
 * @return  X_OK on success.
 *          X_ERROR on failure ncluding rejection based on name, size, or date.
***********************************************************************************************************************/
int openfile(struct k_data * k, UCHAR * s, int mode)
{
    UINT fx_error;
    switch (mode)
    {
        case 1:               /* Read */
        {
            if(FX_SUCCESS != fx_file_open(&g_fx_media,&ifile,(CHAR*)s,FX_OPEN_FOR_READ))
            {
                return(X_ERROR);
            }
            k->s_first   = 1;       /* Set up for getkpt */
            k->zinbuf[0] = '\0';        /* Initialize buffer */
            k->zinptr    = k->zinbuf;   /* Set up buffer pointer */
            k->zincnt    = 0;       /* and count */
            return(X_OK);
        }

        case 2:                /* Write (create) */
        {
            fx_error = fx_file_create(&g_fx_media,(CHAR*)s);
            if((fx_error != FX_SUCCESS) && (fx_error != FX_ALREADY_CREATED))
            {
                return(X_ERROR);
            }
            if(FX_SUCCESS != fx_file_open(&g_fx_media,&ofile,(CHAR*)s,FX_OPEN_FOR_WRITE))
            {
                return(X_ERROR);
            }
            return(X_OK);
        }

        default:
            return(X_ERROR);
    }

}


/*******************************************************************************************************************//**
 * @brief  Get info about existing file
 *
 * @param[in]  k - Kermit struct pointer
 * @param[in]  filename - pointer to file name
 * @param[in]  buf - Pointer to buffer for date-time string
 * @param[in]  buflen - Length of date-time string buffer (must be at least
 *              18 bytes)
 * @param{in]  type - Pointer to int file type:
 *                      0: Prevailing type is text.
 *                      1: Prevailing type is binary.
 * @param[in]  mode - Transfer mode (0 = auto, 1 = manual):
 *                0: Figure out whether file is text or binary and return type.
 *                1: (nonzero) Don't try to figure out file type.
 *
 * @return  X_ERROR on failure.
 *          0L or greater on success == file length.
 *          Date-time string set to yyyymmdd hh:mm:ss modtime of file.
 *          If date can't be determined, first byte of buffer is set to NUL.
 *          Type set to 0 (text) or 1 (binary) if mode == 0.
***********************************************************************************************************************/
ULONG fileinfo(struct k_data * k, UCHAR * filename, UCHAR * buf,
                                    int buflen, short * type, short mode)
{
    /* NO-OP in this implementation */
    UNUSED(k);
    UNUSED(filename);
    UNUSED(buf);
    UNUSED(buflen);
    UNUSED(type);
    UNUSED(mode);
   return (ULONG)X_ERROR;
}


/*******************************************************************************************************************//**
 * @brief  Read the file
 *
 * @param[in]  k - Kermit struct pointer
 *
 * @return  -1 on Failure
 *          First read byte on success
***********************************************************************************************************************/
int readfile(struct k_data * k)
{
    UINT fx_error;
    ULONG bytes_read;

    if (!k->zinptr)
    {
        return(X_ERROR);
    }
    if (k->zincnt < 1) /* Nothing in buffer - must refill */
    {
        if (k->binary) /* Binary - just read raw buffers */
        {
            k->dummy = 0;
            fx_file_read(&ifile,k->zinbuf,(ULONG)k->zinlen,(ULONG*) &k->zincnt);

        } else
        {            /* Text mode needs LF/CRLF handling */
            int c;          /* Current character */
            for (k->zincnt = 0; (k->zincnt < (k->zinlen - 2)); (k->zincnt)++)
            {
                fx_error = fx_file_read(&ifile,&c,1,&bytes_read);
                if(fx_error == FX_END_OF_FILE)
                    break;
                if (c == '\n')      /* Have newline? */
                    k->zinbuf[(k->zincnt)++] = '\r'; /* Insert CR */
                k->zinbuf[k->zincnt] = (UCHAR)c;
            }
    #ifdef DEBUG
            k->zinbuf[k->zincnt] = '\0';

    #endif /* DEBUG */
        }
        k->zinbuf[k->zincnt] = '\0';    /* Terminate. */
        if (k->zincnt == 0)     /* Check for EOF */
          return(-1);
        k->zinptr = k->zinbuf;      /* Not EOF - reset pointer */
        }
        (k->zincnt)--;          /* Return first byte. */
        return(*(k->zinptr)++ & 0xff);
}


/*******************************************************************************************************************//**
 * @brief  Write data to file
 *
 * @param[in]  k - Kermit struct pointer
 * @param[in]  s - write buffer
 * @param[in]  n - number of bytes to write
 *
 * @return  X_OK on success.
 *          X_ERROR on failure
***********************************************************************************************************************/
int writefile(struct k_data * k, UCHAR * s, int n)
{
    int rc= X_OK;
    UINT fx_err;

    if (k->binary)
    {  /* Binary mode, just write it */
        fx_err = fx_file_write(&ofile,s,(ULONG)n);
       if (fx_err != FX_SUCCESS)
       {
         rc = X_ERROR;
       }
    }
    else
    {
        UCHAR * p, * q;
        int i;
        q = s;

        while (1)
        {
            for (p = q, i = 0; ((*p) && (*p != (UCHAR)13)); p++, i++) ;
            if (i > 0)
            {
                fx_err = fx_file_write(&ofile,q,(ULONG)i);
                if(fx_err != FX_SUCCESS)
                {
                    rc = X_ERROR;
                    break;
                }
            }

            if (!*p)
                break;
            q = p+1;
        }
    }

    return(rc);
}

/*******************************************************************************************************************//**
 * @brief  Close the file
 * For output files, the character c is the character (if any) from the Z
 * packet data field.  If it is D, it means the file transfer was canceled
 * in mid stream by the sender, and the file is therefore incomplete.  This
 * routine should check for that and decide what to do.  It should be
 * harmless to call this routine for a file that that is not open.
 *
 * @param[in]  k - Kermit struct pointer
 * @param[in]  c - character from Z packet data field
 * @param[in]  mode - 1 = read, 2 = create, 3 = append.
 *
 * @return  X_OK on success.
 *          X_ERROR on failure
***********************************************************************************************************************/
int closefile(struct k_data * k, UCHAR c, int mode)
{
    int rc = X_OK;			/* Return code */

    switch (mode)
    {
        /* Closing input file */
        case 1:
        {
           fx_file_close(&ifile);
           break;
        }

        case 2:              /* Closing output file */
        case 3:
        {

            fx_file_close(&ofile);
            if ((k->ikeep == 0) &&   /* Don't keep incomplete files */
                (c == 'D') &&        /* This file was incomplete */
                (k->filename))        /* File name is valid */
            {
                fx_file_delete(&g_fx_media,(CHAR*)k->filename); /* Delete it. */
            }
            break;
        }

        default:
            rc = X_ERROR;
    }

    return(rc);
}

#ifdef DEBUG
/*******************************************************************************************************************//**
 * @brief  Return error value

 * @return  error value
***********************************************************************************************************************/
int xerror()
{
   /* Not using error values, hence returning 0 */
    return 0;
}
#endif /* DEBUG */
