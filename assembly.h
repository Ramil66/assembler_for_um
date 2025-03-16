#ifndef ASSEMBLY_H
#define ASSEMBLY_H
#include <string>
#include <unordered_map>
#include <vector>
#include "type.h"

class Assembly
{
public:
    Assembly();
    void Translate(std::string filename,bool debug);
private:
    void passage(std::stringstream &istr);
    void saveStrData(std::vector<std::string>,int);
    void saveStrBSS(std::vector<std::string>,int);
    void saveStrText(std::vector<std::string>,int);
    int parsArihmetic(std::string ,int );
    void fillTags();
    void recordFile(std::string);
    int IP;
    std::unordered_map<std::string,int> reg={
        {"R1",1},{"R2",2},{"R3",3},{"R4",4},{"R5",5},{"R6",6},{"R7",7},{"R8",8},
        {"R9",9},{"R10",10},{"R11",11},{"R12",12},{"R13",13},{"R14",14},{"R15",15},{"R16",16}
    };
    std::unordered_map<std::string,Operations> mut_commands={
        {"ST",Operations::ST},{"STC",Operations::STC},{"SWAP",Operations::SWAP},{"SWAPC",Operations::SWAPC},
        {"SAC",Operations::SAC},{"SACC",Operations::SACC},{"LM",Operations::LM},{"STM",Operations::STM},
    };
    std::unordered_map<std::string,Operations> commands =
    {
        {"LR",Operations::LR},{"LNR",Operations::LNR},{"STR",Operations::STR},{"SWAPR",Operations::SWAPR},
        {"ANDR",Operations::ANDR},{"ORR",Operations::ORR},{"XORR",Operations::XORR},{"NOTR",Operations::NOTR},
        {"BCSR",Operations::BCSR},{"BCRR",Operations::BCRR},{"BALR",Operations::BALR},{"SACR",Operations::SACR},
        {"CR",Operations::CR},{"CCS",Operations::CCS},{"MCS",Operations::MCS},{"AR",Operations::AR},{"SR",Operations::SR},
        {"RSR",Operations::RSR},{"MR",Operations::MR},{"DR",Operations::DR},{"RDR",Operations::RDR},{"REMR",Operations::REMR},
        {"RREMR",Operations::RREMR},{"FAR",Operations::FAR},{"FSR",Operations::FSR},{"RFSR",Operations::RFSR},
        {"FMR",Operations::FMR},{"FDR",Operations::FDR},{"RFDR",Operations::RFDR},{"FLOATR",Operations::FLOATR},
        {"FIXR",Operations::FIXR},{"L",Operations::L},{"LN",Operations::LN},{"ST",Operations::ST},{"SWAP",Operations::SWAP},
        {"AND",Operations::AND},{"OR",Operations::OR},{"XOR",Operations::XOR},{"NOT",Operations::NOT},{"BCS",Operations::BCS},
        {"BCR",Operations::BAL},{"BAL",Operations::SAC},{"SAC",Operations::SAC},{"C"  ,Operations::C},{"EX",Operations::EX},
        {"A",Operations::A},{"S",Operations::S},{"RS",Operations::RS},{"M",Operations::M},{"D",Operations::D},{"RD",Operations::RD},
        {"REM",Operations::REM},{"RRFIM",Operations::RRFIM},{"FA",Operations::FA},{"FS",Operations::FS},{"RFS",Operations::RFS},
        {"FM",Operations::FM},{"FD",Operations::FD},{"RFD",Operations::RFD},{"FLOAT",Operations::FLOAT},{"FIX",Operations::FIX},
        {"LI",Operations::LI},{"LNI",Operations::LNI},{"ANDI",Operations::ANDI},{"ORI",Operations::ORI},{"XORI",Operations::XORI},
        {"NOTI",Operations::NOTI},{"CI",Operations::CI},{"LA",Operations::LA},{"AI",Operations::AI},{"SI",Operations::SI},
        {"RSI",Operations::RSI},{"MI",Operations::MI},{"DI",Operations::DI},{"RDI",Operations::RDI},{"REMI",Operations::REMI},
        {"RREMI",Operations::RREMI},{"FAI",Operations::FAI},{"FSI",Operations::FSI},{"RFSI",Operations::RFSI},{"FMI",Operations::FMI},
        {"FDI",Operations::FDI},{"RFDI",Operations::RFDI},{"FLOATI",Operations::FLOATI},{"FIXI",Operations::FIXI},{"LC",Operations::LC},
        {"LNC",Operations::LNC},{"STC",Operations::STC},{"SWAPC",Operations::SWAPC},{"ANDC",Operations::ANDC},{"ORC",Operations::ORC},
        {"XORC",Operations::XORC},{"NOTC",Operations::NOTC},{"SACC",Operations::SACC},{"CC",Operations::CC},{"LM",Operations::LM},
        {"STM",Operations::STM},{"AC",Operations::AC},{"SC",Operations::SC},{"RSC",Operations::RSC},{"MC",Operations::MC},
        {"DC",Operations::DC},{"RDC",Operations::RDC},{"REMC",Operations::REMC},{"RREMC",Operations::RREMC},{"FLOOR",Operations::FLOOR},
        {"CEIL",Operations::CEIL},{"MIN",Operations::MIN},{"MAX",Operations::MAX},{"SHIFTL",Operations::SHIFTL},{"SHIFTC",Operations::SHIFTC},
        {"SHIFTR",Operations::SHIFTR},{"SHIFTA",Operations::SHIFTA}
    };
    std::string start_tags;
    std::vector<std::string> code;
    std::vector<std::string> bin_code;
    std::unordered_map<std::string,int> text_tags;
    std::unordered_map<std::string,int> data_tags;
    std::unordered_map<std::string,int> bss_tags;
    std::unordered_map<int,std::string> uknown_tags;
};
#endif // ASSEMBLY_H
