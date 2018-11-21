#define _GNU_SOURCE

#include<stdio.h>
#include <unistd.h>

#define KU_MAGIC 0x19311946U

typedef struct __hnode_t
{
    size_t size;
    struct __hnode_t *next;

}hnode_t;

typedef struct __hheader_t
{
    size_t size;
    unsigned long magic;

}hheader_t;   

hnode_t *free_list;
hnode_t *prev_ptr;

