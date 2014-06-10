
/*
   Author : Aditya Kotwal
   Date of Start : 5/22/2014
   Email : akotwal@vmware.com
*/

#include "dedup.h"


int main(){
   cleanDump(DST);
   traverse(SRC);
   return 0;
}

/*
   Wrapper around the function 'generateDump()'. This wrapper
   checks whether the file who's deduplication analysis has
   been requested is accessible by the program.

   Input Parameters:
      - source: absolute path of the file under consideration
 */

void dedup(char *source)
{
   if(isAccessible(source)){
      generateDump(source,DST);
   }
   else{
      printf("File %s not accessible\n",source);
   }
   return;
}

/*
   Delete the dump file created in previous run

   Input Parameters:
      - source : Absolute path where the dedup metadata is to be placed
*/

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

/* Generate text dump by scanning the target location

   Input Parameters:
      - source : char buffer containing absolute address
                 of the folder whos files will be scanned
      - destination: Absolute path where the text dump
                 will be stored
*/

void generateDump(char* source, char* destination){
   FILE *ip,*op;
   unsigned long len;
   unsigned long offset=0;
   char buf[1024*4];	// No dynmaic allocation
   char out[MD5_HASH_LEN*2+1];  // MD5_HASH_LEN stores the length of 
				 // the encoding in bytes. When converted
				 // to char array, each bytes will be 
				 // represented by 2 characters. The array
				 // will be terminated by a null char '\0'
				 // Thus the length is 2*MD5_HASH_LEN + 1
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

/*
   Checks if the file under consideration is accessible to the program. 

   Input Parameters:
      - source: Absolute path of the file whose accessibility has to
		be checked.
   Returns: 1 for success, 0 for failure (not accessible)
*/

int isAccessible(char *source){
   FILE *fp;
   size_t size;
   struct stat st;
   stat(source,&st);
   size = st.st_size;
   if(size == 0){
      return 0;
   }
   fp = NULL;
   if((fp=fopen(source,"rb"))==NULL){
      return 0;
   }
   else{
      if(fclose(fp)!=0){ 
         printf("Error closing file %s\n",source);
      }
      return 1;
   }
}
