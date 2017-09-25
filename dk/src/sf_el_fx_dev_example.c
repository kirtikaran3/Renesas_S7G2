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
 * File Name    : sf_el_fx_dev_example.c
 * Description  : TODO
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           TODO       1.00    Initial Release.
 ***********************************************************************************************************************/


#include "dev_example_common.h"
#include "dev_example_thread.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#define BUFFER_SIZE 128

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/

void sf_fx_ls(sf_console_cb_args_t * p_args);
void sf_fx_cat(sf_console_cb_args_t * p_args);
void sf_fx_mv(sf_console_cb_args_t * p_args);
void sf_fx_cp(sf_console_cb_args_t * p_args);
void sf_fx_rm(sf_console_cb_args_t * p_args);
void sf_fx_touch(sf_console_cb_args_t * p_args);
void sf_fx_edit(sf_console_cb_args_t * p_args);

void sf_fx_mkdir(sf_console_cb_args_t * p_args);
void sf_fx_rmdir(sf_console_cb_args_t * p_args);
void sf_fx_mvdir(sf_console_cb_args_t * p_args);
void sf_fx_clear(sf_console_cb_args_t * p_args);

/***********************************************************************************************************************
 Global variables
***********************************************************************************************************************/

CHAR g_file_name[FX_MAX_LONG_NAME_LEN];
char buffer[BUFFER_SIZE];

struct attributes
{
    UINT *attributes;
    ULONG *size;
    UINT *year;
    UINT *month;
    UINT *day;
    UINT *hour;
    UINT *minute;
    UINT *second;
};


/** sf_fx console menu commands **/

const sf_console_command_t g_sf_fx_commands[] =
{
 {
  (uint8_t*)"ls",
  (uint8_t*)"List the directories\r\n\t\t  Synopsis:- ls",
  sf_fx_ls,
  NULL
 },
 {
  (uint8_t*)"cat",
  (uint8_t*)"Cat the files and display in console\r\n\tSynopsis:- cat <FILE>"
          "\r\n\tExample:- cat sample.txt",
  sf_fx_cat,
  NULL
 },
 {
  (uint8_t*)"mv ",
  (uint8_t*)"Rename the files\r\n\tSynopsis:- mv <OLD FILE NAME> <NEW FILE NAME>"
          "\r\n\tExample:- mv file1.txt file2.txt",
  sf_fx_mv,
  NULL
 },
 {
  (uint8_t*)"cp",
  (uint8_t*)"Copy the files\r\n\tSynopsis:- cp <FILE1> <FILE2>"
          "\r\n\tExample:- cp file1.txt file2.txt",
  sf_fx_cp,
  NULL
 },
 {
  (uint8_t*)"rm ",
  (uint8_t*)"Remove the files\r\n\tSynopsis:- rm <FILE>"
          "\r\n\tExample:- rm sample.txt",
  sf_fx_rm,
  NULL
 },
 {
  (uint8_t*)"touch",
  (uint8_t*)"Create the files\r\n\tSynopsis:- touch <FILE>"
          "\r\n\tExample:- touch sample.txt",
  sf_fx_touch,
  NULL
 },
 {
  (uint8_t*)"edit",
  (uint8_t*)"Edit the files\r\n\tSynopsis:- edit <FILE> <STRING>"
          "\r\n\tExample:- edit sample.txt Hello, World",
  sf_fx_edit,
  NULL
 },
 {
  (uint8_t*)"mkdir",
  (uint8_t*)"Create a directory\r\n\tSynopsis:- mkdir <FOLDER NAME>"
          "\r\n\tExample:- mkdir folder1",
  sf_fx_mkdir,
  NULL
 },
 {
  (uint8_t*)"rmdir",
  (uint8_t*)"Remove the directory\r\n\tSynopsis:- rmdir <FOLDER NAME>"
          "\r\n\tExample:- rmdir folder1",
  sf_fx_rmdir,
  NULL
 },
 {
  (uint8_t*)"mvdir",
  (uint8_t*)"Rename a directory\r\n\tSynopsis:- mvdir <OLD FOLDER> <NEW FOLDER>"
          "\r\n\tExample:- mvdir folder1 folder2",
  sf_fx_mvdir,
  NULL
 },
 {
  (uint8_t*)"clear",
  (uint8_t*)"Clear screen",
  sf_fx_clear,
  NULL
 }
};


/** sf_fx  menu */
const sf_console_menu_t g_sf_fx_menu =
{
        .menu_prev = &g_sf_console_root_menu,
        .menu_name = (uint8_t const *)"sf_el_fx",
        .num_commands = (sizeof(g_sf_fx_commands)) / (sizeof(g_sf_fx_commands[0])),
        .command_list = g_sf_fx_commands

};



/***********************************************************************************************************************
Functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief   Prints the file/dir list in the storage media
 *  @example 'ls'
 *  @retval VOID
 ***********************************************************************************************************************/
void sf_fx_ls(sf_console_cb_args_t * p_args)
{
   UNUSED(p_args);
   UINT status = FX_SUCCESS;

   status =  fx_directory_first_entry_find(&g_fx_media, &g_file_name[0]);
   while (status == FX_SUCCESS)
   {
       /** Find file name, then prfint it. */
       g_sf_console.p_api->write(g_sf_console.p_ctrl, (uint8_t const *)&g_file_name[0], 10);
       g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"\r\n", 10);

       status =  fx_directory_next_entry_find(&g_fx_media, &g_file_name[0]);
   }
}


/*******************************************************************************************************************//**
 * @brief   Displays the content of the file in console
 * @example 'cat filename.ext'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_cat(sf_console_cb_args_t * p_args)
{
  //  UNUSED(p_args);
   UINT status = FX_SUCCESS;
   FX_FILE g_file;
   CHAR *filename ;
   uint32_t actual_size;
   uint32_t size = BUFFER_SIZE;


   filename =(CHAR *)p_args->p_remaining_string;
   status =  fx_file_open(&g_fx_media, &g_file,filename,FX_OPEN_FOR_READ);
   if(status == FX_SUCCESS)
   {
       while (status != FX_END_OF_FILE)
       {
           status = fx_file_read(&g_file,buffer,size,&actual_size);
           if(status == FX_SUCCESS)
           {
               g_sf_console.p_api->write(g_sf_console.p_ctrl,(const uint8_t*)buffer, TX_WAIT_FOREVER);
               g_sf_console.p_api->write(g_sf_console.p_ctrl,(const uint8_t*)"\r\n", TX_WAIT_FOREVER);
           }
           else
           {
               fx_file_close(&g_file);
           }

       }
   }
   else
   {
       g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"file open error\r\n", TX_WAIT_FOREVER);
   }

}


/*******************************************************************************************************************//**
 * @brief   move or rename the current file in the storage media
 * @example 'mv old_filename new_filename'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_mv(sf_console_cb_args_t * p_args)
{
    UINT status = FX_SUCCESS;
    CHAR *new,*old;

    strncpy((char *)buffer,(const char *)p_args->p_remaining_string,128);
    old =(CHAR *)strtok((char *)buffer," ");
    new =(CHAR *)strtok(NULL," ");

    status = fx_file_rename(&g_fx_media,old,new);
    if(status != FX_SUCCESS)
    {
         g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"rename error\r\n", TX_WAIT_FOREVER);
    }

}


/*******************************************************************************************************************//**
 * @brief   remove the file in the storage media
 * @example 'rm filename.ext'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_rm(sf_console_cb_args_t * p_args)
{
    CHAR *rm_file;
    UINT status = FX_SUCCESS;

    strncpy(buffer,(const char *)p_args->p_remaining_string,128);
    rm_file =(CHAR *)strtok(buffer," ");
    status = fx_file_delete(&g_fx_media,rm_file);
    if(status != FX_SUCCESS)
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"file rm error\r\n", TX_WAIT_FOREVER);
    }
    fx_media_flush(&g_fx_media);

}


/*******************************************************************************************************************//**
 * @brief   copy the file in the storage media
 * @example 'cp filename1.ext filename2.ext'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_cp(sf_console_cb_args_t * p_args)
{
    UINT status = FX_SUCCESS;
    CHAR *new,*old;
    FX_FILE g_file_src;
    FX_FILE g_file_dst;
    uint32_t size=BUFFER_SIZE;
    uint32_t actual_size;

     strncpy(buffer,(const char*)p_args->p_remaining_string,128);
     old = (CHAR *)strtok(buffer," ");
     new = (CHAR *)strtok(NULL," ");

     status = fx_file_create(&g_fx_media,new);
     if(status != FX_SUCCESS)
     {
         g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"create error\r\n", TX_WAIT_FOREVER);
     }

     status = fx_file_open(&g_fx_media, &g_file_src,old,FX_OPEN_FOR_READ);
     if(status != FX_SUCCESS)
     {
         g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"src file open error\r\n", TX_WAIT_FOREVER);
     }

     status = fx_file_open(&g_fx_media, &g_file_dst,new,(FX_OPEN_FOR_WRITE | FX_OPEN_FOR_READ));
     if(status != FX_SUCCESS)
     {
         g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"dst file open error\r\n", TX_WAIT_FOREVER);
     }

     while (status != FX_END_OF_FILE)
     {
         status = fx_file_read(&g_file_src,buffer,size,&actual_size);
         if(status == FX_SUCCESS)
         {
             status = fx_file_write(&g_file_dst,buffer,actual_size);
             if(status != FX_SUCCESS)
             {
                 g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"write error\r\n", TX_WAIT_FOREVER);

             }
         }
         else if(status != FX_END_OF_FILE)
         {
             g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"read error\r\n", TX_WAIT_FOREVER);
             break;
         }

     }
     fx_file_close(&g_file_src);
     fx_file_close(&g_file_dst);
     fx_media_flush(&g_fx_media);
}



/*******************************************************************************************************************//**
 * @brief   create a new file in the storage media
 * @example 'create filename.ext'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_touch(sf_console_cb_args_t * p_args)
{
    CHAR *create_file;
    UINT status = FX_SUCCESS;


    strncpy(buffer,(const char*)p_args->p_remaining_string,128);
    create_file = (CHAR *)strtok(buffer," ");
    status = fx_file_create(&g_fx_media,create_file);
    if(status != FX_SUCCESS)
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"file create error\r\n", TX_WAIT_FOREVER);
    }
    fx_media_flush(&g_fx_media);

}

/*******************************************************************************************************************//**
 * @brief   edit a file in the storage media
 * @example 'edit filename.ext String'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_edit(sf_console_cb_args_t * p_args)
{
    CHAR *create_file;
    char *message;
    UINT status = FX_SUCCESS;
    FX_FILE g_new_file;

    strncpy(buffer,(const char*)p_args->p_remaining_string,128);
    create_file =(CHAR *)strtok(buffer," ");
    message = &buffer[strlen((char *)create_file)+1];
    status = fx_file_open(&g_fx_media, &g_new_file,create_file,(FX_OPEN_FOR_READ | FX_OPEN_FOR_WRITE));
    if(status != FX_SUCCESS)
    {
            g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"file open error\r\n", TX_WAIT_FOREVER);
    }
    status = fx_file_write(&g_new_file,message,128);
    if(status != FX_SUCCESS)
    {
               g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"write error\r\n", TX_WAIT_FOREVER);
    }
    status = fx_file_close(&g_new_file);
    fx_media_flush(&g_fx_media);

}

/*******************************************************************************************************************//**
 * @brief  create a directory in the storage media
 * @example 'mkdir dirname'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_mkdir(sf_console_cb_args_t * p_args)
{
    CHAR *create_file;
    UINT status = FX_SUCCESS;

    strncpy(buffer,(const char*)p_args->p_remaining_string,128);
    create_file =(CHAR *)strtok(buffer," ");

    status = fx_directory_create(&g_fx_media,create_file);
    if(status != FX_SUCCESS)
    {
             g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"dir creation error\r\n", TX_WAIT_FOREVER);
    }
    fx_media_flush(&g_fx_media);
}


/*******************************************************************************************************************//**
 * @brief   Remove a directory in the storage media
 * @example 'rmdir dirname'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_rmdir(sf_console_cb_args_t * p_args)
{
    CHAR *rm_directory;
    UINT status = FX_SUCCESS;

    strncpy(buffer,(const char*)p_args->p_remaining_string,128);
    rm_directory =(CHAR *)strtok(buffer," ");

    status = fx_directory_delete(&g_fx_media,rm_directory);
    if(status != FX_SUCCESS)
    {
             g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"dir delete error\r\n", TX_WAIT_FOREVER);
    }
    fx_media_flush(&g_fx_media);
}

/*******************************************************************************************************************//**
 * @brief   Move or Rename a directory in the storage media
 * @example 'mvdir oldname newname'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_mvdir(sf_console_cb_args_t * p_args)
{
    CHAR *old_dir,*new_dir;
    UINT status = FX_SUCCESS;

    strncpy(buffer,(const char*)p_args->p_remaining_string,128);
    old_dir =(CHAR *)strtok(buffer," ");
    new_dir =(CHAR *)strtok(NULL," ");

    status = fx_directory_rename(&g_fx_media,old_dir,new_dir);
    if(status != FX_SUCCESS)
    {
             g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"dir rename error\r\n", TX_WAIT_FOREVER);
    }
    fx_media_flush(&g_fx_media);
}

/*******************************************************************************************************************//**
 * @brief  Clear the Console
 * @example 'clear'
 * @retval VOID
 ***********************************************************************************************************************/
void sf_fx_clear(sf_console_cb_args_t * p_args)
{
    UNUSED(p_args);
    int i;
    for(i=0;i<128;i++)
    {
        g_sf_console.p_api->write(g_sf_console.p_ctrl,(uint8_t const *)"\r\n", TX_WAIT_FOREVER);
    }
}
