#include "HashTable.h"

#include <cassert>
#include <iostream>
#include <iomanip>

const int KeyedHashTable::PRIME_LIST[PRIME_TABLE_COUNT] =
{
     2,    3,   5,   7,  11,  13,  17,  19,
     23,  29,  31,  37,  41,  43,  47,  53,
     59,  61,  67,  71,  73,  79,  83,  89,
     97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311,
    313, 317, 331, 337, 347, 349, 353, 359,
    367, 373, 379, 383, 389, 397, 401, 409,
    419, 421, 431, 433, 439, 443, 449, 457,
    461, 463, 467, 479, 487, 491, 499, 503,
    509, 521, 523, 541
};

int KeyedHashTable::Hash(const std::string& key) const
{
    // TODO
    int i;
    int hash_value = 0;
    for (i=0;i<key.length();i++){
        hash_value += (((int)key[i])*PRIME_LIST[i]);
    }
    hash_value = hash_value % tableSize;
    return hash_value;
}

void KeyedHashTable::ReHash()
{
    // TODO
    int i,oldSize = tableSize;
    HashData* new_table = new HashData[FindNearestLargerPrime(2*oldSize)];
    tableSize = FindNearestLargerPrime(2*oldSize);
    for (i=0;i<oldSize;i++){
        if (table[i].key != "__DELETED__" && table[i].key != "" && table[i].key.empty() == false){
            int place = Hash(table[i].key);
            int quadratic_probing = 1;
            while (1){
                if ((new_table[place].key == "__DELETED__" || new_table[place].key == "") && (new_table[place].intArray.empty() == true)) break;
                else {
                    place += quadratic_probing;
                    quadratic_probing += 2;
                }
            }
            new_table[place] = table[i];
        }
    }
    delete[] table;
    table = new_table;

}

int KeyedHashTable::FindNearestLargerPrime(int requestedCapacity)
{
    // TODO
    int i;
    for (i=0;i<100;i++){
        if (requestedCapacity < PRIME_LIST[i]){
            return PRIME_LIST[i];
        }
    }
}

KeyedHashTable::KeyedHashTable()
{
    // TODO
    tableSize = 2;
    occupiedElementCount = 0;
    table = new HashData[2];
    int i;
    for (i=0;i<tableSize;i++){
        struct HashData new_data;
        new_data.intArray.clear();
        new_data.key = "";
        table[i] = new_data;
    }
}

KeyedHashTable::KeyedHashTable(int requestedCapacity)
{
    // TODO
    tableSize = FindNearestLargerPrime(requestedCapacity);
    occupiedElementCount = 0;
    table = new HashData[FindNearestLargerPrime(requestedCapacity)];
    int i;
    for (i=0;i<tableSize;i++){
        struct HashData new_data;
        new_data.intArray.clear();
        new_data.key = "";
        table[i] = new_data;
    }
}

KeyedHashTable::KeyedHashTable(const KeyedHashTable& other)
{
    // TODO
    tableSize = other.tableSize;
    occupiedElementCount = other.occupiedElementCount;
    table = new HashData[tableSize];
    int i;
    for (i=0;i<tableSize;i++) table[i] = other.table[i];

}

KeyedHashTable& KeyedHashTable::operator=(const KeyedHashTable& other)
{
    // TODO
    tableSize = other.tableSize;
    occupiedElementCount = other.occupiedElementCount;
    delete[] table;
    table = new HashData[tableSize];
    int i;
    for (i=0;i<tableSize;i++) table[i] = other.table[i];
    return *(this);

}

KeyedHashTable::~KeyedHashTable()
{
    // TODO
    delete[] table;
}

bool KeyedHashTable::Insert(const std::string& key,
                            const std::vector<int>& intArray)
{
    // TODO
    int i;
    bool sameKey = false;
    for (i=0;i<tableSize;i++){
        if (table[i].key == key){
            sameKey = true;
            break;
        }
    }
    
    if (sameKey == true) return false;
    else {
        struct HashData new_data;
        new_data.key = key;
        new_data.intArray = intArray;
        
        int place = Hash(key);
        int quadratic_probing = 1;
        while (1){
            if ((table[place].key == "__DELETED__" || table[place].key == "") && (table[place].intArray.empty() == true)) break;
            else {
                place += quadratic_probing;
                quadratic_probing += 2;
            }
        }
        table[place] = new_data;
        occupiedElementCount++;
        while ((occupiedElementCount * EXPAND_THRESHOLD) >= tableSize) ReHash();
    }
}

bool KeyedHashTable::Remove(const std::string& key)
{
    // TODO
    int i;
    bool isExist = false;
    for (i=0;i<tableSize;i++){
        if (table[i].key == key){
            isExist = true;
        }
    }
    
    if (isExist == false) return false;
    else{
        for (i=0;i<tableSize;i++){
            if (table[i].key == key){
                table[i].key = "__DELETED__";
                table[i].intArray.clear();
            }
        }
        occupiedElementCount--;
        return true;
    }
    
}

void KeyedHashTable::ClearTable()
{
   // TODO
   int i;
   for (i=0;i<tableSize;i++){
       table[i].key = "";
       table[i].intArray.clear();
   }
   occupiedElementCount = 0;
}

bool KeyedHashTable::Find(std::vector<int>& valueOut,
                          const std::string& key) const
{
    // TODO
    int i;
    int isExist = false;
    for (i=0;i<tableSize;i++){
        if (table[i].key == key){
            isExist = true;
        } 
    }
    
    if (isExist == false) return false;
    else {
        for (i=0;i<tableSize;i++){
            if (table[i].key == key){
                valueOut = table[i].intArray;
            }
        }
        return true;
    }
}
void KeyedHashTable::Print() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    std::cout << "HT:";
    if(occupiedElementCount == 0)
    {
        std::cout << " Empty";
    }
    std::cout << "\n";
    for(int i = 0; i < tableSize; i++)
    {
        if(table[i].key == "") continue;

        std::cout << "[" << std::setw(3) << i << "] ";
        std::cout << "[" << table[i].key << "] ";
        std::cout << "[";
        for(size_t j = 0; j < table[i].intArray.size(); j++)
        {
            std::cout << table[i].intArray[j];
            if((j + 1) != table[i].intArray.size())
                std::cout << ", ";
        }
        std::cout << "]\n";
    }
    std::cout.flush();
}









