#include "dedup.h"

void traverse(char *dirPath){
   DIR* FD;
   struct dirent* in_file;
   if (NULL == (FD = opendir (dirPath))){
      fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno)); 
      return;
   }
   while ((in_file = readdir(FD))){
      if (!strcmp (in_file->d_name, "."))
	 continue;
      if (!strcmp (in_file->d_name, ".."))    
	 continue;
      printf("File name is %s\n",in_file->d_name);
   }
   return;
}
