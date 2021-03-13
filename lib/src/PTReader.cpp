#include "PTReader.h"

PTDump::PTReader::PTReader(std::string fName)
{
    fileName = fName;
    PathToFile = "./";
    init();
}
PTDump::PTReader::PTReader(std::string fName, std::string path)
{
    fileName = fName;
    PathToFile = path;
    init();
}
PTDump::PTReader::~PTReader()
{

}
void PTDump::PTReader::init()
{
    std::ifstream input_json_file(PathToFile + fileName + ".pt.json");
    input_json_file >> reader;
}
std::vector<std::pair<std::string, std::string>> PTDump::PTReader::getMustPointsToPairsAt(llvm::Instruction* Inst)
{ 

    std::vector<std::pair<std::string, std::string>> result;
    llvm::BasicBlock* BBOfInst = Inst->getParent();
    llvm::Function* FunctionOfInst = Inst->getFunction();

    int Inst_id;
    int Inst_Count = 1;
    for(llvm::Instruction& I : *BBOfInst)
    {
        if(&I == Inst)
        {
            Inst_id = Inst_Count;
            break;
        }
        Inst_Count++;
    }

    if(!reader["FlowInsensitivePointsToInfo"].empty())
    {        

    }
    else if(!reader["FlowSensitivePointsToInfo"].empty())
    {
        auto ProcList = reader["FlowSensitivePointsToInfo"]["Procedure"];
        for(auto proc : ProcList){
            if(proc["Functionid"] == FunctionOfInst->getName())
            {
                auto BBList = proc["BasicBlocks"];
                for(auto BB : BBList){
                    if(BB["BBid"] == BBOfInst->getName())
                    {
                        auto InstSet = BB["Instructions"];
                        for(auto I : InstSet)
                        {
                            if(I["Instructionid"] == Inst_id)
                            {
                                auto PTSet = I["PointsToSet"];
                                for(auto Pointer : PTSet){
                                    for(auto Pointee : Pointer["PointeeSet"])
                                    {
                                        if(Pointee["PointeeType"] == "Must")
                                            result.push_back(std::make_pair(Pointer["name"], Pointee["name"]));
                                    }
                                }
                            }                            
                        }
                    }
                }
            }
        }
    }
    else if(!reader["ContextSensitivePointsToInfo"].empty())
    {

    }
    return result;
}
std::vector<std::pair<std::string, std::string>> PTDump::PTReader::getMayPointsToPairsAt(llvm::Instruction* Inst)
{ 

    std::vector<std::pair<std::string, std::string>> result;
    llvm::BasicBlock* BBOfInst = Inst->getParent();
    llvm::Function* FunctionOfInst = Inst->getFunction();

    int Inst_id;
    int Inst_Count = 1;
    for(llvm::Instruction& I : *BBOfInst)
    {
        if(&I == Inst)
        {
            Inst_id = Inst_Count;
            break;
        }
        Inst_Count++;
    }

    if(!reader["FlowInsensitivePointsToInfo"].empty())
    {        

    }
    else if(!reader["FlowSensitivePointsToInfo"].empty())
    {
        auto ProcList = reader["FlowSensitivePointsToInfo"]["Procedure"];
        for(auto proc : ProcList){
            if(proc["Functionid"] == FunctionOfInst->getName())
            {
                auto BBList = proc["BasicBlocks"];
                for(auto BB : BBList){
                    if(BB["BBid"] == BBOfInst->getName())
                    {
                        auto InstSet = BB["Instructions"];
                        for(auto I : InstSet)
                        {
                            if(I["Instructionid"] == Inst_id)
                            {
                                auto PTSet = I["PointsToSet"];
                                for(auto Pointer : PTSet){
                                    for(auto Pointee : Pointer["PointeeSet"])
                                    {
                                        if(Pointee["PointeeType"] == "May")
                                            result.push_back(std::make_pair(Pointer["name"], Pointee["name"]));
                                    }
                                }
                            }                            
                        }
                    }
                }
            }
        }
    }
    else if(!reader["ContextSensitivePointsToInfo"].empty())
    {

    }
    return result;
}

void PTDump::PTReader::printToDot(llvm::Instruction*)
{
    
}