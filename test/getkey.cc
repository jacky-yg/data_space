#include <unistd.h>
#include <iostream>
#include <assert.h>
#include "leveldb/env.h"
#include "leveldb/table_builder.h"
#include "leveldb/db.h"
#include "leveldb/dumpfile.h"
#include "leveldb/table.h"
#include <sys/types.h>
#include <string>
#include <sys/stat.h>
#include "aes_gcm.h"

#define TXT_SIZE  8
#define AAD_SIZE 32
#define TAG_SIZE 16		/* Valid values are 16, 12, or 8 */
#define KEY_SIZE GCM_256_KEY_LEN
#define IV_SIZE  GCM_IV_DATA_LEN


void decryption(std::string vkey,char* value, const char* ekey, int size);

int main(int argc, char* argv[]) {

leveldb::Table* table = nullptr;
    //std::string file_path = "./data/test_table.db/000005.ldb";
    std::string file_num(argv[0]);
    std::string file_name = std::string(6 - file_num.length(), '0') + file_num + ".ldb";

    std::string file_path = "/home/jacky/leveldb_encryption/key/"+file_name;
    std::cout<<file_path<<std::endl;
    
    //New RandomAccessFile
    leveldb::RandomAccessFile* file = nullptr;
    leveldb::Status status = leveldb::Env::Default()->NewRandomAccessFile(
            file_path,
            &file);
    std::cout << "NewRandomAccessFile status:" << status.ToString() << std::endl;

    //New Table
    struct stat file_stat;
    stat(file_path.c_str(), &file_stat);
    status = leveldb::Table::Open(
            leveldb::Options(),
            file,
            file_stat.st_size,
            &table);

    std::cout << "leveldb::Table::Open status:" << status.ToString() << std::endl;
   //  std::cout<<"before seek"<<std::endl;
    leveldb::Iterator* iter = table->NewIterator(leveldb::ReadOptions());
    
   iter->SeekToFirst();
   if(iter->Valid())
	std::cout<<"valid"<<std::endl;
   std::string key(argv[1]);
   //std::string value(argv[2]);

   //int length = strlen(argv[2]);
   //std::cout<<length<<std::endl;


   iter->Seek(key);
   std::cout << iter->key().ToString() << "->" << iter->value().ToString() << std::endl;
    std::cout<<argv[3]<<std::endl;
   decryption(key,argv[2],iter->value().data(),240); // key,value,encryption key
  


   
   

    /*while (iter->Valid()) {
        std::cout << iter->key().ToString() << "->" << iter->value().ToString() << std::endl;
        iter->Next();
    }*/

    delete iter;
    delete file;
    delete table;
}


void decryption(std::string vkey, char* value,const char* ekey, int size){
    struct gcm_key_data gkey;
    struct gcm_context_data gctx;
    
    //int vlength = strlen(value);
    //std::cout<<vlength<<std::endl;

    uint8_t ct[size], pt[size];	// Cipher text and plain text

    uint8_t iv[IV_SIZE], aad[AAD_SIZE], key[KEY_SIZE];	// Key and authentication data
    uint8_t tag2[TAG_SIZE];	// Authentication tags for encode and decode
    memcpy(key,ekey,KEY_SIZE);
    memset(iv, 0, IV_SIZE);
    memset(aad, 0, AAD_SIZE);

    memcpy(ct,value,size);
    //contents.remove_prefix(data_size);

    //size_t re_size = n - data_size;
    //2019
    //std::cout<<"re_size:"<<re_size<<std::endl;


    aes_gcm_pre_256(key, &gkey);
    aes_gcm_dec_256(&gkey, &gctx, pt, ct, size , iv, aad, AAD_SIZE, tag2, TAG_SIZE);

    std::cout<<"key="<<key<<",value=";
    for(int i=0; i<size; i++)
	std::cout<<pt[i];
    std::cout<<"\n";
}
    



/*void readblock() {
    leveldb::Table* table = nullptr;
    //std::string file_path = "./data/test_table.db/000005.ldb";
    std::string file_path = "./table_builder.data";

    //New RandomAccessFile
    leveldb::RandomAccessFile* file = nullptr;
    leveldb::Status status = leveldb::Env::Default()->NewRandomAccessFile(
            file_path,
            &file);
    std::cout << "NewRandomAccessFile status:" << status.ToString() << std::endl;

    //New Table
    struct stat file_stat;
    stat(file_path.c_str(), &file_stat);
    status = leveldb::Table::Open(
            leveldb::Options(),
            file,
            file_stat.st_size,
            &table);
    std::cout << "leveldb::Table::Open status:" << status.ToString() << std::endl;
    status = leveldb::Table::InternalReader()

    leveldb::Iterator* iter = table->NewIterator(leveldb::ReadOptions());
    
    iter->Seek("key1");
    std::cout << iter->key().ToString() << "->" << iter->value().ToString() << std::endl;

    table::InternalGet

    while (iter->Valid()) {
        std::cout << iter->key().ToString() << "->" << iter->value().ToString() << std::endl;
        iter->Next();
    }

    delete iter;
    delete file;
    delete table;
}*/


