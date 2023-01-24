/*
    Implementation of a generic fixed size hash table.

    Collisions are circumvented via linear probing (stride of 1), i.e. we just keep walking down the 
    table and insert the new item into the leading empty slot.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#define TABLE_SIZE 10
#define ITEM_NVALS 3 // needs to be >= 3
#define MAX_KEY_SIZE 256

// derived data types



typedef struct hashtable_st
{
    uint64_t * capacity;       // max number of items
    uint64_t * nvals_per_item; // number of values per item
    uint64_t ** items;         // array of pointers 
    
} hashtable_t;




// hash table 
hashtable_t my_table = {NULL};



// simple hash funciton (Cormen's multiplication method)
uint64_t hash(const uint64_t key)
{
    
    int64_t A = rand()*2718281;
    int64_t n = 1+rand()*15;
    int64_t x = floor(A*pow(2,32))*key;
   
    int64_t hash_val = abs((x >> (32-n)) % TABLE_SIZE);  
    //printf("\n hash(%i) = %i \n", key, hash_val);
    
    return hash_val;
}


// initialization of an empty hash table 
void init_hash_table(hashtable_t *table, const void *base_ptr, const uint64_t table_capacity, const uint64_t nvals_per_item)
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
    if(table_capacity < 1 || nvals_per_item < 1)
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
bool insert_item(hashtable_t *table,  const uint64_t key, const uint64_t *values)
{
    
    void *base_ptr = table->capacity;
    uint64_t capacity = *(table->capacity);
    uint64_t nvals_per_item = *(table->nvals_per_item);
    uint64_t size_of_item = (1 + nvals_per_item) * sizeof(uint64_t); 

    printf("\n Attempting to insert new item with key = %i \n",key);
    
    // hash the index for this new item
    uint64_t index = hash(key);
    
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
int64_t lookup_item(const hashtable_t *table, const uint64_t key)
{
    
    void *base_ptr = table->capacity;
    uint64_t capacity = *(table->capacity);
    uint64_t nvals_per_item = *(table->nvals_per_item);
    uint64_t size_of_item = (1 + nvals_per_item) * sizeof(uint64_t); 
    
    printf("\n Attempting to search for item with key = %i. \n",key);  
    
    // hash index for this item
    uint64_t index = hash(key);
    
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


bool delete_item(const hashtable_t *table, const uint64_t key)
{
    
    void *base_ptr = table->capacity;
    uint64_t capacity = *(table->capacity);
    uint64_t nvals_per_item = *(table->nvals_per_item);
    uint64_t size_of_item = (1 + nvals_per_item) * sizeof(uint64_t); 
    
    printf("\n Attempting to delete item with key = %i. \n",key);  
    
    // hash index for this item
    uint64_t index = hash(key);
    
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






