
//  Created by Aditya Kotwal on 6/5/14.

#include <stdio.h>
#include <string.h>
#include "dedup.h"
/*
A function to generate MD5 hash of a string using funcitons
available in the openSSL library.

Input Parameters:
   - String: A char buffer containing the string which is to
             be md5 encoded
   - md5buf: A pre-allocated character buffer where the
             result will be stored
   - len : Lenght of the string which has to be md5 encoded
*/
void getMD5(const char *string, char *md5buf, long len){
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
A function to get the equivalent hexadecimal representation
of a nibble passed.

Input Parameters:
   - bin : Character parameter passed with the higher nibble set to 0
Ouput:
   - Hexademical charater representation
*/
char bin2Hex(unsigned char bin){
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
	 default:
	    printf("Error in conversion\n");
	    return ((char) 0 );
      }
   }
   return '0';
}
