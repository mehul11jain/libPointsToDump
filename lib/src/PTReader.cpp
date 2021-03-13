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

void PTDump::PTReader::printToDot(llvm::Instruction* Inst)
{
    std::vector<std::pair<std::string, std::string>> MayPointsToSet, MustPointsToSet;
    MayPointsToSet = getMayPointsToPairsAt(Inst);
    MustPointsToSet = getMustPointsToPairsAt(Inst);

    std::set<std::string> Nodes;
    for(auto x : MayPointsToSet){
        Nodes.insert(x.first);
        Nodes.insert(x.second);
    }
    for(auto x : MustPointsToSet){
        Nodes.insert(x.first);
        Nodes.insert(x.second);
    }
    std::string str;
    llvm::raw_string_ostream ss(str);
    ss << *Inst;    
    std::map<std::string, int> NodeMap;
    std::ofstream fstr;
    fstr.open("test.dot");
    fstr << "digraph {\n";
    fstr << "label=\" Points-To graph At " << ss.str() <<"\"";
    fstr << "rankdir=\"LR\"\n";
    fstr << "node[shape=circle, width=0.5, color=lightblue2, style=filled]\n";
    int i = 1;
    for(auto n : Nodes)    
    {
        NodeMap[n] = i;
        fstr << i++ << " [label=\"" << n << "\"]\n";
    }    
    for(auto x : MayPointsToSet)
    {
        fstr << NodeMap[x.first] << " -> " << NodeMap[x.second] << ";\n";
    }
    fstr << "}\n";
    fstr.close();
}
bool PTDump::PTReader::isMayPointee(llvm::Instruction* Inst, llvm::Value* Pointer, llvm::Value* Pointee)
{
    std::vector<std::pair<std::string,std::string>> MayPointsToSet = getMayPointsToPairsAt(Inst);
    std::string PointerStr = Pointer->getName().str();
    std::string PointeeStr = Pointee->getName().str();
    std::pair<std::string, std::string> Pointer_Pointee_pair = std::make_pair(PointerStr,PointeeStr);

    for(auto pairs : MayPointsToSet)
    {
        if(pairs == Pointer_Pointee_pair)
        {
            return 1;
        }
    }
    return 0;

}

bool PTDump::PTReader::isMustPointee(llvm::Instruction* Inst, llvm::Value* Pointer, llvm::Value* Pointee)
{
    std::vector<std::pair<std::string,std::string>> MustPointsToSet = getMustPointsToPairsAt(Inst);
    std::string PointerStr = Pointer->getName().str();
    std::string PointeeStr = Pointee->getName().str();
    std::pair<std::string, std::string> Pointer_Pointee_pair = std::make_pair(PointerStr,PointeeStr);

    for(auto pairs : MustPointsToSet)
    {
        if(pairs == Pointer_Pointee_pair)
        {
            return 1;
        }
    }
    return 0;
}