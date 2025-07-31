#include "../../../include/cpu.h"
#include<stdlib.h>
#include <glob.h>
#include<stdio.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<cjson/cJSON.h>
static void checkfile(char* file){
    int fd;
    struct stat sb;
    char* data;
    fd = open(file, O_RDONLY);
    ASSERT(fd != -1, ".json file failed to open");
    ASSERT(fstat(fd,&sb) != -1, "fstat failed!");

    data = mmap(NULL,sb.st_size,PROT_READ,MAP_PRIVATE,fd,0);
    cJSON* json = cJSON_ParseWithLength(data, sb.st_size);
    ASSERT(json != 0, "cJSON parse, FAILED");

    int size = cJSON_GetArraySize(json);

    for(int i = 0; i < size; i++){
        cJSON* item = cJSON_GetArrayItem(json, i);
        ASSERT(item, "cJSON get item failed");

        cJSON* name = cJSON_GetObjectItem(item, "initial.pc");
        // ASSERT(cJSON_IsString(name), "Missing or invalid 'name' field");

        printf("%d\n", name->valueint);
        TODO("")
    }


    cJSON_Delete(json);
    munmap(data,sb.st_size);
    close(fd);
}

/**
 * Function will open all singlestep json files to test.
 */
void testcpu(){
    cpu_t cpu;
    bus_t bus;
    bus.ram = (uint8_t*)malloc(sizeof(uint8_t) * 1.6e7);
    ASSERT(bus.ram, "malloc failed!")
    cpu.bus = &bus;
    glob_t result;
    int val = glob("tests/v1/*.json",0,NULL,&result);
    ASSERT(val == 0,"glob failed");

    for(int i = 0; i < result.gl_pathc; i++){
        checkfile(result.gl_pathv[i]);
    }

}