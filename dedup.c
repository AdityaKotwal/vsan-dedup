
#include "dedup.h"

/* 
 * dedup.c 
 *
 *    This module provides functions and wrappers to deal with
 *    the dump file as well as other functions which can be
 *    used to encode the hashes for the purpose of deduplication 
*/

int main( )
{
   cleanDump( DST );
   traverse( SRC );
   return 0;
}

/*
 * ------------------------------------------------------------
 *  cleanDump --
 * 
 *    File opened and closed in write mode and thus over-written
 *
 *  Side Effects:
 *    Contents of the target file lost
 * -------------------------------------------------------------
 */

void cleanDump( char *file ) // IN: Absolute path of the file to be overwritten
{ 
   FILE *fp = NULL;
   fp = fopen( file , "wb" );
   if( fp == NULL ){
      printf( "Failed to open file %s\n" , file );
   }
   if( fclose( fp ) != 0 ){
      printf( "Failed to close file %s\n" , file );
   }
   return;
}

/*
 * ----------------------------------------------------------
 *
 * generateDedupDump --
 *
 *    Generate text dump consisting of file name, segment size,
 *    segment offset and md5hash for segments of files.
 *
 * Side Effects:
 *    File as specified by the MACRO SRC, shall have modifed 
 *    contents
 *
 * -----------------------------------------------------------
 */

void generateDedupDump( char* source,	       // IN: Abs path of source file
		  char* destination )  // IN: Abs path of destination of dump
{
   FILE *ip,*op;
   unsigned long len;
   unsigned long offset;
   char buf[ 1024 * 4 ];	
   char out[ MD5_HASH_LEN * 2 + 1];  // each byte represented by 2 characters.
				     // String terminated by a null char '\0'	
   offset = 0;
   if( !isDedupCandidate( source ) ){
      printf( "Source not valid\n" );
      return;
   }
   ip = fopen( source , "rb" );
   if( ip == NULL ){
      printf( "Failed to open file %s \n" , source );
   }
   op = fopen( destination , "rb" );
   if( op == NULL ){
      printf( "Failed to open file %s \n" , destination );
   }
   fprintf( op , "String,File Name,md5,offset,size\n" ); 
   while((len = fread( buf , 1 , RDLEN , ip )) != 0 ){
      printf("Inside\n");
      strcpy( out , "" );
      getMD5( buf , len , out );
      printf( "%s,%s,%lu,%lu\n" , source , out , offset , len );
      fprintf( op , "%s,%s,%lu,%lu\n" , source , out , offset , len );
      offset += len;
   }
   if( fclose( op ) != 0 ){
      printf( "Error closing file %s\n" , destination );
   }
   if(fclose(ip)!=0){
      printf( "Error closing file %s\n" , source );
   }
}

/*
 * -------------------------------------------------------
 *
 *  isAccessible --
 *
 *    Checks if the file can be accessed by the program
 *
 *  Results:
 *    Returns 1 on success, and 0 on failure
 *
 *  Side Effect:
 *    No major side effects. File is opened and closed in read
 *    mode.
 *
 * -------------------------------------------------------
 */

int isDedupCandidate(char *source) // IN: Abs path to check
{
/*   size_t size;
   struct stat st;
   stat( source , &st );
   size = st.st_size;
   if( size < 1024 * 4 ){ // Files with size less than 4kB to be ignored 
			// eg. Infinite streams and raw devices
      return 0;
   }*/
   return 1;
}


/*
 * ------------------------------------------------------------
 *
 *  traverse --
 *
 *    Traverses all the files in a directory and calls the 
 *    method generateDedupDump for each file
 *
 * -------------------------------------------------------------
 */

void traverse( char *dirPath ) // IN: Directory Path
{
   DIR* FD;
   struct dirent* in_file;
   char path[ MAX_FILE_NAME_LEN ];
   int len;
   Str_Strcpy( path , dirPath , MAX_FILE_NAME_LEN );
   len=(int)strlen(dirPath);
   path[ len++ ] = '/';
   if (NULL == (FD = opendir (dirPath))){
      printf("Error : Failed to open input directory\n"); 
      return;
   }
   while ( (in_file = readdir( FD )) ){
      if (!strcmp( in_file->d_name , "."))
	 continue;
      if (!strcmp( in_file->d_name , "..")) 
	 continue;
      Str_Strcpy( path + len , in_file->d_name , MAX_FILE_NAME_LEN - len);
      printf( "Path is %s\n" , path );
      generateDedupDump( path , DST );
   }
   return;
}
