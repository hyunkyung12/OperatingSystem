#include "ku_malloc.h"

hnode_t *getPrev(hnode_t *temp)
{
    hnode_t *head = free_list;

    if (head == temp) return head;

    while(head->next != temp)
    {
        head = head -> next;
    }
    return head;
}

void *nextFit(void *ptr, size_t size) 
{
    hnode_t *temp = ptr;
    size = size + sizeof(hheader_t);

    while(temp != NULL)
    {
        if( size <= temp->size ) return temp;
        temp = temp->next;
    }

    temp = free_list;

    while(temp != ptr)
    {
        if( size <= temp->size ) return temp;
        temp = temp->next;       
    }

    return NULL;
}


void *malloc(size_t size)
{
    void *ptr;
    void *fit;
    hheader_t *header;
    hnode_t *prev;
    hnode_t *temp;
    size_t real = size + sizeof(hheader_t);

    // Case 1 : size = 0
    if( size == 0 ){
        fprintf(stderr, "[Alloc] Addr: %p Length: 0\n", ptr);
        return NULL;
    }

    // Case 2 : free_list == NULL (free 없이 처음 malloc)
    if( free_list == NULL ){
        header = sbrk(sizeof(hheader_t));
        header -> size = size;
        header -> magic = KU_MAGIC ^ (unsigned int)ptr;
        ptr = sbrk(size);
    }

    // Case 3 : free_list != NULL
    else{

        fit = nextFit(prev_ptr,real);

        // Case 3-1 : free_list 에 size + header 를 할당할 자리가 없을 때
        if(fit == NULL)
        {
            header = sbrk(sizeof(hheader_t));
            header -> size = size;
            header -> magic = KU_MAGIC ^ (unsigned int)((void *)fit + sizeof(hheader_t));
            ptr = sbrk(size);   
        }

        // Case 3-2 : free_list 에 size + header 를 할당할 자리가 있을 때
        else
        {
            temp = (hnode_t *)((void *)fit + real);
            prev = getPrev((hnode_t *)fit);

            // Case 3-2.1 : free_list 의 자리와 size + header 의 크기가 동일
            if(temp->size == real)
            { 
                temp->size = real;
                temp->next = ((hnode_t *)fit) -> next;
                prev->next = temp;

                ((hheader_t *)fit)->size = size;
                ((hheader_t *)fit)->magic = KU_MAGIC ^ (unsigned int)((void *)fit + sizeof(hheader_t));

                ptr = (void *)fit + sizeof(hnode_t);
                prev_ptr = prev->next;
            }

            // Case 3-2.2 : free_list 의 자리가 size + header 의 크기보다 클 때
            else if(temp->size > real)
            {
                temp->size = (((hnode_t *)fit)->size) - real;
                temp->next = ((hnode_t *)fit)->next;
                prev->next = (hnode_t *)((void *)fit + real);

                ((hheader_t *)fit)->size = size;
                ((hheader_t *)fit)->magic = KU_MAGIC ^ (unsigned int)((void *)fit + sizeof(hheader_t));

                ptr = (void *)fit + sizeof(hnode_t);
                prev_ptr = prev->next->next;
            }

        }

    }

    if( ptr > 0 ){
        fprintf(stderr, "[Alloc] Addr: %p Length: %lu\n", ptr, size);
        return ptr;
    }
    else{
        fprintf(stderr, "[Alloc] Addr: NULL Length: %lu\n", size);
        return NULL;
    }
}

void free(void *ptr)
{
    size_t size;
    unsigned long magic;

    hheader_t *temp = (hheader_t *)((void *)ptr - sizeof(hheader_t));
    hnode_t *new = ((hnode_t *)((void *)ptr - sizeof(hnode_t)));
    hnode_t *prev = free_list;

    size = temp->size;
    magic = temp->magic;

    if( free_list == NULL )
    {
        if(( temp->magic ) == (KU_MAGIC ^ (unsigned int)ptr))
        {
            free_list = new;
            free_list->size = size;
            free_list->next = NULL;
        }
    }
    else
    {
        if(( temp->magic ) == (KU_MAGIC ^ (unsigned int)ptr))
        {
            new->size = size;
            new->next = free_list;
            free_list = new;   
        }


    }

    fprintf(stderr, "[Free]  Addr: %p Length: %zu\n", ptr, size);
}