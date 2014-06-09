
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
    MD5Init(&ctx);
    str = (char*) string;
  /*  while(len>0){
        if(len > 512){
            MD5Update(&ctx, str, 512);
            len -= 512;
            str += 512;
        } else{*/
     MD5Update(&ctx, str, len);
 /*           break;
        }
    }*/
    MD5Final(final, &ctx);
    strcpy(md5buf,"");
    for(i=0;i<MD5_HASH_LEN;i++){
        Str_Sprintf(md5buf,MAX_FILE_NAME_LEN,"%s%02x",md5buf,final[i]);
    }
    return;
}
