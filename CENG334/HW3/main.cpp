#include <string.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "ext2fs.h"
#include <fcntl.h>
#include <vector>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm> 
#include <queue>

using namespace std;

#define CEIL(num, denom) ((num) / (denom) + ((num) % (denom) ? 1U : 0U))
#define EXT2_DIR_LENGTH(name_length) (sizeof(struct ext2_dir_entry) + CEIL(name_length, 4U) * 4U)

vector<char*> tokenizePath(char* path){
    // Trim beginning /s and end /s.
    int size = 0;
    for (int i=0;path[i] != '\0'; i++) size++;
    size++;
    int b = 0;
    int e = size-1;
    for (int i=0;path[i] == '/'; i++) b++;
    for (int i=size-1;path[i] == '\0' || path[i] == '/'; i--) e--;
    vector<char*> tokens;
    bool flag = true;
    vector<char> word;

    // Tokenize
    for (int i=b;i<=e;i++){
        if (flag == true && path[i] != '/') word.push_back(path[i]);
        else if (flag == true && path[i] == '/'){
            char* new_word = new char[word.size() + 1];
            new_word[word.size()] = '\0';
            for (int j=0;j<word.size();j++) new_word[j] = word[j];
            flag = false;
            word.clear();
            tokens.push_back(new_word);
        }
        else if (flag == false && path[i] != '/'){
            word.push_back(path[i]);
            flag = true;
        }
        else if (flag == false && path[i] == '/') continue;
    }
    if (word.size() != 0){
        char* new_word = new char[word.size() + 1];
        new_word[word.size()] = '\0';
        for (int j=0;j<word.size();j++) new_word[j] = word[j];
        flag = false;
        word.clear();
        tokens.push_back(new_word);       
    }

    return tokens;
}

void BIT_CLEAR(char*& bitmap, uint32_t offset){*((bitmap) + ((int) ((offset) / 8))) &= ~(1 << ((int) ((offset) % 8)));}
bool IS_BIT_SET(char*& bitmap, uint32_t offset){return *((bitmap) + ((int) ((offset) / 8))) & (1 << ((int) ((offset) % 8)));}
void BIT_SET(char*& bitmap, uint32_t offset){*((bitmap) + ((int) ((offset) / 8))) |= (1 << ((int) ((offset) % 8)));}

typedef struct ext2_super_block ext2_super_block;
typedef struct ext2_block_group_descriptor ext2_block_group_descriptor;
typedef struct ext2_inode ext2_inode;
typedef struct ext2_dir_entry ext2_dir_entry;

uint32_t INODE_COUNT = 0;
uint32_t BLOCK_SIZE = 0;
uint32_t BLOCKS_PER_GROUP = 0;
uint32_t INODES_PER_GROUP = 0;
uint16_t INODE_SIZE = 0;
uint32_t GROUP_SIZE = 0;
uint32_t GROUP_COUNT = 0;
uint32_t POINTER_PER_INDIRECT_BLOCK = 0;
uint32_t BLOCK_COUNT = 0;

ext2_super_block* super_block;
char* DISK = NULL;

int operation = -1;


void readSuperBlock(){
    super_block = (ext2_super_block*) &DISK[EXT2_BOOT_BLOCK_SIZE];
    BLOCK_SIZE = pow(2, 10 + super_block->log_block_size);
    INODE_COUNT = super_block->inode_count;
    BLOCKS_PER_GROUP = super_block->blocks_per_group;
    INODES_PER_GROUP = super_block->inodes_per_group;
    INODE_SIZE = super_block->inode_size;
    GROUP_SIZE = BLOCKS_PER_GROUP * BLOCK_SIZE;
    GROUP_COUNT = INODE_COUNT / INODES_PER_GROUP;
    POINTER_PER_INDIRECT_BLOCK = BLOCK_SIZE / sizeof(uint32_t);
    BLOCK_COUNT = BLOCKS_PER_GROUP * GROUP_COUNT;
}

char *Block(int i){return &DISK[BLOCK_SIZE*(i)];}

ext2_block_group_descriptor* getBlockGroupDescriptorByGroupNo(int i){return (ext2_block_group_descriptor*) &DISK[BLOCK_SIZE + i*sizeof(ext2_block_group_descriptor)];}

ext2_inode* getInodeByInodeNo(int i){
    int group_no = (i-1)/INODES_PER_GROUP;
    int local_index = i - 1 - group_no * INODES_PER_GROUP;
    ext2_block_group_descriptor* gd = getBlockGroupDescriptorByGroupNo(group_no);
    char* inode_table = Block(gd->inode_table);
    return (ext2_inode*) (inode_table + local_index * INODE_SIZE);
}

bool isPathExist(vector<char*> tokens){
    bool flag = true;
    ext2_inode* current_inode = getInodeByInodeNo(2);
    for (int i=0;i<tokens.size();i++){
        int inode_no;
        bool isCurrentTokenFound = false;
        char* current_token = tokens[i];
        
        bool localFlag = false;;
        // Check Direct Blocks
        for (int j=0;j<12;j++){
            if (current_inode->direct_blocks[j] == 0) continue;
            else{
                char* block = Block(current_inode->direct_blocks[j]);
                ext2_dir_entry* tempDirEntry;
                for (int k=0;k<BLOCK_SIZE;k+=tempDirEntry->length){
                    tempDirEntry = (ext2_dir_entry*) &block[k];
                    if (!strncmp(current_token, tempDirEntry->name, tempDirEntry->name_length)){
                        isCurrentTokenFound = true;
                        inode_no = tempDirEntry->inode;
                        localFlag = true;
                    }
                }
            }
        }

        // Check Single Indirect Blocks
        if ((current_inode->single_indirect != 0) && (localFlag == false)){
            uint32_t* singlePointerBlock = (uint32_t*) Block(current_inode->single_indirect);
            for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                if (singlePointerBlock[j] == 0) continue;
                else{
                    char* block = Block(singlePointerBlock[j]);
                    ext2_dir_entry* tempDirEntry;
                    for (int k=0;k<BLOCK_SIZE;k+=tempDirEntry->length){
                        tempDirEntry = (ext2_dir_entry*) &block[k];
                        if (!strncmp(current_token, tempDirEntry->name, tempDirEntry->name_length)){
                            isCurrentTokenFound = true;
                            inode_no = tempDirEntry->inode;
                            localFlag = true;
                        }
                    }
                }
            }
        }

        // Check Double Indirect Blocks
        if ((current_inode->double_indirect != 0) && (localFlag == false)){
            uint32_t* doublePointerBlock = (uint32_t*) Block(current_inode->double_indirect);
            for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                if (doublePointerBlock[j] == 0) continue;
                else {
                    uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[j]);
                    for (int k=0;k<POINTER_PER_INDIRECT_BLOCK;k++){
                        if (singlePointerBlock[k] == 0) continue;;
                        char* block = Block(singlePointerBlock[k]);
                        ext2_dir_entry* tempDirEntry;                    
                        for (int l=0;l<BLOCK_SIZE;l+=tempDirEntry->length){
                            tempDirEntry = (ext2_dir_entry*) &block[l];
                            if (!strncmp(current_token, tempDirEntry->name, tempDirEntry->name_length)){
                                isCurrentTokenFound = true;
                                inode_no = tempDirEntry->inode;
                                localFlag = true;
                            }
                        }
                    }
                }
            }
        }
        // Check Triple Indirect Blocks
        if ((current_inode->triple_indirect != 0) && (localFlag == false)){
            uint32_t* triplePointerBlock = (uint32_t*) Block(current_inode->triple_indirect);
            for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                if (triplePointerBlock[j] == 0) continue;
                else {
                    uint32_t* doublePointerBlock = (uint32_t*) Block(triplePointerBlock[j]);
                    for (int k=0;k<POINTER_PER_INDIRECT_BLOCK;k++){
                        if (doublePointerBlock[k] == 0) continue;
                        else {
                            uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[k]);
                            for (int l=0;l<POINTER_PER_INDIRECT_BLOCK;l++){
                                if (singlePointerBlock[l] == 0) continue;
                                char* block = Block(singlePointerBlock[l]);
                                ext2_dir_entry* tempDirEntry;
                                for (int m=0;m<BLOCK_SIZE;m+=tempDirEntry->length){
                                    tempDirEntry = (ext2_dir_entry*) &block[m];
                                    if (!strncmp(current_token, tempDirEntry->name, tempDirEntry->name_length)){
                                        isCurrentTokenFound = true;
                                        inode_no = tempDirEntry->inode;
                                        localFlag = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }                
        if (!isCurrentTokenFound) {flag = false; break;}
        current_inode = getInodeByInodeNo(inode_no);
    }
    return flag;
}

ext2_dir_entry* prev_dir_entry;
ext2_dir_entry* file_dir_entry;
ext2_dir_entry* directory_dir_entry = NULL;
int dir_block_index;
vector<int> inodesOnThePath(vector<char*> tokens){
    vector<int> inodes;
    inodes.push_back(2);
    ext2_inode* current_inode = getInodeByInodeNo(2);
    for (int i=0;i<tokens.size();i++){
        int inode_no;
        char* current_token = tokens[i];
        bool localFlag = false;


        // Check Direct Blocks
        for (int j=0;j<12;j++){
            if (current_inode->direct_blocks[j] == 0) continue;
            else{
                char* block = Block(current_inode->direct_blocks[j]);
                ext2_dir_entry *tempDirEntry, *nextDirEntry;
                for (int k=0;k<BLOCK_SIZE;k+=tempDirEntry->length){
                    tempDirEntry = (ext2_dir_entry*) &block[k];
                    nextDirEntry = (ext2_dir_entry*) &block[k + tempDirEntry->length];
                    if (!strncmp(current_token, tempDirEntry->name, tempDirEntry->name_length)){
                        inode_no = tempDirEntry->inode;
                        localFlag = true;
                        if (((getInodeByInodeNo(tempDirEntry->inode)->mode) >> 15) & 0xFFFF){
                            file_dir_entry = tempDirEntry;
                            dir_block_index = current_inode->direct_blocks[j];
                        }
                        if (((getInodeByInodeNo(tempDirEntry->inode)->mode) >> 14) & 0xFFFF){
                            directory_dir_entry = tempDirEntry;
                            dir_block_index = current_inode->direct_blocks[j];
                        }
                    }
                    if (!strncmp(current_token, nextDirEntry->name, nextDirEntry->name_length)) prev_dir_entry = tempDirEntry;
                }
            }

        }

       // Check Single Indirect Blocks
        if ((current_inode->single_indirect != 0) && (localFlag == false)){
            uint32_t* singlePointerBlock = (uint32_t*) Block(current_inode->single_indirect);
            for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                if (singlePointerBlock[j] == 0) continue;
                else{
                    char* block = Block(singlePointerBlock[j]);
                    ext2_dir_entry* tempDirEntry, *nextDirEntry;
                    for (int k=0;k<BLOCK_SIZE;k+=tempDirEntry->length){
                        tempDirEntry = (ext2_dir_entry*) &block[k];
                        nextDirEntry = (ext2_dir_entry*) &block[k + tempDirEntry->length];
                        if (!strncmp(current_token, tempDirEntry->name, tempDirEntry->name_length)){
                            inode_no = tempDirEntry->inode;
                            localFlag = true;
                            if (((getInodeByInodeNo(tempDirEntry->inode)->mode) >> 15) & 0xFFFF){
                                file_dir_entry = tempDirEntry;
                                dir_block_index = singlePointerBlock[j];
                            }
                            if (((getInodeByInodeNo(tempDirEntry->inode)->mode) >> 14) & 0xFFFF){
                                directory_dir_entry = tempDirEntry;
                                dir_block_index = current_inode->direct_blocks[j];
                            }
                        }
                        if (!strncmp(current_token, nextDirEntry->name, nextDirEntry->name_length)) prev_dir_entry = tempDirEntry;
                    }
                }
            }
        }

        // Check Double Indirect Blocks
        if ((current_inode->double_indirect != 0) && (localFlag == false)){
            uint32_t* doublePointerBlock = (uint32_t*) Block(current_inode->double_indirect);
            for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                if (doublePointerBlock[j] == 0) continue;
                else {
                    uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[j]);
                    for (int k=0;k<POINTER_PER_INDIRECT_BLOCK;k++){
                        if (singlePointerBlock[k] == 0) continue;
                        char* block = Block(singlePointerBlock[k]);
                        ext2_dir_entry* tempDirEntry, *nextDirEntry;                    
                        for (int l=0;l<BLOCK_SIZE;l+=tempDirEntry->length){
                            tempDirEntry = (ext2_dir_entry*) &block[l];
                            nextDirEntry = (ext2_dir_entry*) &block[l + tempDirEntry->length];
                            if (!strncmp(current_token, tempDirEntry->name, tempDirEntry->name_length)){
                                inode_no = tempDirEntry->inode;
                                localFlag = true;
                                if (((getInodeByInodeNo(tempDirEntry->inode)->mode) >> 15) & 0xFFFF){
                                    file_dir_entry = tempDirEntry;
                                    dir_block_index = singlePointerBlock[k];
                                }    
                                if (((getInodeByInodeNo(tempDirEntry->inode)->mode) >> 14) & 0xFFFF){
                                    directory_dir_entry = tempDirEntry;
                                    dir_block_index = current_inode->direct_blocks[j];
                                }                                                            
                            }
                            if (!strncmp(current_token, nextDirEntry->name, nextDirEntry->name_length)) prev_dir_entry = tempDirEntry;
                        }
                    }
                }
            }
        }

        // Check Triple Indirect Blocks
        if ((current_inode->triple_indirect != 0) && (localFlag == false)){
            uint32_t* triplePointerBlock = (uint32_t*) Block(current_inode->triple_indirect);
            for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                if (triplePointerBlock[j] == 0) continue;
                else {
                    uint32_t* doublePointerBlock = (uint32_t*) Block(triplePointerBlock[j]);
                    for (int k=0;k<POINTER_PER_INDIRECT_BLOCK;k++){
                        if (doublePointerBlock[k] == 0) continue;
                        else {
                            uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[k]);
                            for (int l=0;l<POINTER_PER_INDIRECT_BLOCK;l++){
                                if (singlePointerBlock[l] == 0) continue;
                                char* block = Block(singlePointerBlock[l]);
                                ext2_dir_entry* tempDirEntry, *nextDirEntry;
                                for (int m=0;m<BLOCK_SIZE;m+=tempDirEntry->length){
                                    tempDirEntry = (ext2_dir_entry*) &block[m];
                                    nextDirEntry = (ext2_dir_entry*) &block[m + tempDirEntry->length];
                                    if (!strncmp(current_token, tempDirEntry->name, tempDirEntry->name_length)){
                                        inode_no = tempDirEntry->inode;
                                        localFlag = true;
                                        if (((getInodeByInodeNo(tempDirEntry->inode)->mode) >> 15) & 0xFFFF){
                                            file_dir_entry = tempDirEntry;
                                            dir_block_index = singlePointerBlock[l];
                                        } 
                                        if (((getInodeByInodeNo(tempDirEntry->inode)->mode) >> 14) & 0xFFFF){
                                            directory_dir_entry = tempDirEntry;
                                            dir_block_index = current_inode->direct_blocks[j];
                                        }                                             
                                    }
                                    if (!strncmp(current_token, nextDirEntry->name, nextDirEntry->name_length)) prev_dir_entry = tempDirEntry;
                                }
                            }
                        }
                    }
                }
            }
        }           


        inodes.push_back(inode_no);
        current_inode = getInodeByInodeNo(inode_no);
    }
    return inodes;    
}

void updateAccessTimes(vector<int> inodes){
    for (int i=0;i<inodes.size();i++){
        ext2_inode* inode = getInodeByInodeNo(inodes[i]);
        UPDATE_TIME(inode->access_time);
    }
}
void updateDeletionTime(int i){
    ext2_inode* inode = getInodeByInodeNo(i);
    UPDATE_TIME(inode->deletion_time);
}
void updateAccessTime(int i){
    ext2_inode* inode = getInodeByInodeNo(i);
    UPDATE_TIME(inode->access_time);
}
void updateCreationTime(int i){
    ext2_inode* inode = getInodeByInodeNo(i);
    UPDATE_TIME(inode->creation_time);
}
void updateModificationTime(int i){
    ext2_inode* inode = getInodeByInodeNo(i);
    UPDATE_TIME(inode->modification_time);   
}

bool is_block_used(uint32_t block_index) {
	uint32_t block_group_index = (uint32_t) ((block_index - super_block->first_data_block) / BLOCKS_PER_GROUP);
	uint32_t block_offset = (block_index - super_block->first_data_block) - block_group_index * BLOCKS_PER_GROUP;
	ext2_block_group_descriptor *group_desc = getBlockGroupDescriptorByGroupNo(block_group_index);
	char *block_bitmap = Block(group_desc->block_bitmap);
    return IS_BIT_SET(block_bitmap, block_offset);
}

void deallocate_block(uint32_t block_index) {
    if (!is_block_used(block_index)) return;
    cout << "EXT2_FREE_BLOCK " << block_index << endl;
	uint32_t block_group_index = (uint32_t) ((block_index - super_block->first_data_block) / BLOCKS_PER_GROUP);
	uint32_t block_offset = (block_index - super_block->first_data_block) - block_group_index * BLOCKS_PER_GROUP;
	ext2_block_group_descriptor *group_desc = getBlockGroupDescriptorByGroupNo(block_group_index);
	char *block_bitmap = Block(group_desc->block_bitmap);
	group_desc->free_block_count++;
    super_block->free_block_count++;
    BIT_CLEAR(block_bitmap, block_offset);
}

bool is_inode_used(uint32_t inode_index) {
	uint32_t block_group_index = ((inode_index - 1) / INODES_PER_GROUP);
	uint32_t inode_offset = inode_index - 1 - block_group_index * INODES_PER_GROUP;
	ext2_block_group_descriptor *group_desc = getBlockGroupDescriptorByGroupNo(block_group_index);
	char *inode_bitmap = Block(group_desc->inode_bitmap);
	return IS_BIT_SET(inode_bitmap, inode_offset);
}

void deallocate_inode(uint32_t inode_index) {
    if (!is_inode_used(inode_index)) return;
	uint32_t block_group_index = ((inode_index - 1) / INODES_PER_GROUP);
	uint32_t inode_offset = inode_index - 1 - block_group_index * INODES_PER_GROUP;
	ext2_block_group_descriptor *group_desc = getBlockGroupDescriptorByGroupNo(block_group_index);
	char *inode_bitmap = Block(group_desc->inode_bitmap);
    group_desc->free_inode_count++;
    super_block->free_inode_count++;
	BIT_CLEAR(inode_bitmap, inode_offset);

    if (operation == 1) group_desc->used_dirs_count--;
}

void allIndirectDirectBlocksDeallocate(ext2_inode* file_inode){
    // Direct Blocks
    for (int i=0;i<12;i++) {
        if (file_inode->direct_blocks[i] == 0) continue;
        else deallocate_block(file_inode->direct_blocks[i]);

    }
    // Single Indirect Blocks
    if (file_inode->single_indirect != 0){
        uint32_t* singlePointerBlock = (uint32_t*) Block(file_inode->single_indirect);
        for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
            if (singlePointerBlock[i] == 0) continue;
            else deallocate_block(singlePointerBlock[i]);
        }
        deallocate_block(file_inode->single_indirect);
    }
    // Double Indirect Blocks
    if (file_inode->double_indirect != 0){
        uint32_t* doublePointerBlock = (uint32_t*) Block(file_inode->double_indirect);
        for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
            if (doublePointerBlock[i] == 0) continue;
            else {
                uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[i]);
                for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                    if (singlePointerBlock[j] == 0) continue;
                    else deallocate_block(singlePointerBlock[j]);
                }
                deallocate_block(doublePointerBlock[i]);
            }
        }
        deallocate_block(file_inode->double_indirect);
    }
    // Triple Indirect Blocks
    if (file_inode->triple_indirect != 0){
        uint32_t* triplePointerBlock = (uint32_t*) Block(file_inode->triple_indirect);
        for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
            if (triplePointerBlock[i] == 0) continue;
            else {
                uint32_t* doublePointerBlock = (uint32_t*) Block(triplePointerBlock[i]);
                for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                    if (doublePointerBlock[j] == 0) continue;
                    uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[j]);
                    for (int k=0;k<POINTER_PER_INDIRECT_BLOCK;k++){
                        if (singlePointerBlock[k] == 0) continue;
                        else deallocate_block(singlePointerBlock[k]);
                    }
                    deallocate_block(doublePointerBlock[j]);                                     
                }
            }
            deallocate_block(triplePointerBlock[i]);
        }
        deallocate_block(file_inode->triple_indirect);
    }
}

int allocateInode(){

   for (int i=1; i<=INODE_COUNT; i++){
        if (is_inode_used(i)) continue;
        else {
            uint32_t block_group_index = ((i-1)/INODES_PER_GROUP);
            uint32_t inode_offset = i - 1 - block_group_index * INODES_PER_GROUP;
            ext2_block_group_descriptor* group_description = getBlockGroupDescriptorByGroupNo(block_group_index);
            char* inode_bitmap = Block(group_description->inode_bitmap);
            BIT_SET(inode_bitmap, inode_offset);
            super_block->free_inode_count--;
            group_description->free_inode_count--;
            
            if (operation == 2) group_description->used_dirs_count++;

            return i;
        }    
   } 
   return -1;

   // There is a field in the superblock first_inode. It may be needed to update this !!!!!
}

int allocateBlock(){
    int firstBlockNo = super_block->first_data_block;
    for (int i=firstBlockNo; i < firstBlockNo+BLOCK_COUNT; i++){
        if (is_block_used(i)) continue;
        else {
            uint32_t block_group_index = (uint32_t) ((i - firstBlockNo) / BLOCKS_PER_GROUP);
            uint32_t block_offset = (i - firstBlockNo) - block_group_index * BLOCKS_PER_GROUP;
            ext2_block_group_descriptor* group_descriptor = getBlockGroupDescriptorByGroupNo(block_group_index);
            char* block_bitmap = Block(group_descriptor->block_bitmap);
            group_descriptor->free_block_count--;
            super_block->free_block_count--;
            BIT_SET(block_bitmap, block_offset);
            return i;
        }
    }
    return -1;
}

int clearPointerBlocks(int inode_no){
    int N = 0;
    ext2_inode* inode = getInodeByInodeNo(inode_no);
    bool flag = false;
    bool flag2 = false;
    if (inode->single_indirect != 0){
        flag2 = true;
        uint32_t* singlePointerBlock = (uint32_t*) Block(inode->single_indirect);
        for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
            if (singlePointerBlock[i] == 0) continue;
            else flag = true; 
        }
    }
    if (flag == false && flag2 == true){
        N++;
        deallocate_block(inode->single_indirect);
        inode->single_indirect = 0;
    }
    flag = false;
    flag2 = false;

    if (inode->double_indirect != 0){
        flag2 = true;
        uint32_t* doublePointerBlock = (uint32_t*) Block(inode->double_indirect);
        for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
            if (doublePointerBlock[i] == 0) continue;
            else {
                uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[i]);
                for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                    if (singlePointerBlock[j] == 0) continue;
                    else flag = true;
                }
            }
        }
    }
    if (flag == false && flag2 == true){
        N++;
        deallocate_block(inode->double_indirect);
        inode->double_indirect = 0;
    }
    flag = false;    
    flag2 = false;
    
    if (inode->triple_indirect != 0){
        flag2 = true;
        uint32_t* triplePointerBlock = (uint32_t*) Block(inode->triple_indirect);
        for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
            if (triplePointerBlock[i] == 0) continue;
            else {
                uint32_t* doublePointerBlock = (uint32_t*) Block(triplePointerBlock[i]);
                for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                    if (doublePointerBlock[j] == 0) continue;
                    uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[j]);
                    for (int k=0;k<POINTER_PER_INDIRECT_BLOCK;k++){
                        if (singlePointerBlock[k] == 0) continue;
                        else flag = true;
                    }
                }
            }
        }
    }
    if (flag == false && flag2 == true){
        N++;
        deallocate_block(inode->triple_indirect);
        inode->triple_indirect = 0;
    }
    flag = false;  
    flag2 == false;

    return N;
}

void rm(vector<char*> tokens){
    if (isPathExist(tokens) == false) return;
    vector<int> inodes= inodesOnThePath(tokens);
    int fileInodeNo = inodes[inodes.size() - 1];
    int parentInodeNo = inodes[inodes.size() -2];

    ext2_inode* file_inode = getInodeByInodeNo(fileInodeNo);
    ext2_inode* parent_inode = getInodeByInodeNo(parentInodeNo);    

    // remove a dir entry with fname under parent_inode
    char* file_name = new char[file_dir_entry->name_length + 1];
    for (int i=0;i<file_dir_entry->name_length;i++) file_name[i] = file_dir_entry->name[i];
    file_name[file_dir_entry->name_length] = '\0';
    cout << "EXT2_FREE_ENTRY " << "\""<< file_name << "\"" << " " << dir_block_index << endl;

    // prev_dir_entry->length = 0x1234; // Removal of linked list node


    prev_dir_entry->length += file_dir_entry->length;

    // for each block in file_inode's direact and indirect blocks, deallocate the block
    allIndirectDirectBlocksDeallocate(file_inode);
    
    
    file_inode->link_count--;
    if (file_inode->link_count == 0){
        cout << "EXT2_FREE_INODE " << fileInodeNo << endl;
        deallocate_inode(fileInodeNo);
    }

    // Update write time in super_block
    UPDATE_TIME(super_block->write_time);

    updateCreationTime(inodes[inodes.size() - 2]);
    updateModificationTime(inodes[inodes.size() - 2]);


    // Deletion time updated
    updateDeletionTime(inodes[inodes.size() - 1]);
    // Access Creation updated
    updateCreationTime(inodes[inodes.size() - 1]);
}

void rmdir(vector<char*> tokens){
    if (isPathExist(tokens) == false) return;
    vector<int> inodes = inodesOnThePath(tokens);

    int parent_inode_no = inodes[inodes.size() - 2];
    int inode_no = inodes[inodes.size() - 1];

    // if /abs/path/to/dir has more than tow dir entry then return !!!!!!!!!


    ext2_inode* parent_inode = getInodeByInodeNo(parent_inode_no);
    ext2_inode* inode = getInodeByInodeNo(inode_no);
    
    cout << "EXT2_FREE_ENTRY " << "\""<< directory_dir_entry->name << "\"" << " " << dir_block_index << endl;
    prev_dir_entry->length += directory_dir_entry->length;


    allIndirectDirectBlocksDeallocate(inode);
    deallocate_inode(inode_no);
    cout << "EXT2_FREE_INODE " << inode_no << endl;

    inode->link_count-=2;

    parent_inode->link_count--;

    // Update write time in super_block
    UPDATE_TIME(super_block->write_time);

    // Deletion time updated
    updateDeletionTime(inode_no);

}

void mkdir(vector<char*> tokens){
    char* new_dir_name = tokens[tokens.size() - 1];
    vector<char*> tokensUpToParent; 
    for (int i=0;i<tokens.size()-1;i++) tokensUpToParent.push_back(tokens[i]);

    if (isPathExist(tokensUpToParent) == false) return ;
    if (isPathExist(tokens) == true) return ;

    int allocatedInodeNo = allocateInode();
    cout << "EXT2_ALLOC_INODE " << allocatedInodeNo << endl;

    int allocatedBlockNo = allocateBlock();
    cout << "EXT2_ALLOC_BLOCK " << allocatedBlockNo << endl;

    vector<int> inodes = inodesOnThePath(tokensUpToParent);
    int parentInodeNo = inodes[inodes.size()-1];

    
    ext2_inode* parent_inode = getInodeByInodeNo(parentInodeNo);

    parent_inode->link_count++;
    UPDATE_TIME(parent_inode->creation_time);
    UPDATE_TIME(parent_inode->modification_time);

    


    ext2_inode* new_inode = getInodeByInodeNo(allocatedInodeNo);
    new_inode->mode = 0x41fd; // I don't know why this works, ask to hoca
    new_inode->uid = EXT2_I_UID;
    new_inode->size = 0x1000; // I don't know why this works, ask to hoca
    UPDATE_TIME(new_inode->access_time);
    UPDATE_TIME(new_inode->creation_time);
    UPDATE_TIME(new_inode->modification_time);


    new_inode->deletion_time = 0;
    new_inode->gid = EXT2_I_GID;
    new_inode->link_count = 2; // Possibly because of . and ..
    new_inode->block_count_512 = BLOCK_SIZE/512; // Possibly because of this but ask it to hoca
    new_inode->flags = parent_inode->flags;
    new_inode->reserved = parent_inode->reserved;
    new_inode->direct_blocks[0] = allocatedBlockNo;
    new_inode->single_indirect = 0;
    new_inode->double_indirect = 0;
    new_inode->triple_indirect = 0;


    for (int i=0; i<12; i++){
        if (parent_inode->direct_blocks[i] == 0) continue;
        else {
            cout << "EXT2_ALLOC_ENTRY " << "\"" << new_dir_name << "\" " << parent_inode->direct_blocks[i] << endl;
            char* block = Block(parent_inode->direct_blocks[i]);
            ext2_dir_entry* tempDirEntry;
            for (int k=0;k<BLOCK_SIZE;k+=EXT2_DIR_LENGTH(tempDirEntry->name_length)){
                tempDirEntry = (ext2_dir_entry*) &block[k];

                // Adjust length of previous dir entry
                if (EXT2_DIR_LENGTH(tempDirEntry->name_length) < tempDirEntry->length) tempDirEntry->length = EXT2_DIR_LENGTH(tempDirEntry->name_length);

                if (tempDirEntry->inode == 0){
                    tempDirEntry->inode = allocatedInodeNo;
                    tempDirEntry->length = BLOCK_SIZE - k;  // for consistency
                    tempDirEntry->name_length = strlen(new_dir_name);
                    tempDirEntry->file_type = EXT2_D_DTYPE;
                    for (int i=0;i<strlen(new_dir_name); i++) tempDirEntry->name[i] = new_dir_name[i];
                    tempDirEntry->name[strlen(new_dir_name)] = '\0';                    
                    break;
                }
            }
            break;
        }
    }

    char* block = Block(allocatedBlockNo);
    ext2_dir_entry* tempDirEntry;
    for (int i=0; i<BLOCK_SIZE; i+=tempDirEntry->length){
        tempDirEntry = (ext2_dir_entry*) &block[i];
        if (tempDirEntry->inode != 0) continue;
        else {
            tempDirEntry->inode = allocatedInodeNo;
            tempDirEntry->length = 12;
            tempDirEntry->name_length = 1;
            tempDirEntry->file_type = 2;
            tempDirEntry->name[0] = '.';
            tempDirEntry->name[1] = '\0';
            break;
        }
    }
    cout << "EXT2_ALLOC_ENTRY " << "\".\" " << allocatedBlockNo << endl;

    for (int i=0; i<BLOCK_SIZE; i+=tempDirEntry->length){
        tempDirEntry = (ext2_dir_entry*) &block[i];
        if (tempDirEntry->inode != 0) continue;
        else {
            tempDirEntry->inode = parentInodeNo;
            tempDirEntry->length = 0xff4; // I don't know why this works, ask to hoca
            tempDirEntry->name_length = 2;
            tempDirEntry->file_type = 2;
            tempDirEntry->name[0] = '.';
            tempDirEntry->name[1] = '.';
            tempDirEntry->name[2] = '\0';
            break;
        }
    }    

    cout << "EXT2_ALLOC_ENTRY " << "\"..\" " << allocatedBlockNo << endl;

    UPDATE_TIME(super_block->write_time);
    updateCreationTime(allocatedInodeNo);
    updateModificationTime(allocatedInodeNo);

}

void printFileContent(vector<int> dataBlockNoS){
    // This is not related to core implementation. It is just for debugging purposes.
    for (int i=0;i<dataBlockNoS.size();i++){
        char* block = Block(dataBlockNoS[i]);
        for (int j=0;j<BLOCK_SIZE;j++) if (block[j] != '\0') cout << block[j];
    }
}

void printFileContentWithNulls(vector<int> dataBlockNoS){
    // This is not related to core implementation. It is just for debugging purposes.
    for (int i=0;i<dataBlockNoS.size();i++){
        char* block = Block(dataBlockNoS[i]);
        for (int j=0;j<BLOCK_SIZE;j++) if (1) cout << block[j];
    }
}


bool isIn(int x, vector<int> v){
    bool flag = false;
    for (int i=0;i<v.size();i++){
        if (v[i] == x){
            flag = true;
            break;
        }
    }
    return flag;
}
void clearInode(ext2_inode*& inode, vector<int> BLOCKS_TO_BE_FREED){
    int N = BLOCKS_TO_BE_FREED.size();
    int C = 0;

    for (int i=0;i<12;i++){
        if (inode->direct_blocks[i] == 0) continue;
        if (isIn(inode->direct_blocks[i], BLOCKS_TO_BE_FREED)){
            inode->direct_blocks[i] = 0;
            C++;
            if (C == N) return ;
        }
    }

    if (inode->single_indirect != 0){
        uint32_t* singlePointerBlock = (uint32_t*) Block(inode->single_indirect);
        for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
            if (singlePointerBlock[i] == 0) continue;
            if (isIn(singlePointerBlock[i], BLOCKS_TO_BE_FREED)){
                singlePointerBlock[i] = 0;
                C++;
                if (C == N) return; 
            }
        }
    }

    if (inode->double_indirect != 0){
        uint32_t* doublePointerBlock = (uint32_t*) Block(inode->double_indirect);
        for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
            if (doublePointerBlock[i] == 0) continue;
            else {
                uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[i]);
                for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                    if (singlePointerBlock[j] == 0) continue;
                    if (isIn(singlePointerBlock[j], BLOCKS_TO_BE_FREED)){
                        singlePointerBlock[j] = 0;
                        C++;
                        if (C == N) return; 
                    }
                }
            }
        }
    }

    if (inode->triple_indirect != 0){
        uint32_t* triplePointerBlock = (uint32_t*) Block(inode->triple_indirect);
        for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
            if (triplePointerBlock[i] == 0) continue;
            else {
                uint32_t* doublePointerBlock = (uint32_t*) Block(triplePointerBlock[i]);
                for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                    if (doublePointerBlock[j] == 0) continue;
                    uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[j]);
                    for (int k=0;k<POINTER_PER_INDIRECT_BLOCK;k++){
                        if (singlePointerBlock[k] == 0) continue;
                        if (isIn(singlePointerBlock[k], BLOCKS_TO_BE_FREED)){
                            singlePointerBlock[k] = 0;
                            C++;
                            if (C == N) return; 
                        }
                    }
                }
            }
        }
    }    
}

void printbufferqueue(queue<char> queue){
    while (!queue.empty()){
        char c = queue.front();
        queue.pop();
        cout << c;
    }
}

ext2_block_group_descriptor* getGroupDescOfBlock(int block_no){
	uint32_t block_group_index = (uint32_t) ((block_no - super_block->first_data_block) / BLOCKS_PER_GROUP);
    return getBlockGroupDescriptorByGroupNo(block_group_index);
}

void ed(vector<char*> tokens, int INDEX, int BACKSPACE, const char* STRING){
    if (isPathExist(tokens) == false) return;
    vector<int> inodes = inodesOnThePath(tokens);

    int parent_inode_no = inodes[inodes.size() - 2];
    int inode_no = inodes[inodes.size() - 1];

    ext2_inode* parent_inode = getInodeByInodeNo(parent_inode_no);
    ext2_inode* inode = getInodeByInodeNo(inode_no);

    vector<int> dataBlockNoS;
    for (int i=0; i<12; i++){
        if (inode->direct_blocks[i] == 0) continue;
        else dataBlockNoS.push_back(inode->direct_blocks[i]);
    }
    if (inode->single_indirect != 0){
        uint32_t* singlePointerBlock = (uint32_t*) Block(inode->single_indirect);
        for (int i=0; i<POINTER_PER_INDIRECT_BLOCK; i++){
            if (singlePointerBlock[i] == 0) continue;
            else dataBlockNoS.push_back(singlePointerBlock[i]);
        }
    }
    if (inode->double_indirect != 0){
        uint32_t* doublePointerBlock = (uint32_t*) Block(inode->double_indirect);
        for (int i=0; i<POINTER_PER_INDIRECT_BLOCK; i++){
            if (doublePointerBlock[i] == 0) continue;
            else {
                uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[i]);
                for (int j=0; j<POINTER_PER_INDIRECT_BLOCK; j++){
                    if (singlePointerBlock[j] == 0) continue;
                    else dataBlockNoS.push_back(singlePointerBlock[j]);
                }

            }
        }
    }
    if (inode->triple_indirect != 0){
        uint32_t* triplePointerBlock  = (uint32_t*) Block(inode->triple_indirect);
        for (int i=0; i<POINTER_PER_INDIRECT_BLOCK; i++){
            if (triplePointerBlock[i] == 0) continue;
            else {
                uint32_t* doublePointerBlock = (uint32_t*) Block(triplePointerBlock[i]);
                for (int j=0; j<POINTER_PER_INDIRECT_BLOCK; j++){
                    if (doublePointerBlock[j] == 0) continue;
                    uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[j]);
                    for (int k=0; k<POINTER_PER_INDIRECT_BLOCK;k++){
                        if (singlePointerBlock[k] == 0) continue;
                        else dataBlockNoS.push_back(singlePointerBlock[k]);
                    }
                }            
            }
        }
    }

    /*int CURSOR1 = 0;
    queue<char> content_buffer;
    for (int i=0;i<dataBlockNoS.size();i++){
        char* dataBlock = Block(dataBlockNoS[i]);
        if (CURSOR1 >= (INDEX - BACKSPACE)) break;
        for (int j=0; j<BLOCK_SIZE; j++){
            if (CURSOR1 >= (INDEX - BACKSPACE)) break;
            if (dataBlock[j] != '0') content_buffer.push(dataBlock[j]);
            CURSOR1++;
        }
    }

    
    for (int i=0;STRING[i] != '\0' ;i++) content_buffer.push(STRING[i]);



    int CURSOR2 = 0;
    int BACKSPACE_COUNT = BACKSPACE;
    for (int i=0;i<dataBlockNoS.size();i++){
        char* dataBlock = Block(dataBlockNoS[i]);
        for (int j=0; j<BLOCK_SIZE; j++){
            if (CURSOR2 >= CURSOR1){
                if (BACKSPACE_COUNT > 0) BACKSPACE_COUNT--;
                else {
                    if (dataBlock[j] != '\0') content_buffer.push(dataBlock[j]);
                }
            }
            CURSOR2++;
        }
    }*/


    queue<char> content_buffer;
    int CURSOR = 0;
    bool flag = false;

    for (int i=0;i<dataBlockNoS.size();i++){
        char* dataBlock = Block(dataBlockNoS[i]);
        for (int j=0; j<BLOCK_SIZE; j++){
            if (CURSOR < (INDEX-BACKSPACE)){
                if (dataBlock[j] != '\0') content_buffer.push(dataBlock[j]);
                CURSOR++;
            }
            else {
                flag = true;
                break;
            }
        }
        if (flag) break;
    }
    
    for (int i=0;STRING[i] != '\0' ;i++) content_buffer.push(STRING[i]);

    CURSOR = 0;
    for (int i=0;i<dataBlockNoS.size();i++){
        char* dataBlock = Block(dataBlockNoS[i]);
        for (int j=0; j<BLOCK_SIZE; j++){
            if (CURSOR >= INDEX){
                if (dataBlock[j] != '\0'){
                    content_buffer.push(dataBlock[j]);
                }
                CURSOR++;
            }
            else CURSOR++;
        }
    }

    // Now content_buffer includes updated file content as a whole.
    float CHARACTER_COUNT = content_buffer.size();
    float BLOCK_SIZE_F = BLOCK_SIZE;
    int REQUIRED_BLOCK_COUNT = ceil(CHARACTER_COUNT/BLOCK_SIZE_F);

    if (REQUIRED_BLOCK_COUNT == dataBlockNoS.size()){
        // No further alloc/dealloc needed
        for (int i=0; i<dataBlockNoS.size();i++){
            char* block = Block(dataBlockNoS[i]);
            if (content_buffer.empty()) break;
            for (int j=0;j<BLOCK_SIZE;j++){
                if (content_buffer.empty()) break;
                char c = content_buffer.front();
                content_buffer.pop();
                block[j] = c;
            }
        }
    }
    else if (REQUIRED_BLOCK_COUNT < dataBlockNoS.size()){        
        // Some Blocks Will be Deallocated        
        int BLOCK_COUNT_TO_BE_FREED = dataBlockNoS.size() - REQUIRED_BLOCK_COUNT;
        vector<int> BLOCK_NOS_TO_BE_FREED;
        for (int i=0;i<BLOCK_COUNT_TO_BE_FREED;i++) BLOCK_NOS_TO_BE_FREED.push_back(dataBlockNoS[dataBlockNoS.size() - i -1]);
        reverse(BLOCK_NOS_TO_BE_FREED.begin(), BLOCK_NOS_TO_BE_FREED.end());

        for (int i=0;i<BLOCK_NOS_TO_BE_FREED.size();i++) dataBlockNoS.pop_back();
        for (int i=0;i<BLOCK_NOS_TO_BE_FREED.size();i++) deallocate_block(BLOCK_NOS_TO_BE_FREED[i]);
        clearInode(inode, BLOCK_NOS_TO_BE_FREED);
        int CLEARED_POINTER_BLOCK_COUNT = clearPointerBlocks(inode_no);
        for (int i=0; i<dataBlockNoS.size();i++){
            char* block = Block(dataBlockNoS[i]);
            if (content_buffer.empty()) break;
            for (int j=0;j<BLOCK_SIZE;j++){
                if (content_buffer.empty()){
                    block[j] = '\0';
                    continue;
                }
                char c = content_buffer.front();
                content_buffer.pop();
                block[j] = c;
            }
        }
        inode->block_count_512 -= ((BLOCK_SIZE/512)*(BLOCK_COUNT_TO_BE_FREED + CLEARED_POINTER_BLOCK_COUNT));
    }
    else if (REQUIRED_BLOCK_COUNT > dataBlockNoS.size()){
        // Some Blocks Will be Allocated
        int EXTRA_BLOCK_COUNT = REQUIRED_BLOCK_COUNT - dataBlockNoS.size();
        vector<int> extraBlockNos;

        for (int i=0;i<EXTRA_BLOCK_COUNT;i++){
            int new_block_no = allocateBlock();
            cout << "EXT2_ALLOC_BLOCK " << new_block_no << endl;
            extraBlockNos.push_back(new_block_no);
        }
        for (int i=0;i<EXTRA_BLOCK_COUNT;i++) dataBlockNoS.push_back(extraBlockNos[i]);


        for (int i=0;i<EXTRA_BLOCK_COUNT;i++){
            int CURRENT_BLOCK_NO = extraBlockNos[i];
            bool flag = false;

            // Direct Blocks
            for (int i=0;i<12;i++){
                if (inode->direct_blocks[i] != 0) continue;
                else{
                    inode->direct_blocks[i] = CURRENT_BLOCK_NO;
                    flag = true;
                    break;
                }
            }
            if (flag) continue;

            // Single Indirect
            if (inode->single_indirect != 0){
                uint32_t* singlePointerBlock = (uint32_t*) Block(inode->single_indirect);
                for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
                    if (singlePointerBlock[i] != 0) continue;
                    else{
                        singlePointerBlock[i] = CURRENT_BLOCK_NO;
                        flag = true;
                        break;;
                    }
                }
            }
            if (flag) continue;


            // Double Indirect
            bool flag2 = false;
            if (inode->double_indirect != 0){
                uint32_t* doublePointerBlock = (uint32_t*) Block(inode->double_indirect);
                for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
                    if (doublePointerBlock[i] == 0) continue;
                    else {
                        uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[i]);
                        for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                            if (singlePointerBlock[j] != 0) continue;
                            else {
                                singlePointerBlock[j] = CURRENT_BLOCK_NO;
                                flag = true;
                                flag2 = true;
                                break;
                            }
                        }
                    }
                    if (flag2) break;
                }
            }
            if (flag) continue;

            // Triple Indirect
            bool flag3 = false;
            bool flag4 = false;
            if (inode->triple_indirect != 0){
                uint32_t* triplePointerBlock = (uint32_t*) Block(inode->triple_indirect);
                for (int i=0;i<POINTER_PER_INDIRECT_BLOCK;i++){
                    if (triplePointerBlock[i] == 0) continue;
                    else {
                        uint32_t* doublePointerBlock = (uint32_t*) Block(triplePointerBlock[i]);
                        for (int j=0;j<POINTER_PER_INDIRECT_BLOCK;j++){
                            if (doublePointerBlock[j] == 0) continue;
                            uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[j]);
                            for (int k=0;k<POINTER_PER_INDIRECT_BLOCK;k++){
                                if (singlePointerBlock[k] == 0) continue;
                                else {
                                    singlePointerBlock[k] = CURRENT_BLOCK_NO;
                                    flag = true;
                                    flag3 = true;
                                    flag4 = true;
                                    break;
                                }
                            }
                        }
                        if (flag3) break;
                    }
                }
                if (flag4) break;
            }
            if (flag) continue;

        }

        for (int i=0; i<dataBlockNoS.size();i++){
            char* block = Block(dataBlockNoS[i]);
            if (content_buffer.empty()) break;
            for (int j=0;j<BLOCK_SIZE;j++){
                if (content_buffer.empty()) break;
                char c = content_buffer.front();
                content_buffer.pop();
                block[j] = c;
            }
        }

        inode->block_count_512 += ((BLOCK_SIZE/512)*EXTRA_BLOCK_COUNT);
    }

    inode->size += (strlen(STRING) - BACKSPACE);

    UPDATE_TIME(super_block->write_time);
    UPDATE_TIME(inode->modification_time);
    UPDATE_TIME(inode->access_time);
    return ;
}

void cat(vector<char*> tokens){
    // For debugging
    if (isPathExist(tokens) == false) return;
    vector<int> inodes = inodesOnThePath(tokens);

    int parent_inode_no = inodes[inodes.size() - 2];
    int inode_no = inodes[inodes.size() - 1];

    ext2_inode* parent_inode = getInodeByInodeNo(parent_inode_no);
    ext2_inode* inode = getInodeByInodeNo(inode_no);

    vector<int> dataBlockNoS;
    for (int i=0; i<12; i++){
        if (inode->direct_blocks[i] == 0) continue;
        else dataBlockNoS.push_back(inode->direct_blocks[i]);
    }
    if (inode->single_indirect != 0){
        uint32_t* singlePointerBlock = (uint32_t*) Block(inode->single_indirect);
        for (int i=0; i<POINTER_PER_INDIRECT_BLOCK; i++){
            if (singlePointerBlock[i] == 0) continue;
            else dataBlockNoS.push_back(singlePointerBlock[i]);
        }
    }
    if (inode->double_indirect != 0){
        uint32_t* doublePointerBlock = (uint32_t*) Block(inode->double_indirect);
        for (int i=0; i<POINTER_PER_INDIRECT_BLOCK; i++){
            if (doublePointerBlock[i] == 0) continue;
            else {
                uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[i]);
                for (int j=0; j<POINTER_PER_INDIRECT_BLOCK; j++){
                    if (singlePointerBlock[j] == 0) continue;
                    else dataBlockNoS.push_back(singlePointerBlock[j]);
                }

            }
        }
    }
    if (inode->triple_indirect != 0){
        uint32_t* triplePointerBlock  = (uint32_t*) Block(inode->triple_indirect);
        for (int i=0; i<POINTER_PER_INDIRECT_BLOCK; i++){
            if (triplePointerBlock[i] == 0) continue;
            else {
                uint32_t* doublePointerBlock = (uint32_t*) Block(triplePointerBlock[i]);
                for (int j=0; j<POINTER_PER_INDIRECT_BLOCK; j++){
                    if (doublePointerBlock[j] == 0) continue;
                    uint32_t* singlePointerBlock = (uint32_t*) Block(doublePointerBlock[j]);
                    for (int k=0; k<POINTER_PER_INDIRECT_BLOCK;k++){
                        if (singlePointerBlock[k] == 0) continue;
                        else dataBlockNoS.push_back(singlePointerBlock[k]);
                    }
                }            
            }
        }
    }

    printFileContent(dataBlockNoS);
}

int main(int argc, char* argv[]){
    if (argc == 4){
        const char* FS_IMAGE_PATH = argv[1];
        const char* directory_path = argv[3];
        int FS_IMAGE_FILE_DESCRIPTOR = open(FS_IMAGE_PATH, O_RDWR);
        struct stat file_stat;
        fstat(FS_IMAGE_FILE_DESCRIPTOR, &file_stat);
        DISK = (char*) mmap(0, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, FS_IMAGE_FILE_DESCRIPTOR, 0);
        vector<char*> tokens = tokenizePath(argv[3]);
        readSuperBlock();

        if (!strcmp("rm", argv[2])){
            operation = 0;
            rm(tokens);
        }
        else if (!strcmp("rmdir", argv[2])){
            operation = 1;
            rmdir(tokens);
        }
        else if (!strcmp("mkdir", argv[2])){
            operation = 2;
            mkdir(tokens);
        }
        else if (!strcmp("cat", argv[2])) cat(tokens);
        return 0;
    }
    else if (argc == 10){
        const char* FS_IMAGE_PATH = argv[1];
        const char* directory_path = argv[7];
        const char* index_as_string = argv[4];
        const char* backspace_as_string = argv[6];
        char* STRING = argv[9];
        const int INDEX = atoi(index_as_string);
        const int BACKSPACE = atoi(backspace_as_string);

        /*char* QUOTED_STRING = new char[strlen(STRING) + 1 + 2];
        QUOTED_STRING[0] = '\"';
        QUOTED_STRING[strlen(STRING) + 2] = '\0';
        QUOTED_STRING[strlen(STRING) + 1] = '\"';
        for (int i=1;i<=strlen(STRING);i++) QUOTED_STRING[i] = STRING[i-1];*/

        int FS_IMAGE_FILE_DESCRIPTOR = open(FS_IMAGE_PATH, O_RDWR);
        struct stat file_stat;
        fstat(FS_IMAGE_FILE_DESCRIPTOR, &file_stat);
        DISK = (char*) mmap(0, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, FS_IMAGE_FILE_DESCRIPTOR, 0);
        vector<char*> tokens = tokenizePath(argv[7]);
        readSuperBlock();        

        // if (!strcmp("ed", argv[2])) ed(tokens, INDEX, BACKSPACE, QUOTED_STRING);
        if (!strcmp("ed", argv[2])) ed(tokens, INDEX, BACKSPACE, STRING);


        return 0;
    }

    return 0;
}