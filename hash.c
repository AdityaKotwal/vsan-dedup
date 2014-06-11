/*
 * This module deals with functions to encode string values
 * to their corresponing md5 hash
 */

#include <stdio.h>
#include <string.h>
#include "dedup.h"

/*
 * --------------------------------------------------------
 *
 *  getMD5 --
 *
 *    A function to generate MD5 hash of a string using 
 *    funcitons available in the openSSL library.
 *
 * ----------------------------------------------------------
 */

void getMD5(const char *string,	 // IN: String buffer which has to be encoded
	    long len,		 // IN: Length of the string buffer 
	    char *md5buf){	 // OUT: Pre-allocated buffer to store result
   unsigned char final[MD5_HASH_LEN];
   MD5_CTX ctx;
   int i;
   char *str;
   unsigned char b;
   MD5Init(&ctx);
   str = (char*) string;
   MD5Update(&ctx, str, len);
   MD5Final(final, &ctx);
   strcpy(md5buf,"");
   for(i=0;i<MD5_HASH_LEN;i++){
      b = final[i];
      md5buf[i * 2] = bin2Hex(b >> 4);
      md5buf[i * 2 + 1] = bin2Hex(b & 0x0F);
   }
   md5buf[MD5_HASH_LEN * 2]='\0';
   return;
}

/*
 * ----------------------------------------------------------
 * 
 *  bin2Hex --
 *
 *    A function to get the equivalent hexadecimal representation
 *    of a binary value passed.
 *
 * Required:
 *    The 4 LSB bits must be 0 for the input
 * ---------------------------------------------------------
 */

char bin2Hex(unsigned char bin) // IN: the binary number passed
{			        //     with higher nibble set to 0
   int value; 
   value = (bin & 0x1);
   value += (bin & 0x2) * 2;
   value += (bin & 0x4) * 4;
   value += (bin & 0x8) * 8;
   if(value < 10){
      return ((char)(value + 0x60));
   }
   else{
      switch(value % 10){
	 case 0:
	    return 'A';
	 case 1:
	    return 'B';
	 case 2:
	    return 'C';
	 case 3:
	    return 'D';
	 case 4:
	    return 'E';
	 case 5:
	    return 'F';
      }
   }
   printf("Error in coversion\n");
   return '0';
}
