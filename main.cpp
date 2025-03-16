#include <iostream>
#include "assembly.h"
#include <exception>


int main(int argc, char* argv[]){
    Assembly myAsm;
    try{
        myAsm.Translate(argv[argc-1],argv[1]=="-debug");
    }
    catch(const std::exception& error){
        std::cout<<error.what();
    }
    return 0;
}
