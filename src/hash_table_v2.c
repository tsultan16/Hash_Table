/*
    Implementation of a generic fixed size hash table.

    Collisions are circumvented via linear probing (stride of 1), i.e. we just keep walking down the 
    table and insert the new item into the leading empty slot.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 10
#define MAX_KEY_SIZE 256

// derived data type for hash table items
typedef struct
{
    unsigned int status;     // item status (0 = empty, 1 = occupied, 2 = deleted)     
    char key[MAX_KEY_SIZE];  // item key string
    int value;               // item value
    
} item;

// hash table
item hash_table[TABLE_SIZE];



// hash funciton
unsigned int hash(char *key)
{
    
    int i;
    int len = strnlen(key,MAX_KEY_SIZE);
    unsigned int hash_val = 0;
    
    for(i = 0; i < len; i++)
    {
        hash_val += key[i];
        hash_val *= key[i]*key[i];
        
        //printf("\n i = %i, key[i] = %c, (int)(key[i])= %i \n",i,key[i],(unsigned int)(key[i]));
    }
    
    return (hash_val % TABLE_SIZE);
}


void init_hash_table()
{
    int i;
    
    // initialize all slots as empty
    for(i = 0; i < TABLE_SIZE; i++)
    {
        hash_table[i].status = 0;
    }

}


void print_table()
{
    int i;
    
    printf("\nTable start: \n");
    for(i = 0; i < TABLE_SIZE; i++)
    {
        if(hash_table[i].status == 0)
        {
            printf("\t %i \t ---<empty> \n",i);
        }
        else if(hash_table[i].status == 1)
        {
            printf("\t %i \t %s \n",i,hash_table[i].key);
        }
        else if(hash_table[i].status == 2)
        {
            printf("\t %i \t ---<deleted> \n",i);
        }
    }
    printf("Table end.\n");
    
    
}


bool insert_item(item p)
{
    
    // hash index for this item
    printf("Attempting to insert new item: key = %s, value = %i \n",p.key, p.value);
    unsigned int index = hash(p.key);
    
    int i;
    
    //starting at index, walk down table and place item into leading empty slot
    for(i=0; i< TABLE_SIZE; i++)
    {
        unsigned int try = (index + i) % TABLE_SIZE;
                
        if(hash_table[try].status != 1) 
        {
            hash_table[try] = p;
            hash_table[try].status = 1;
            printf("\n Found empty slot. %s has been inserted at location %i. \n",p.key,try);
            return true;
        }            
        
    }
    
    printf("\n Warning! Unable to insert new item with key: %s. Ran out of empty slots. \n",p.key);
    return false;
    
}



item lookup_item(char *key)
{
    
    // hash index for this item
    printf("\nAttempting to search for item with key = %s. \n",key);
    unsigned int index = hash(key);
    
    int i;
    
    //starting at index, walk down table and search for the item
    for(i=0; i< TABLE_SIZE; i++)
    {
        unsigned int try = (index + i) % TABLE_SIZE;
                
        if(hash_table[try].status == 0) break; // item not found
 
        if(hash_table[try].status == 2) continue;
        
        if(hash_table[try].status == 1 && strncmp(hash_table[try].key,key,MAX_KEY_SIZE)==0)
        {
            printf("Found %s at location %i. \n",key,try);
            return hash_table[try];
        }            
        
    }
    
    printf("Item does not exist. \n");
    item tmp;
    return tmp;
    
}


bool delete_item(char *key)
{
    
    // hash index for this item
    printf("\nAttempting to delete item with key = %s. \n",key);
    unsigned int index = hash(key);
    
    int i;
    
    //starting at index, walk down table and search for the item
    for(i=0; i< TABLE_SIZE; i++)
    {
        unsigned int try = (index + i) % TABLE_SIZE;
                
        if(hash_table[try].status == 0) break; // item not found
 
        if(hash_table[try].status == 2) continue;
        
        if(hash_table[try].status == 1 && strncmp(hash_table[try].key,key,MAX_KEY_SIZE)==0)
        {
            printf("Deleting %s at location %i. \n",key,try);
            hash_table[try].status = 2;
            return true;
        }            
        
    }
    
    printf("Item does not exist. \n");
    return false;
    
}


int main()
{

    print_table();

    item tanzid = {.status = 0, .key="Tanzid", .value=27};
    item andrew = {.status = 0, .key="Andrew", .value=27};
    item kyle = {.status = 0, .key="Kyle", .value=28};
    item matt = {.status = 0, .key="Matt", .value=26};
    item patrick = {.status = 0, .key="Patrick", .value=27};
        
    insert_item(tanzid);
    print_table();
    
    insert_item(andrew);
    print_table();
    
    insert_item(kyle);
    print_table();
    
    insert_item(matt);
    print_table();

    insert_item(patrick);
    print_table();

    item tmp;
    tmp = lookup_item("Tanzid");    
    tmp = lookup_item("Makoto");


    delete_item("Tom");
    delete_item("Kyle");    
    print_table();

    delete_item("Patrick");
    print_table();



	return 0;
}






