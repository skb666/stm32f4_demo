/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
uint8_t exf_getfree(uint8_t *drv, uint32_t *total, uint32_t *free) {
  FATFS *fs1;
  uint8_t res;
  uint32_t fre_clust = 0, fre_sect = 0, tot_sect = 0;
  res = (uint32_t)f_getfree((const TCHAR *)drv, (DWORD *)&fre_clust, &fs1);
  if (res == 0) {
    tot_sect = (fs1->n_fatent - 2) * fs1->csize;
    fre_sect = fre_clust * fs1->csize;
#if _MAX_SS != 512
    tot_sect *= fs1->ssize / 512;
    fre_sect *= fs1->ssize / 512;
#endif
    *total = tot_sect >> 1;
    *free = fre_sect >> 1;
  }
  return res;
}

void Fatfs_RW_test(void) {
  uint32_t total, mfree;  // file objects
  //uint32_t byteswritten;  // File write counts
  //uint32_t bytesread;     // File read counts
  //uint8_t wtext[] = "This is STM32 working with FatFs"; // File write buffer
  //uint8_t rtext[100];     // File read buffers
  //char filename[] = "STM32cube.txt";

  //printf("\n ****** FatFs Example ******\n\n");

  /*##-1- Register the file system object to the FatFs module ##############*/

  retSD = f_mount(&SDFatFS, SDPath, 1);

  // f_mkdir("0:dxc");
  if (!retSD) {
    printf("mount sucess!!! \n");
  } else {
    printf("mount error : %d \n", retSD);
    Error_Handler();
  }

  /*##-2- Create and Open new text file objects with write access ######*/
  retSD = exf_getfree((uint8_t *)SDPath, &total, &mfree);
  if (retSD == 0) {
    printf("total : %lu MB,free : %lu MB  \n", total >> 10, mfree >> 10);
  } else {
    printf("getfree error!!! \n");
  }
  //retSD = f_open(&SDFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
  //if (retSD) {
  //  printf(" open file error : %d\n", retSD);
  //} else {
  //  printf(" open file sucess!!! \n");
  //}

  /*##-3- Write data to the text files ###############################*/
  //retSD = f_write(&SDFile, wtext, sizeof(wtext), (void *)&byteswritten);
  //if (retSD) {
  //  printf(" write file error : %d\n", retSD);
  //} else {
  //  printf(" write file sucess!!! \n");
  //  printf(" write Data : %s\n", wtext);
  //}

  /*##-4- Close the open text files ################################*/
  //retSD = f_close(&SDFile);
  //if (retSD) {
  //  printf(" close error : %d\n", retSD);
  //} else {
  //  printf(" close sucess!!! \n");
  //}

  /*##-5- Open the text files object with read access ##############*/
  //retSD = f_open(&SDFile, filename, FA_READ);
  //if (retSD) {
  //  printf(" open file error : %d\n", retSD);
  //} else {
  //  printf(" open file sucess!!! \n");
  //}

  /*##-6- Read data from the text files ##########################*/
  //retSD = f_read(&SDFile, rtext, sizeof(rtext), (UINT *)&bytesread);
  //if (retSD) {
  //  printf(" read error!!! %d\n", retSD);
  //} else {
  //  printf(" read sucess!!! \n");
  //  printf(" read Data : %s\n", rtext);
  //}

  /*##-7- Close the open text files ############################*/
  //retSD = f_close(&SDFile);
  //if (retSD) {
  //  printf(" close error!!! %d\n", retSD);
  //} else {
  //  printf(" close sucess!!! \n");
  //}

  /*##-8- Compare read data with the expected data ############*/
  //if (bytesread == byteswritten) {
  //  printf(" FatFs is working well!!!\n\n");
  //}
}
/* USER CODE END Application */
