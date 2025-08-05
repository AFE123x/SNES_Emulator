#include "../../../include/cpu.h"
#include "json-c/json_object.h"
#include "json-c/json_tokener.h"
#include <stdio.h>

/* For JSON Tests */
#include <dirent.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void fill_initial(cpu_t *cpu, struct json_object *initial) {
    /* acquire program counter */
    struct json_object *pc;
    json_object_object_get_ex(initial, "pc", &pc);
    cpu->pc.raw = json_object_get_int(pc);
    /* acquire general purpose registers */
    json_object_object_get_ex(initial, "a", &pc);
    cpu->a.raw = json_object_get_int(pc);
    json_object_object_get_ex(initial, "x", &pc);
    cpu->x.raw = json_object_get_int(pc);
    json_object_object_get_ex(initial, "y", &pc);
    cpu->y.raw = json_object_get_int(pc);
    json_object_object_get_ex(initial, "s", &pc);
    cpu->sp = json_object_get_int(pc);
    json_object_object_get_ex(initial, "p", &pc);
    cpu->flag.raw = json_object_get_int(pc);
    json_object_object_get_ex(initial, "dbr", &pc);
    cpu->data_bank_register = json_object_get_int(pc);
    json_object_object_get_ex(initial, "d", &pc);
    cpu->d = json_object_get_int(pc);
    json_object_object_get_ex(initial, "pbr", &pc);
    cpu->prg_bank_register = json_object_get_int(pc);
    json_object_object_get_ex(initial, "e", &pc);
    cpu->emulation_mode = json_object_get_int(pc) > 0;
    json_object_object_get_ex(initial, "ram", &pc);
    unsigned long len = json_object_array_length(pc); // gets the length of the array
    for (int i = 0; i < len; i++) {
        struct json_object *memory = json_object_array_get_idx(pc, i);
        struct json_object *address = json_object_array_get_idx(memory, 0); // address
        struct json_object *byte = json_object_array_get_idx(memory, 1);    // byte
        cpu->bus->ram[json_object_get_int(address)] = json_object_get_int(byte);
        // printf("\ncpu->bus->ram[%d] = %d;",json_object_get_int(address),json_object_get_int(byte));
    }
}

static void compare_final(cpu_t *cpu, struct json_object *final, const char* instruction) {
    struct json_object *json_obj;
    // printf("instruction %s\n",instruction);
    /* acquire program counter */
    json_object_object_get_ex(final, "pc", &json_obj);
    ASSERT(cpu->pc.raw == json_object_get_int(json_obj),
           "PC Test - FAILED! expected %x, got %x",
           json_object_get_int(json_obj), cpu->pc.raw);

    /* acquire accumulator */
    json_object_object_get_ex(final, "a", &json_obj);
    ASSERT(cpu->a.raw == json_object_get_int(json_obj),
           "A Test - FAILED! expected %x, got %x",
           json_object_get_int(json_obj), cpu->a.raw);

    /* acquire X register */
    json_object_object_get_ex(final, "x", &json_obj);
    // uint16_t expected_x = (/*cpu->flag.n.x > 0 || */cpu->emulation_mode > 0) ? cpu->x.lo : cpu->x.raw;
    
    ASSERT(cpu->x.raw == json_object_get_int(json_obj),
           "X Test - FAILED! expected %04x, got %04x, %d, %d",
           json_object_get_int(json_obj), cpu->y.raw,cpu->flag.n.x,cpu->emulation_mode);

    /* acquire Y register */
    json_object_object_get_ex(final, "y", &json_obj);
    // uint16_t expected_y = (cpu->flag.n.x > 0 || cpu->emulation_mode > 0) ? cpu->y.lo : cpu->y.raw;
    ASSERT(cpu->y.raw == json_object_get_int(json_obj),
           "Y Test - FAILED! expected %d, got %d",
           json_object_get_int(json_obj), cpu->y.raw);

    /* acquire stack pointer */
    json_object_object_get_ex(final, "s", &json_obj);
    ASSERT(cpu->sp == json_object_get_int(json_obj),
           "SP Test - FAILED! expected %x, got %x",
           json_object_get_int(json_obj), cpu->sp);

    /* acquire processor status */
    json_object_object_get_ex(final, "p", &json_obj);
    ASSERT(cpu->flag.raw == json_object_get_int(json_obj),
           "PROCESSOR STATUS Test - FAILED! expected %x, got %x",
           json_object_get_int(json_obj), cpu->flag.raw);

    /* acquire data bank register */
    json_object_object_get_ex(final, "dbr", &json_obj);
    ASSERT(cpu->data_bank_register == json_object_get_int(json_obj),
           "DATABANK REGISTER Test - FAILED! expected %x, got %x",
           json_object_get_int(json_obj), cpu->data_bank_register);

    /* acquire direct page register */
    json_object_object_get_ex(final, "d", &json_obj);
    ASSERT(cpu->d == json_object_get_int(json_obj),
           "DIRECT PAGE Test - FAILED! expected %x, got %x",
           json_object_get_int(json_obj), cpu->d);

    /* acquire program bank register */
    json_object_object_get_ex(final, "pbr", &json_obj);
    ASSERT(cpu->prg_bank_register == json_object_get_int(json_obj),
           "PROGRAM BANK REGISTER Test - FAILED! expected %x, got %x",
           json_object_get_int(json_obj), cpu->prg_bank_register);

    /* acquire emulation mode flag */
    json_object_object_get_ex(final, "e", &json_obj);
    ASSERT(cpu->emulation_mode == (json_object_get_int(json_obj) > 0),
           "EMULATION MODE Test - FAILED! expected %d, got %d",
           json_object_get_int(json_obj) > 0, cpu->emulation_mode);

    /* acquire and check RAM state */
    json_object_object_get_ex(final, "ram", &json_obj);
    unsigned long len = json_object_array_length(json_obj);
    for (int i = 0; i < len; i++) {
        struct json_object *memory = json_object_array_get_idx(json_obj, i);
        struct json_object *address = json_object_array_get_idx(memory, 0);
        struct json_object *byte = json_object_array_get_idx(memory, 1);

        uint32_t addr = json_object_get_int(address);
        uint8_t expected_byte = json_object_get_int(byte);

        ASSERT(cpu->bus->ram[addr] == expected_byte,
               "MEMORY TEST - FAILED! Address %x: expected %02x, got %02x",
               addr, expected_byte, cpu->bus->ram[addr]);
    }
}
static void filetest(cpu_t *cpu, char *file) {
    int fd = open(file, O_RDONLY);
    ASSERT(fd != -1, "Failed to open file");

    struct stat file_info;
    if (fstat(fd, &file_info) == -1) {
        close(fd);
        // error
    }
    ASSERT(fstat(fd, &file_info) != -1, "FAILED TO GET STATS ABOUT FILE");

    off_t length = file_info.st_size;
    if (file_info.st_size == 0)
        return;
    void *buffer = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0); // map our file to memory

    /* Extract array */

    // struct json_tokener* json_tokener_new	(	void 		 ) 	[read]
    struct json_tokener *token = json_tokener_new();
    ASSERT(token, "FAILED TO CREATE TOKENIZER\n");
    struct json_object *main = json_tokener_parse_ex(token, buffer, (int)length);
    unsigned long len = json_object_array_length(main); // gets the length of the array
    printf("%s ", file);
    for (int i = 0; i < len; i++) {
        struct json_object *item = json_object_array_get_idx(main, i); // gets item
        struct json_object *name;
        /* extract name of instruction */
        json_object_object_get_ex(item, "name", &name);
        const char* namey = json_object_get_string(name);
        struct json_object *initial;
        json_object_object_get_ex(item, "initial", &initial);
        fill_initial(cpu, initial);
        cpu->cycles_left = 0;
        StepCPU(cpu);
        // printf("%s\n", json_object_get_string(name));
        json_object_object_get_ex(item, "final", &initial);
        compare_final(cpu, initial, namey);
    }
    printf(" tests - PASSED\n");
    json_tokener_free(token);
    json_object_put(main);
    munmap(buffer, length); // remove buffer
    close(fd);              // close file
}
/**
 * @brief Tests the CPU's CLC instruction
 * @param cpu - cpu reference
 * @result will fail if carry flag isn't cleared
 */
static void test2(cpu_t *cpu) {
    struct dirent *dp;
    DIR *dfd;

    char *dir = "./test";
    if ((dfd = opendir(dir)) == NULL) {
        fprintf(stderr, "Can't open %s\n", dir);
        // return 0;
        return;
    }

    char filename_qfd[100];

    while ((dp = readdir(dfd)) != NULL) {
        sprintf(filename_qfd, "%s/%s", dir, dp->d_name); // our file to open
        if (dp->d_name[0] == '.')
            continue; // prevents . and .. from entering filetest().
        filetest(cpu, filename_qfd);
    }
}

void test(cpu_t *cpu) {
    test2(cpu);
    printf("test succeed\n");
}