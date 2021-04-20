#include "FIS_PTWriter.h"

PTDump::FIS_PTWriter::FIS_PTWriter() : PTDump::PTWriter::PTWriter()
{

}
PTDump::FIS_PTWriter::FIS_PTWriter(PTDump::AnalysisType type, std::string File, std::string path) : PTDump::PTWriter::PTWriter(type, File, path)
{
    
}
PTDump::FIS_PTWriter::~FIS_PTWriter()
{
    WriteToJson();
}
void PTDump::FIS_PTWriter::addEdge(const llvm::Value* Pointer,const llvm::Value* Pointee)
{
    PointsToGraph[Pointer].insert(Pointee);
}
void PTDump::FIS_PTWriter::WriteToJson()
{
    writer["FlowInsensitivePointsToInfo"]["PointsToGraph"] = json::array();
    for(auto ptspair : PointsToGraph)
    {
        json obj;
        obj["name"] = ptspair.first->getName();
        llvm::Type* type;
        std::string type_str;
        llvm::raw_string_ostream rso2(type_str);
        if(llvm::isa<llvm::GlobalVariable>(ptspair.first))
        {
            type = ptspair.first->getType()->getPointerElementType();
            obj["procedure"] = "";
            obj["scope"] = "global";
        }
        else
        {
            type = ptspair.first->getType();
            obj["procedure"] = findFunction(ptspair.first)->getName();
            obj["scope"] = "local";
        }
        type->print(rso2);
        obj["type"] = rso2.str();
        obj["PointsToSet"] = json::array();
        
        for(auto pointee : ptspair.second)
        {
            json pointeeobj;
            pointeeobj["name"] = pointee->getName();
            llvm::Type* pointeetype;
            std::string type_str_2;
            llvm::raw_string_ostream rso(type_str_2);
            if(llvm::isa<llvm::GlobalVariable>(pointee))
            {
                pointeetype = pointee->getType()->getPointerElementType();
                pointeeobj["procedure"] = "";
                pointeeobj["scope"] = "global";
            }
            else
            {
                pointeetype = pointee->getType();
                pointeeobj["procedure"] = findFunction(pointee)->getName();
                pointeeobj["scope"] = "local";
            }
            pointeetype->print(rso);
            pointeeobj["type"] = rso.str();
            obj["PointsToSet"] += pointeeobj;
        }
        writer["FlowInsensitivePointsToInfo"]["PointsToGraph"] += obj;
    }
    std::string fname = fileName + ".pt.json";
    std::ofstream out(Output_File_Path + fname);
    out << std::setw(4) << writer << std::endl;
}
const llvm::Function* PTDump::FIS_PTWriter::findFunction(const llvm::Value* v)
{
    const llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction>(v);
    return I->getFunction();
}
bool PTDump::FIS_PTWriter::isValidPTFormat()
{
   return 1; 
}