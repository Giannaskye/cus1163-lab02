#include "proc_reader.h"

int list_process_directories(void){
  DIR *dir = opendir( "/proc");
   if (dir == NULL)
   {
		perror("Failed to open");
		return -1;
	}
    struct dirent *entries;
    int proc_count =0;
    printf("Process directories in /proc:\n");
    printf("%-8s %-20s\n", "PID", "Type");
    printf("%-8s %-20s\n", "---", "----");

    while((entries = readdir(dir)) != NULL)
   {
		if(is_number(entries->d_name))
			{
			printf("%s\n", entries->d_name);
			proc_count ++;
			}
   }

   if(closedir(dir) == -1)
   {
	  perror ("closedir failed");
	  return -1;
	}
	 printf("TOTAL: %d\n",proc_count);
     return 0; 
}

int read_process_info(const char* pid){
    char filepath[256];
    int rp;
    snprintf(filepath, sizeof(filepath),"/proc/%s/status", pid);
    printf("\n--- Process Information for PID %s ---\n", pid);
    rp = read_file_with_syscalls(filepath);
    if(rp == -1)
    {
		perror("Read file with syscalls failed");
		return -1;
	} 

    snprintf(filepath, sizeof(filepath), "/proc/%s/cmdline", pid);
    printf("\n--- Command Line ---\n");
    rp = read_file_with_syscalls(filepath);
    if(rp == -1)
    {
		perror("Read file with sysclls failed");
		return -1;
	}
	printf("\n"); // Add extra newline for readability
	return 0;
}




int show_system_info(void){
    int line_count = 0;
    const int MAX_LINES = 10;
	FILE *fd;
	char lines[256];
    printf("\n--- CPU Information (first %d lines) ---\n", MAX_LINES);
     fd = fopen("/proc/cpuinfo", "r");
	 if(fd == NULL)
	 {
		perror("fopen failed");
		return -1;
	 } 
   
    while(line_count < MAX_LINES && fgets(lines, sizeof(lines), fd) != NULL)
    {
		printf("%s", lines);
		line_count++;
    }
	if(fclose(fd) == EOF)
	{
	perror("fclose failed");
	return -1;
	}
  printf("\n--- Memory Information (first %d lines) ---\n", MAX_LINES);
	line_count=0;
    fd = fopen("/proc/meminfo", "r");
    if(fd == NULL)
    {
	perror("failed to open");
	return -1;
	}
	while(line_count < MAX_LINES && fgets(lines, sizeof(lines), fd) != NULL)
	{
		printf("%s", lines);
		line_count++;

   }
   
	if(fclose(fd) == EOF)
	{
	perror(" fclose failed");
	return -1;
	}
	return 0;
}


void compare_file_methods(void) {
    const char* test_file = "/proc/version";

    printf("Comparing file reading methods for: %s\n\n", test_file);

    printf("=== Method 1: Using System Calls ===\n");
    read_file_with_syscalls(test_file);

    printf("\n=== Method 2: Using Library Functions ===\n");
    read_file_with_library(test_file);

    printf("\nNOTE: Run this program with strace to see the difference!\n");
    printf("Example: strace -e trace=openat,read,write,close ./lab2\n");
}


    
int read_file_with_syscalls(const char* filename) {
    int fd;
    char buffer[1024];
    ssize_t bytes_read;

    fd= open(filename, O_RDONLY);
    if(fd ==-1){
		perror("open failed");
		return -1;
		}
		

	while((bytes_read = read(fd,buffer, sizeof(buffer)-1)) > 0){
		buffer[bytes_read] = '\0';
		printf("%s", buffer);
}
   if(bytes_read == -1) 
   {
        perror("read failed");
        return -1;
	}

    if(close(fd) == -1){
		perror("close failed");
		}
		return 0;
  
}

int read_file_with_library(const char* filename) {
    FILE* pointer;
    char buffer[1024];
    
    pointer = fopen(filename,"r");
	if (pointer == NULL)
 {
	 perror("fopen failed");
	 return -1;
 }
	while(fgets(buffer,sizeof(buffer), pointer) != NULL)
{
	 printf("%s", buffer);
} 
	if(fclose(pointer)== EOF){
		perror(" fclose failed");
		return -1;
}
    return 0; 
}

int is_number(const char* str) {
	if (str == NULL || *str == '\0') {
		return 0;
    }
    while(*str){
	   if(!isdigit((unsigned char)*str)){
		   return 0;
		   }
		   str++;
	   }
	return 1;
  }
