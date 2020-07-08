#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <iostream>
#include "leveldb/env.h"
#include "leveldb/table_builder.h"


int main(int argc, char *argv[]){

    using namespace std;
    leveldb::Options options;
   // options.block_restart_interval = 16;
   
    //std::cout<<argv[0];
   // std::cout<<argv[1];

    std::string file_name(argv[0]);
    std::cout<<file_name<<std::endl;
    
    
    leveldb::WritableFile* file;
    leveldb::Status s = leveldb::Env::Default()->NewWritableFile(
            file_name,
            &file);
    leveldb::TableBuilder table_builder(options, file);


    ifstream fin("/home/jacky/leveldb_encryption/test/buffer.txt");  
    std::string kstr;
    std::string str;
    int count = 1; 
    while( getline(fin,str) )
    {
    	if((count % 2) == 1){
	    //cout << "key: " << kstr << endl;
	    kstr = str;
        }
    	    
    	if((count % 2) == 0){
	    //cout << "encryption key: " << str << endl;
	    table_builder.Add(kstr,str);
        }
        count++;
    }

    
    //const char *cstr = file_name.c_str();
    

 
    //table_builder.Add("confuse", "value");
    leveldb::Status status = table_builder.Finish();
    file->Close();

    remove( "/home/jacky/leveldb_encryption/test/buffer.txt") ;
 return 0;

}
