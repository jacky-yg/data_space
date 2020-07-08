#include <unistd.h>
#include <iostream>
#include "leveldb/env.h"
#include "leveldb/table_builder.h"

int main() {
    leveldb::Options options;
    options.block_restart_interval = 4;

    std::string file_name("/home/jacky/leveldb_encryption/key_space/table_builder.data");

    leveldb::WritableFile* file;
    leveldb::Status s = leveldb::Env::Default()->NewWritableFile(
            file_name,
            &file);

    leveldb::TableBuilder table_builder(options, file);
    table_builder.Add("confuse", "value");
    leveldb::Status status = table_builder.Finish();
    return 0;
}
