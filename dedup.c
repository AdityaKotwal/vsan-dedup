
/*
 * Author : Aditya Kotwal
 * Date of Start : 5/22/2014
 * Email : akotwal@vmware.com
 */

#include "dedup.h"

void generateDump(char* source, char* destination);
void dedup(char *source);
void cleanDump(char *file);
int checkSource(char *source);
void FWRITE(char *buf,char *msg, FILE* stream);
FILE* FOPEN(char *fileName, char* mode);
void FCLOSE(FILE *fp);


int main(){
    cleanDump(DST);
    walk(SRC,dedup);
    return 0;
}


void dedup(char *source)
{
    if(checkSource(source)){
        generateDump(source,DST);
    }
    return;
}


// Delete the dump file created in previous run
void cleanDump(char *file){
	FILE *fp=NULL;
	fp=FOPEN(file,"wb");
	FCLOSE(fp);
	return;
}

// Generate text dump by scanning the target location
void generateDump(char* source, char* destination){
    FILE *ip,*op;
    char size_str[MAX_BUF_LEN];
    char offset_str[MAX_BUF_LEN];
    unsigned long len;
    unsigned long offset=0;
    char *buf, *buf_orig , *buf_read;
    char header[MAX_FILE_NAME_LEN];
    char out[MD5_HASH_LEN*2+1];
    buf = NULL;
    buf_orig = NULL;
    buf_read = NULL;
    printf("File to work on : %s\n",source);
    ip = FOPEN(source,"rb");
    op = FOPEN(destination,"ab");

    if(verbose){
        strcpy(header,"String, File Name, md5, offset, size \n");
        FWRITE(header, "header", op);
    }

    if((buf = (char*)malloc(4*1024)) == NULL){
        printf("Error in allocating memory");
    }
    buf_orig=buf;
    buf_read = buf;
    while((len=fread(buf,1,RDLEN,ip))!=0){
		strcpy(out,"");
        getMD5(buf,out,len);
        if(verbose){
            FWRITE(buf_read, "buf",op);
            FWRITE(",",",",op);
        }
        if(fileNameRequired){
            FWRITE(source, "fileName",op);
            FWRITE(",",",",op);
        }
        FWRITE(out,"md5value",op);
        FWRITE(",",",",op);
        Str_Sprintf(offset_str,MAX_FILE_NAME_LEN, "%lu",offset);
        FWRITE(offset_str,"offset",op);
        FWRITE(",",",",op);

        Str_Sprintf(size_str,MAX_FILE_NAME_LEN, "%lu",len);
        FWRITE(size_str,"size",op);
        FWRITE("\n","New Line",op);

        offset += len;
    }
    free(buf);
    FCLOSE(op);
    FCLOSE(ip);
}


// Error checking wrapper for fwrite
void FWRITE(char *buf,char *msg, FILE* stream){
    if(fwrite(buf,1,strlen(buf),stream)!=strlen(buf)){
        printf("Error in writing %s to file\n",msg);
    }
}
//Error checking wrapper for fopen
FILE* FOPEN(char *fileName, char* mode){
    FILE* fp = NULL;
    if((fp=fopen(fileName,mode))==NULL){
        printf("Failed to open file %s in %s mode\n",fileName,mode);
    }
    return fp;
}

// Error checking wrapper for fclose
void FCLOSE(FILE *fp){
    if(fclose(fp)!=0){
        printf("Error closing file, %d\n",errno);
    }
}


int checkSource(char *source){
    FILE *fp=NULL;
    if((fp=fopen(source, "rb"))==NULL){
        return 0;
    }
    else{
        FCLOSE(fp);
        return 1;
    }
}
