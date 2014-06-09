
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
	fp=fopen(file,"wb");
	if(fp==NULL){
        	printf("Failed to open file %s in %s mode\n",file,"wb");
    	}
	if(fclose(fp)!=0){
		printf("Failed to close file %s\n",file);
	}	
	return;
}

// Generate text dump by scanning the target location
void generateDump(char* source, char* destination){
    FILE *ip,*op;
    unsigned long len;
    unsigned long offset=0;
    char buf[1024*4];	// No dynmaic allocation
    char out[MD5_HASH_LEN*2+1];
    printf("File to work on : %s\n",source);
    ip=fopen(source,"wb");
    if(ip==NULL){
        printf("Failed to open file %s in %s mode\n",source,"wb");
    }
    op=fopen(destination,"wb");
    if(op==NULL){
        printf("Failed to open file %s in %s mode\n",destination,"ab");
    }
    fprintf(op,"String,File Name,md5,offset,size\n"); 
    while((len=fread(buf,1,RDLEN,ip))!=0){
	strcpy(out,"");
        getMD5(buf,out,len);
	fprintf(op,"%s,%s,%lu,%lu\n",source,out,offset,len);
        offset += len;
    }
    if(fclose(op)!=0){
	printf("Error closing file %s\n",destination);
    }
    if(fclose(ip)!=0){
	printf("Error closing file %s\n",source);
    }
}


int checkSource(char *source){
    FILE *fp=NULL;
    if((fp=fopen(source, "rb"))==NULL){
        return 0;
    }
    else{
        if(fclose(fp)!=0){
		printf("Error closing file %s\n",source);
	}
        return 1;
    }
}
