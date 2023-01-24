
// MurMur Hash 1 implementation (same as Appleby code)


#include <stdio.h>
#include <stdint.h>
#include <math.h>


// 32-bit MurMur hash function for arbitrary key size (set seed = 0 if no preference) 
uint32_t murmur_hash(const void * key, uint32_t len, uint32_t seed)  //len is the byte length of the key
{
    
    // mixing constants
    const uint32_t m = 0xc6a4a793;
    const uint32_t r = 16;
    
    
    
    // initialize the hash to some "random" value
    uint32_t hashval = seed ^ (len*m);  
    
    const unsigned char * data = (const unsigned char *) key;
    
    
    //printf("\n data (char) = %c \n",data[0]);
    //printf("\n data  = %i \n",*(uint32_t *)data);
    
    
    // go through 4 bytes at a time and mix it up
    while (len >= 4)
    {
        hashval +=  *(uint32_t *)data;
        hashval *= m;
        hashval ^= hashval >> r; 
        
        data += 4; // advance data pointer onto next 4 byte block 
        len -= 4;          
    }
    
    // now mix up remaining few bytes of key
    switch(len)
    {
        case 1: 
            hashval+= data[0];
            hashval*= m;
            hashval ^= hashval >> r; 
        case 2:
            hashval += data[1] << 8;
        case 3:
            hashval += data[2] << 16;       
    }
    
    return hashval;
}



int main()
{

    uint64_t key, i;
    uint32_t hash;
    
    for(i=0; i<10; i++)
    {
        key = 10000*rand();
        hash = murmur_hash(&key, 8U, 0U);  // 8 byte key data, seed = 0
        printf("\n Key = %i, Hashval = %i \n",key,hash);
    }
    //printf("\n key (int)  = %i \t %i \n",key[0], key[1]);
    //printf("\n key address = %p \n",&key);
    
    
    printf("\n Done! \n");

    return 0;

}
