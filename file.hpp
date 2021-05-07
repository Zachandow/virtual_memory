
#pragma once
#include <string>

#define READ_BUF_SIZ 256

class File{
private:
    FILE* obj;
public:
    File() = default;

    File(const char* filenum) {
        this->obj = fopen(filenum, "r");
    }

    ~File(){
        fclose(this->obj);
    }

    char operator[](int index) const{
        fseek(this->obj, index, 0);
        char val = fgetc(this->obj);
        return val;
    }

    int getAddress() {

        char line[READ_BUF_SIZ];
        fgets(line, READ_BUF_SIZ, obj);
        return std::stoi(line);
    }

    int parseValue() {

        char line[READ_BUF_SIZ];
        memset(line, 0, sizeof(line));
        fgets(line, READ_BUF_SIZ, obj);

        int index = READ_BUF_SIZ - 1;
        for(; index >= 0; --index){
            if(line[index] == ':'){
                ++index; 
                break;
            }
        }
        return std::stoi((char*)&line[index]); 
    }
};
