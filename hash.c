
//  Created by Aditya Kotwal on 6/5/14.

#include <stdio.h>
#include <string.h>
#include "dedup.h"

// Generate md5 hash for a string buffer
void getMD5(const char *string, char *md5buf, long len){
    unsigned char final[MD5_HASH_LEN];
    MD5_CTX c;
    int i;
    char *str;
    MD5Init(&c);
    str = (char*) string;
    while(len>0){
        if(len > 512){
            MD5Update(&c, str, 512);
            len -= 512;
            str += 512;
        } else{
            MD5Update(&c, str, len);
            break;
        }
    }
    MD5Final(final, &c);
    strcpy(md5buf,"");
    for(i=0;i<MD5_HASH_LEN;i++){
        Str_Sprintf(md5buf,MAX_FILE_NAME_LEN,"%s%02x",md5buf,final[i]);
    }
    return;
}
