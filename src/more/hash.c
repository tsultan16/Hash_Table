/*
    Implementation of a generic fixed size hash table.

    Collisions avoidance via linear probing (stride of 1), i.e. just keep walking down the 
    table and insert the new item into the leading empty slot.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

# include "hash.h"



// simple hash funciton (Cormen's multiplication method)
uint64_t 
hash(const uint64_t key, const uint64_t table_capacity)
{
    
    int64_t A = rand()*2718281;
    int64_t n = 1+rand()*15;
    int64_t x = floor(A*pow(2,32))*key;
   
    int64_t hash_val = abs((x >> (32-n)) % table_capacity);  
    //printf("\n hash(%i) = %i \n", key, hash_val);
    
    return hash_val;
}


// initialization of an empty hash table 
void 
init_hash_table(hashtable_t *table, const void *base_ptr, const uint64_t table_capacity, const uint64_t nvals_per_item)
{
    int i, j, offset;
    uint64_t size_of_item = (1 + nvals_per_item) * sizeof(uint64_t) ; 
 
    /* make sure we're getting a valid base_ptr */
    if(base_ptr == NULL)
    {
        printf("\n Invalid base_ptr provided. Unable to initilize hash table. \n");

          return;  
    }    
       
    /* make sure table_capacity and nvals_per_item are also valid */
    if(table_capacity < 1 || nvals_per_item < 3)
    {
        printf("\n Invalid table_capacity or nvals_per_item. Unable to initilize hash table. \n");

        return;  
    }    
              
    printf("\n Initializing hash table... \n");
   
    /* map table into memory via the base_ptr */ 
    table->capacity       = base_ptr;
    table->nvals_per_item = base_ptr + sizeof(uint64_t);
    table->items = malloc(sizeof(uint64_t *) * table_capacity); // allocate memory for an array of item pointers    
   

    //printf("\n table capacity pointer       = %p \n",table->capacity);
    //printf("\n table nvals_per_item pointer = %p \n",table->nvals_per_item);   
    
    /* set table attributes */
    *(table->capacity) = table_capacity;   
    *(table->nvals_per_item) = nvals_per_item;  
    
    
    for(i = 0; i< table_capacity; i++)
    {
        offset = i*size_of_item;    
        table->items[i] = base_ptr + 2*sizeof(uint64_t) + offset;
        //printf("\n i = %i, offset =  %i \n",i,offset);
        //printf("\n item address = %p \n",table->items[i]);
        uint64_t *p = base_ptr + 3*sizeof(uint64_t) + offset;


        for(j = 0; j < nvals_per_item; j++)
        {
            //uint64_t *p = base_ptr + 2*sizeof(uint64_t) + offset + (j+1)*sizeof(uint64_t);
            p[j] = (uint64_t) 0;
            //printf("\n Value # %i = %i, value address = %p \n",j+1,*p,p);
                  
        } 
        
    }
    

}


// insert a new item into the table
bool 
insert_item(hashtable_t *table,  const uint64_t key, const uint64_t *values)
{
    
    assert(table != NULL);
    
    void *base_ptr = table->capacity;
    uint64_t capacity = *(table->capacity);
    uint64_t nvals_per_item = *(table->nvals_per_item);
    uint64_t size_of_item = (1 + nvals_per_item) * sizeof(uint64_t); 

    printf("\n Attempting to insert new item with key = %i \n",key);
    
    // hash the index for this new item
    uint64_t index = hash(key, capacity);
    
    uint64_t i, j, try, slot_status;
    uint64_t *p; // = base_ptr + 3*sizeof(uint64_t);

    
    /*starting at index, traverse down the table and place item in leading empty slot */
    for(i=0; i< capacity; i++)
    {
        uint64_t try = (index + i) % capacity;
        
        p = base_ptr + 3*sizeof(uint64_t) + try*size_of_item;

        // get status of this slot
        slot_status = p[0]; 
        
        printf("\n try = %i, status = %i \n",try,slot_status);
        
        if(slot_status==1) printf("\n Collision detected! \n");
        
        if(slot_status != 1) 
        {
            printf("\n Found empty slot at location %i \n",try);
 
            p[0] = 1;   // set status to occupied
            p[1] = key; // set key
            for(j=0;j<nvals_per_item-2;j++)
            {
               p[2+j] = values[j]; // set remaining values
            }                
            
            printf("\n Item has been inserted. \n");
            return true;
        }            
        
    }
    
    printf("\n Warning! Unable to insert new item with key: %i. Ran out of empty slots. \n",key);
    return false;  
    
}


// find and return the location of item with given key
int64_t 
lookup_item(const hashtable_t *table, const uint64_t key)
{
    
    assert(table != NULL);

    void *base_ptr = table->capacity;
    uint64_t capacity = *(table->capacity);
    uint64_t nvals_per_item = *(table->nvals_per_item);
    uint64_t size_of_item = (1 + nvals_per_item) * sizeof(uint64_t); 
    
    printf("\n Attempting to search for item with key = %i. \n",key);  
    
    // hash index for this item
    uint64_t index = hash(key, capacity);
    
    uint64_t  i, slot_status;
    uint64_t  *p;
    
    for(i=0; i< capacity; i++)
    {
        uint64_t try = (index + i) % capacity;
        
        p = base_ptr + 3*sizeof(uint64_t) + try*size_of_item;

        // get status of this slot
        slot_status = p[0]; 
        
        //printf("\n try = %i, status = %i \n",try,slot_status);

        if(slot_status == 0) break;
        
        if(slot_status == 2) continue;
        
        if((slot_status == 1) && (p[1] == key)) 
        {
            printf("\n Found item with key = %i at location %i \n",key,try);
            return try;
        }            
        
    }    
    
    
    printf("Item does not exist. \n");
    return (-1);   
    
}


bool 
delete_item(const hashtable_t *table, const uint64_t key)
{

    assert(table != NULL);

    void *base_ptr = table->capacity;
    uint64_t capacity = *(table->capacity);
    uint64_t nvals_per_item = *(table->nvals_per_item);
    uint64_t size_of_item = (1 + nvals_per_item) * sizeof(uint64_t); 
    
    printf("\n Attempting to delete item with key = %i. \n",key);  
    
    // hash index for this item
    uint64_t index = hash(key, capacity);
    
    uint64_t  i, slot_status;
    uint64_t  *p;
    
    for(i=0; i< capacity; i++)
    {
        uint64_t try = (index + i) % capacity;
        
        p = base_ptr + 3*sizeof(uint64_t) + try*size_of_item;

        // get status of this slot
        slot_status = p[0]; 
        
        //printf("\n try = %i, status = %i \n",try,slot_status);

        if(slot_status == 0) break;
        
        if(slot_status == 2) continue;
        
        if((slot_status == 1) && (p[1] == key)) 
        {
            printf("\n Deleting item with key = %i at location %i \n",key,try);
            p[0] = 2; // set status to deleted
            return true;
        }            
        
    }    
    
    printf("Item does not exist. \n");
    return false;
    
}





