#include "assembly.h"
#include <fstream>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <exception>


Assembly::Assembly() {}
void Assembly::passage(std::stringstream &istr){
    std::string str;
    std::string section;
    int line=0;
    std::vector<std::string> s;
    getline(istr,str);
    if(!std::regex_match(str,std::regex("^[ ]*global[ ]+[a-z|A-Z|\.|@|_|\?|\$]+[@|_|\?|\$|a-z|A-Z|0-9]"))&&start_tags.empty())
        throw std::exception("error: line "+line+" invalid global format");
    str=std::regex_replace(str,std::regex(" {2,}"), " ");
    boost::trim(str);
    boost::split(s,str,boost::is_any_of(" "));
    start_tags=s[1];
    while(std::getline(istr,str)){
        line++;
        if(!str.empty()){
            if(str.contains("section")){
                if(std::regex_match(str,std::regex("^[ ]*section[ ]+.data[ ]*$")))
                    section="data";
                else if(std::regex_match(str,std::regex("^[ ]*section[ ]+.text[ ]*$")))
                    section="text";
                else if(std::regex_match(str,std::regex("^[ ]*section[ ]+.bss[ ]*$")))
                    section="bss";
                else
                    throw std::exception("error: line "+line+" invalid section format");
            }
            str=std::regex_replace(str,std::regex("[,]")," , ");
            str=std::regex_replace(str,std::regex("[:]")," : ");
            str=std::regex_replace(str,std::regex("[;]")," ; ");
            str=std::regex_replace(str,std::regex(" {2,}"), " ");
            boost::trim(str);
            boost::split(s,str,boost::is_any_of(" "));
            if(s[0]!=";"){
                switch(section){
                    code.push_back(str);
                    case "data":
                        saveStrData(s,line);
                        break;
                    case "bss":
                        saveStrBSS(s,line);
                        break;
                    case "text":
                        saveStrText(s,line);
                        break;
                }
            }
        }
    }
}
int Assembly::parsArihmetic(std::string str,int answ){}
void Assembly::saveStrData(std::vector<std::string> str,int line){
    int state=1;
    int count_bite;
    for(int i=0;i<str.size();i++){
        switch (state) {
            case 1:
                if(!std::regex_match(str[i],std::regex("^[a-z|A-Z|\.|@|_|\?|\$]+[@|_|\?|\$|a-z|A-Z|0-9]*$")))
                    throw std::exception("error: line "+line+" invalid label format");
                data_tags.insert({str[i],IP});
                state=2;
                break;
            case 2:
                if(str[i]!=":")
                    i--;
                state=3;
                break;
            case 3:
                if(str[i]=="db")
                    count_bite=1;
                else if(str[i]=="dw")
                    count_bite=2;
                else if(str[i]=="dd")
                    count_bite=4;
                else if(str[i]=="dq")
                    count_bite=8;
                else
                    throw std::exception("error: line "+line+" invalid directive format");
                state=4;
                break;
            case 4:
                if(std::regex_match(str[i],std::regex("^(([1-9]+[0-9]*)|(0x[0-9A-Fa-f]+)|(0b[0-1]+)$"))){
                    switch(count_bite){
                        case 1:
                            bin_code.push_back(std::bitset<8>(std::stoi(str[i])).to_string());
                            break;
                        case 2:
                            bin_code.push_back(std::bitset<16>(std::stoi(str[i])).to_string());
                            break;
                        case 4:
                            bin_code.push_back(std::bitset<32>(std::stol(str[i])).to_string());
                            break;
                        case 8:
                            bin_code.push_back(std::bitset<64>(std::stoll(str[i])).to_string());;
                            break;
                    }
                }
                else if(std::regex_match(str[i],std::regex("^[+-]?[0-9]+\\.?[0-9]*([eE][+-]?[0-9]+)?$"))){
                    switch(count_bite){
                        case 4:
                            bin_code.push_back(std::bitset<32>(std::stof(str[i])).to_string());
                            break;
                        case 8:
                            bin_code.push_back(std::bitset<64>(std::stod(str[i])).to_string());;
                            break;
                    }
                }
                else if(std::regex_match(str[i],std::regex("^(\"[\s\S]*\"))$"))){
                    for(int j=0;j<str[i].size();j++){
                        bin_code.push_back(std::to_string(str[i][j]));
                    }
                }
                else
                    throw std::exception("error: line "+line+" invalid data format");
                state=5;
                if(i==str.size()-1||str[i+1]==";"){
                    IP+=count_bite;
                    bin_code.push_back("\n");
                    return;
                }
                break;
            case 5:
                if(!(str[i]==","&&i!=str.size()-1))
                    throw std::exception("error: line "+line+" missing or has an extra comma");
                state=4;
                break;
        }
    }
    throw std::exception("error: line "+line);
}
void Assembly::saveStrBSS(std::vector<std::string>,int){
    int state=1;
    int count_bite;
    for(int i=0;i<str.size();i++){
        switch (state) {
            case 1:
                if(!std::regex_match(str[i],std::regex("^[a-z|A-Z|\.|@|_|\?|\$]+[@|_|\?|\$|a-z|A-Z|0-9]*$")))
                    throw std::exception("error: line "+line+" invalid label format");
                bss_tags.insert({str[i],IP});
                state=2;
                break;
            case 2:
                if(str[i]!=":")
                    i--;
                state=3;
                break;
            case 3:
                if(str[i]=="resb")
                    count_bite=1;
                else if(str[i]=="resw")
                    count_bite=2;
                else if(str[i]=="resd")
                    count_bite=4;
                else if(str[i]=="resq")
                    count_bite=8;
                else
                    throw std::exception("error: line "+line+" invalid directive format");
                state=4;
                break;
            case 4:
                if(!std::regex_match(str[i],std::regex("^[1-9]+[0-9]*$")))
                    throw std::exception("error: line "+line+" invalid count");
                int count_el=std::stoi(str[i]);
                for(int j=0;j<count_el;j++){
                    switch(count_bite){
                        case 1:
                            bin_code.push_back(std::bitset<8>(0).to_string());
                            break;
                        case 2:
                            bin_code.push_back(std::bitset<16>(0).to_string());
                            break;
                        case 4:
                            bin_code.push_back(std::bitset<32>(0).to_string());
                            break;
                        case 8:
                            bin_code.push_back(std::bitset<64>(0).to_string());
                            break;
                    }
                }
                if(i==str.size()-1||str[i+1]==";"){
                    IP+=count_bite;
                    bin_code.push_back("\n");
                    return;
                }
                throw std::exception("error: line "+line+" invalid");
        }
    }
    throw std::exception("error: line "+line+" invalid label");
}

void Assembly::saveStrText(std::vector<std::string> str){
    int state=1;
    std::string bin_str="";
    for(int i=0;i<str.size();i++){
        switch(state){
            case 1://Начало проверка верности команды
                if(commands.contains(str[i])){
                    bin_str=std::bitset<7>(commands[str[i]]).to_string();
                    state=2;
                }
                else if(std::regex_match(str[i],std::regex("^[a-z|A-Z|\.|@|_|\?|\$]+[@|_|\?|\$|a-z|A-Z|0-9]*$"))){
                    text_tags.insert({str[i],0});
                    state=7;
                }
                else
                    throw std::exception("error: line "+line+" invalid command format");
                break;
            case 2://запятая после команды
                if(str[i]!=",")
                    throw std::exception("error: line "+line+" missing comma after command");
                state=3;
                break;
            case 3://R1
                if(!reg.contains(str[i]))
                    throw std::exception("error: line "+line+" there is a missing first register");
                bin_str+=std::bitset<4>(reg[str[i]]).to_string();
                state=4;
                break;
            case 4://2 операнд
                if(str[i]=="*"){//косвенная адресация
                    bin_str="1"+bin_str;
                    state=4;
                }
                else if(reg.contains(str[i])&&(i==str.size()-1||str[i+1]==";")){//R2
                    bin_str+=std::bitset<4>(reg[str[i]]).to_string();
                    bin_code.push_back(bin_str);
                    if(str.size()==6)
                        IP+=2;
                    IP+=2;
                    bin_code.push_back("\n");
                    return;
                }
                //Непосредственный операнд
                //^[+-]?[0-9]+\\.?[0-9]*([eE][+-]?[0-9]+)?$
                else if(std::regex_match(str[i],std::regex("^(([1-9]+[0-9]*)|(0x[0-9A-Fa-f]+)|(0b[0-1]+)$"))&&(i==str.size()-1||str[i+1]==";")){
                    bin_str+=std::bitset<20>(std::stol(str[i])).to_string();
                    bin_code.push_back(bin_str);
                    IP+=4;
                    bin_code.push_back("\n");
                    return;
                }
                else if(std::regex_match(str[i],std::regex("^[+-]?[0-9]+\\.?[0-9]*([eE][+-]?[0-9]+)?$"))&&(i==str.size()-1||str[i+1]==";")){
                    bin_str+=std::bitset<32>(std::stof(str[i])).to_string();
                    bin_code.push_back(bin_str);
                    IP+=4;
                    bin_code.push_back("\n");
                    return;
                }
                //Адрес
                else if((i-str.size()==2)&&std::regex_match(str[i],std::regex("^(0x[0-9A-Fa-f]+)|(\[?[a-z|A-Z|\.|@|_|\?|\$]+[@|_|\?|\$|a-z|A-Z|0-9]*\]?)$")))
                    state=5;
                else
                    throw std::exception("error: line "+line+" invalid address format");
                break;
            case 5://Адрес
                //Если адрес задан числом в 16-ном формате
                if(std::regex_match(str[i],std::regex("^(0x[0-9A-Fa-f]+)$")))
                    bin_str+=std::bitset<16>(std::stol(str[i])).to_string();
                else{
                    if(bss_tags.contains(str[i])){
                        bin_str += std::bitset<16>(bss_tags[str[i]]).to_string();
                    }
                    else if(data_tags.contains(str[i])){
                        bin_str += std::bitset<16>(data_tags[str[i]]).to_string();
                    }
                    else if(text_tags.contains(str[i])){
                        if(mut_commands.contains(str[i-3]))
                            throw std::exception("error: line "+line+" change text section");
                        bin_str += std::bitset<16>(text_tags[str[i]]).to_string();
                    }
                    else{
                        bin_code.push_back(bin_str);
                        bin_code.push_back(std::bitset<16>(0).to_string());
                        uknown_tags.insert({bin_code.size()-1, str[i]});
                        bin_str.clear();
                    }
                }
                state=6;
                break;
            case 6://Запятая после адреса
                if(str[i]!=",")
                    throw std::exception("error: line "+line+" missing coma after address");
                state=4;
                break;
            case 7:
                if(str[i]!=":")
                    throw std::exception("error: line "+line+" invalid tags format");
                text_tags[str[i-1]]=IP;
                if(i==str.size()-1){
                    bin_code.push_back("\n");
                    return;
                }
                state=1;
                break;

        }
    }
    throw std::exception("error: line "+line);
}
void Assembly::fillTags(){
    bin_code[0]=std::bitset<16>(text_tags[start_tags]).to_string();
    for(auto& item : uknown_tags){
         if(bss_tags.contains(item.second)){
            bin_code[item.first]=std::bitset<16>(bss_tags[item.second]).to_string();
        }
        else if(data_tags.contains(item.second)){
            bin_code[item.first]=std::bitset<16>(data_tags[item.second]).to_string();
        }
        else if(text_tags.contains(item.second)){
            bin_code[item.first]=std::bitset<16>(text_tags[item.second]).to_string();
        }
        else
            throw std::exception("error: unidentified tag");
    }
}
void Assembly::recordFile(std::string filename){
    std::ofstream file_bin;
    std::ofstream file;
    file_bin.open(filename, std::ios::binary);
    file.open("listing-"+filename);
    file<<"Listing\n";
    int j=0;
    std::string tempStr="";
    for(int i=0;i<bin_code.size();i++){
        if(bin_code=="\n"){
            file<<code[j]<<"\t|\t"<<tempStr;
            tempStr="";
        }
        else{
            file_bin<<bin_code[i];
            tempStr+=bin_code[i];
        }
    }
}
void Assembly::recordTagsFile(){
    std::ofstream file;
    file.open("tags-"+filname);
    for(auto& item : text_tags)
        file<<item.first<<" "<<item.second<<"\n";
}
void Assembly::Translate(std::string filename,bool debug){
    std::ifstream file(filename);
    if(file.is_open()){
        std::stringstream istr;
        istr << file.rdbuf();
        IP=0;
        passage(istr);
        fillTags();
        if(debug) recordFile();
        recordFile(filename);
    }
}
