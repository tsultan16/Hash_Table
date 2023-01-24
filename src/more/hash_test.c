/*
    Implementation of a generic fixed size hash table.

    Collisions are circumvented via linear probing (stride of 1), i.e. we just keep walking down the 
    table and insert the new item into the leading empty slot.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

# include "hash.h"


#define TABLE_SIZE 10
#define ITEM_NVALS 3 // needs to be >= 3



void print_table(const hashtable_t *table)
{
    int i, j, offset;
    void *base_ptr = table->capacity;
    uint64_t capacity = *(table->capacity);
    uint64_t nvals_per_item = *(table->nvals_per_item);
    uint64_t size_of_item = (1 + nvals_per_item) * sizeof(uint64_t); 
    

    if(table == NULL)
    {
        printf("\n Invalid  table pointer. \n");
        return;  
    }    

    printf("\n Table capacity = %i, nvals_per_item =  %i \n",*(table->capacity),*(table->nvals_per_item));
    printf("\n-------------------------------------------------------\n");
    uint64_t *p;
    uint64_t slot_status;
    
    for(i = 0; i< capacity; i++)
    {
        p = base_ptr + 3*sizeof(uint64_t) + i*size_of_item;

        // get status of this slot
        slot_status = p[0]; 
   
        if(slot_status == 0)   
        {
            printf("\t %i \t ---<empty> \n",i);
        }
        else if(slot_status == 1)
        {
            printf("\t %i \t key = %i \t val = %i \n",i,p[1],p[2]);
        }
        else if(slot_status == 2)
        {
            printf("\t %i \t ---<deleted> \n",i);
        }
    
    }
    printf("-------------------------------------------------------\n");  
    
}



int main()
{

    // hash table 
    hashtable_t my_table = {NULL};


    uint64_t table_capacity = TABLE_SIZE;
    uint64_t nvals_per_item = ITEM_NVALS;
    uint64_t item_bytes, table_bytes;    
    void * base_ptr;
    
    
    // allocate memory for hash table
    item_bytes = (1 + nvals_per_item) * sizeof(uint64_t); 
    table_bytes = 2 * sizeof(uint64_t)  +  table_capacity *  item_bytes ;     
    printf("\n Table capacity = %i, nvals_per_item = %i, table_bytes = %i \n",table_capacity,nvals_per_item,table_bytes);

    base_ptr = malloc(table_bytes);
    
    // initialize hash table
    init_hash_table(&my_table, base_ptr, table_capacity, nvals_per_item);


    // testing...
    
    print_table(&my_table);
 
    
    uint64_t i = 1;
    for(i = 1; i<=10; i++)
    {    
       uint64_t key = i;
       uint64_t val = abs(rand())%1000;
       insert_item(&my_table,key,&val);
       print_table(&my_table);

    }   
 
    
    int64_t find = lookup_item(&my_table, 4);

    bool found = delete_item(&my_table, 10);
    print_table(&my_table);

    found = delete_item(&my_table, 7);
    print_table(&my_table);


	return 0;
}






