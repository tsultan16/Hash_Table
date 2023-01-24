
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



// 64-bit MurMur hash 2 function for arbitrary key size (set seed = 0 if no preference) 
uint64_t murmur_hash_2(const void * key, uint64_t len, uint64_t seed, uint64_t table_size)  //len is the byte length of the key
{
    
    // mixing constants
	const uint64_t m = 0xc6a4a7935bd1e995;
    const uint32_t r = 47;
   
    
    // initialize the hash to some "random" value
    uint64_t hash_val = seed ^ (len * m);  
    
    const uint64_t * data = (const uint64_t *) key;  //casting the key not necessary if it's already 64 bits, but not assuming that for now  
    const uint64_t * end  = data + (len / 8);
    
    //printf("\n data (char) = %c \n",data[0]);
    //printf("\n data  = %i \n",*(uint32_t *)data);
    
    // go through 8 bytes at a time and mix it up at the bit-level
    while (data != end)
    {
        uint64_t k = *(data++); // get 8-byte chunk out of the key and advance data pointer 
        
        k *= m;
        k ^= k >> r;
        k *= m; 
        
        hash_val ^= k;
        hash_val *= m;
              
    }
    
    // now mix up remaining few (<=7) bytes of key   (for 64 bit keys, this is part not needed)
    const unsigned char * data2 = (const unsigned char *) data;
    
	switch(len & 7)
	{
        case 7: hash_val ^= ((uint64_t)data2[6]) << 48;
        case 6: hash_val ^= ((uint64_t)data2[5]) << 40;
        case 5: hash_val ^= ((uint64_t)data2[4]) << 32;
        case 4: hash_val ^= ((uint64_t)data2[3]) << 24;
        case 3: hash_val ^= ((uint64_t)data2[2]) << 16;
        case 2: hash_val ^= ((uint64_t)data2[1]) << 8;
        case 1: hash_val ^= ((uint64_t)data2[0]);    
        hash_val *= m;
	};
 
	hash_val ^= hash_val >> r;
	hash_val *= m;
	hash_val ^= hash_val >> r;
    
    return (hash_val % table_size);

}



int main()
{

    uint64_t key, i;
    uint32_t hash32;
    uint64_t hash64;
    uint64_t table_size = 1000;
    
    for(i=0; i<20; i++)
    {
        key = i+1; //10000*rand();
        //hash32 = murmur_hash(&key, 8U, 0U);  // 8 byte key data, seed = 0
        hash64 = murmur_hash_2(&key, 8UL, 0UL, table_size);  // 8 byte key data, seed = 0
        printf("\n Key = %i, Hashval = %i \n",key,hash64);
    }
    //printf("\n key (int)  = %i \t %i \n",key[0], key[1]);
    //printf("\n key address = %p \n",&key);
    
    
    printf("\n Done! \n");

    return 0;

}
