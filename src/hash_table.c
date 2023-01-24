/*
   Simple Hash Table implementation. 
   
   Table represented by an array of pointers (NULL pointer indicates an empty slot). 
   Collisions are circumvented via "open addressing: linear probing", i.e. just keep walking down the 
   table and insert the new item into the leading empty slot.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// fixed parameters
#define MAX_NAME 256
#define TABLE_SIZE 10


// derived data type for "person"
typedef struct  
{

    char name[MAX_NAME];
    int age;
    // other info...

} person;



// array of person pointers
person * hash_table[TABLE_SIZE];

#define DELETED_NODE (person*)(0xFFFFFFFFFFFFFFFFUL)



// hash function
unsigned int hash(char *name)
{

    int i;
    int length = strnlen(name, MAX_NAME);
    unsigned int hash_value = 0;
   
    for(i = 0; i < length; i++)
    {
        hash_value += name[i];
        hash_value *= name[i]*name[i];
    }

    return (hash_value % TABLE_SIZE);

}


// initilize an empty hash table
void init_hash_table()
{

    int i;

    for(i = 0; i < TABLE_SIZE; i++)
    {
        hash_table[i] = NULL;  // null pointer indicates empty slot    
    }


}


void print_table()
{
 
    int i;

    printf("\n Start \n");
    for(i = 0; i < TABLE_SIZE; i++)
    {
        if(hash_table[i] == NULL)
        {
            printf("\t %i \t --- \n",i);
        }
        else if(hash_table[i] == DELETED_NODE)
        {
            printf("\t %i \t ---<deleted> \n",i);
        }        
        else
        {
            printf("\t %i \t %s \n",i,hash_table[i]->name);
        }


    }
    printf("End \n");

}


bool hash_table_insert(person *p)
{
    if(p == NULL) return false;

    // compute hash table index
    int index = hash(p->name);
     
    int i;
    
    //starting at index, walk down the table until an empty slot is found (or we get back to index). 
    for(i = 0; i< TABLE_SIZE; i++) 
    {
        int try = (i + index) % TABLE_SIZE;
        if(hash_table[try] == NULL || hash_table[try] == DELETED_NODE)
        {
            hash_table[try] = p;
            printf("\n Found empty slot. %s has been inserted into the table at location %i.\n",p->name,try);
            return true;            
        }
    }

    printf("\n WARNING!!! Unable to insert %s. Ran out of empty slots. \n",p->name);
    return false;  

}

person *hash_table_lookup(char *name)
{

    int index = hash(name);

     //starting at index, walk down the table until name has been found (or we get back to index). 
    int i;
    for(i = 0; i< TABLE_SIZE; i++) 
    {
        int try = (i + index) % TABLE_SIZE;
        
        if(hash_table[try] == NULL) break; // not found
        
        if(hash_table[try] == DELETED_NODE) continue;        
        
        if(hash_table[try] != NULL && strncmp(hash_table[try]->name, name, MAX_NAME) == 0)
        {
            return hash_table[index];
        }
    }

    return NULL;


}


person *hash_table_delete(char *name)
{
    
    // compute hash table index
    int index = hash(name);
     
    int i;
    
    //starting at index, walk down the table until name has been found (or we get back to index). 
    for(i = 0; i< TABLE_SIZE; i++) 
    {
        int try = (i + index) % TABLE_SIZE;
        
        if(hash_table[try] == NULL) break;

        if(hash_table[try] == DELETED_NODE) continue;  
        
        if(hash_table[try] != NULL && strncmp(hash_table[try]->name, name, MAX_NAME) == 0)
        {
            person *tmp = hash_table[try];
            hash_table[try] = DELETED_NODE;
            printf("%s has been deleted from the table.\n",name);
            return tmp;
        }
    }

    printf("%s is not in the table. Unable to delete.\n",name);
    return NULL; 


    
}



int main()
{

    init_hash_table();

    print_table();

    person tanzid = {.name ="Tanzid", .age=27};
    person andrew = {.name ="Andrew", .age=27};
    person kyle = {.name ="Kyle", .age=28};
    person matt = {.name ="Matt", .age=26};
    person patrick = {.name ="Patrick", .age=28};

    hash_table_insert(&tanzid);
    print_table();

    hash_table_insert(&andrew);
    print_table();

    hash_table_insert(&kyle);
    print_table();

    hash_table_insert(&matt);
    print_table();

    hash_table_insert(&patrick);
    print_table();



    /*printf("Tanzid => %u \n",hash("Tanzid"));
    printf("Andrew => %u \n",hash("Andrew"));
    printf("Kyle => %u \n",hash("Kyle"));
    printf("Matt => %u \n",hash("Matt"));
    printf("Patrick => %u \n",hash("Patrick")); */
  
    person *tmp = hash_table_lookup("Tanzid");
  
    if(tmp == NULL)
    {
        printf("Tanzid not found. \n");
    }
    else
    {
        printf("Found %s. \n",tmp->name);
    }

    tmp = hash_table_lookup("Nova");
  
    if(tmp == NULL)
    {
        printf("Nova not found. \n");
    }
    else
    {
        printf("Found %s. \n",tmp->name);
    }
    
    tmp = hash_table_delete("Patrick");
    print_table();

    tmp = hash_table_delete("Kyle");
    print_table();

    tmp = hash_table_delete("Andrew");
    print_table();


    hash_table_delete("Tom");



    return 0;

}

