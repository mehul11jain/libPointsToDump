#include "PTWriter.h"

PTDump::PTWriter::PTWriter()
{
    Atype = FlowInsensitive;
    fileName = "default";
    writer["FlowInsensitivePointsToInfo"] = json::object();
    writer["FlowSensitivePointsToInfo"] = json::object();
    writer["ContextSensitivePointsToInfo"] = json::object();
}
PTDump::PTWriter::PTWriter(PTDump::AnalysisType type, std::string File)
{
    Atype = type;
    fileName = File;
    writer["FlowInsensitivePointsToInfo"] = json::object();
    writer["FlowSensitivePointsToInfo"] = json::object();
    writer["ContextSensitivePointsToInfo"] = json::object();
}
PTDump::PTWriter::~PTWriter()
{
    WriteToJson(fileName);
}
bool PTDump::PTWriter::AddPointsToinfoAt(llvm::Function* currFunc, llvm::BasicBlock* currBB, llvm::Instruction* currInst, llvm::Value* Pointer, llvm::Value* Pointee, PointeeType type)
{
    if(!currFunc || !currBB || !currInst || !Pointer || !Pointee){
        std::cout << "Invalid input given (got Null Pointer). kindly Check the input to AddPointsToinfoAt()\n";
        return 0;
    }
    if(Atype == PTDump::AnalysisType::FlowInsensitive){}
    else if(Atype == PTDump::AnalysisType::FlowSensitive)
    {
        if(latestFunction.empty() || latestFunction.top() != currFunc)
        {
            latestFunction.push(currFunc);
            AddProcedureInfo(currFunc); 
        }
        if(latestBB.empty() || latestBB.top() != currBB)
        {
            latestBB.push(currBB);
            AddBasicBlockInfo(currBB);
        }

        if(latestInst.empty() || latestInst.top() != currInst)// llvm::outs() << *currBB << *currInst << *currFunc << "\n";
        {
            latestInst.push(currInst);
            PointsToInfoAt(currInst);
        }
        addPointsTo(Pointer, Pointee, type);
    }
    else if(Atype == PTDump::AnalysisType::ContextSensitive){}
    return 1;
}
bool PTDump::PTWriter::addPointsTo(llvm::Value* Pointer, llvm::Value* Pointee, PointeeType type)
{
    // To implement  
    if(Atype == PTDump::AnalysisType::FlowInsensitive){}
    else if(Atype == PTDump::AnalysisType::FlowSensitive)
    {
        if(latestBB.empty()){
            llvm::outs() << "Invalid use of library no info about the basic block of instruction provided";
            return 0;
        }
        else if(latestFunction.empty()){
            llvm::outs() << "Invalid use of library no info about the Procedure of instruction provided";
            return 0;
        }
        else if(latestInst.empty()){
            llvm::outs() << "Invalid use of library no info about the instruction provided";
            return 0;
        }

        llvm::BasicBlock* LastBB = latestBB.top();
        llvm::Function* LastProc = latestFunction.top();
        llvm::Instruction* LastInst = latestInst.top();

        int Inst_id;
        int Inst_Count = 1;
        for(llvm::Instruction& I : *LastBB)
        {
            if(&I == LastInst)
            {
                Inst_id = Inst_Count;
                break;
            }
            Inst_Count++;
        }

        json *ProcedureArray = &(writer["FlowSensitivePointsToInfo"]["Procedure"]);
        for(auto it = (*ProcedureArray).begin(); it != (*ProcedureArray).end(); it++)
        {
            if((*it)["Functionid"] == LastProc->getName().str())
            {
                for(auto i = ((*it)["BasicBlocks"]).begin(); i != ((*it)["BasicBlocks"]).end(); i++)
                {
                    if((*i)["BBid"] == LastBB->getName().str())
                    {                                                
                        for(auto i2 = (*i)["Instructions"].begin(); i2 != (*i)["Instructions"].end(); i2++)
                        {
                            if((*i2)["Instructionid"] == Inst_id){
                                bool search_poiner = 0;
                                for(auto i3 = (*i2)["PointsToSet"].begin(); i3 != (*i2)["PointsToSet"].end(); i3++)
                                {
                                    if((*i3)["name"] == Pointer->getName().str())
                                    {
                                        json PointeeObj;
                                        PointeeObj["name"] = Pointee->getName().str();
                                        
                                        std::string type_str_2;
                                        llvm::raw_string_ostream rso2(type_str_2);
                                        Pointee->getType()->print(rso2);

                                        PointeeObj["type"] = rso2.str();
                                        PointeeObj["PointeeType"] = (type == PTDump::MayPointee) ? "May" : "Must";
                                        (*i3)["PointeeSet"] += PointeeObj;
                                        search_poiner = 1;
                                    }
                                }
                                if(!search_poiner)
                                {
                                    json obj;
                                    obj["name"] = Pointer->getName().str(); 
                                    
                                    std::string type_str;
                                    llvm::raw_string_ostream rso(type_str);
                                    Pointer->getType()->print(rso);                                

                                    obj["type"] = rso.str();                                                                        
                                    json PointeeObj;
                                    PointeeObj["name"] = Pointee->getName().str();
                                    
                                    std::string type_str_2;
                                    llvm::raw_string_ostream rso2(type_str_2);
                                    Pointee->getType()->print(rso2);

                                    PointeeObj["type"] = rso2.str();
                                    PointeeObj["PointeeType"] = (type == PTDump::MayPointee) ? "May" : "Must";
                                    obj["PointeeSet"] += PointeeObj;
                                    (*i2)["PointsToSet"] += obj;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if(Atype == PTDump::AnalysisType::ContextSensitive){}
    return 1;
}

bool PTDump::PTWriter::AddProcedureInfo(llvm::Function* currFunc)
{
    if(Atype == PTDump::AnalysisType::FlowInsensitive){}
    else if(Atype == PTDump::AnalysisType::FlowSensitive)
    {
        std::string FuncName = currFunc->getName().str();
        if(writer["FlowSensitivePointsToInfo"]["Procedure"].empty())
            writer["FlowSensitivePointsToInfo"]["Procedure"] = json::array();
        
        json procObj = json::object();        
        procObj["Functionid"] = FuncName;
        if(procObj["BasicBlocks"].empty())
            procObj["BasicBlocks"] = json::array();        

        writer["FlowSensitivePointsToInfo"]["Procedure"] += procObj;        
    }
    else if(Atype == PTDump::AnalysisType::ContextSensitive){}
    return 1;
}
bool PTDump::PTWriter::AddBasicBlockInfo(llvm::BasicBlock* currBB)
{
    llvm::Function* currFunc = latestFunction.top();

    if(Atype == PTDump::AnalysisType::FlowInsensitive){}
    else if(Atype == PTDump::AnalysisType::FlowSensitive)
    {                                
        json *ProcedureArray = &(writer["FlowSensitivePointsToInfo"]["Procedure"]);        
        for(auto it = (*ProcedureArray).begin(); it != (*ProcedureArray).end(); it++)
        {
            if((*it)["Functionid"] == currFunc->getName().str())
            {
                json obj;
                obj["BBid"] = currBB->getName().str(); 
                if(obj["Instructions"].empty())               
                    obj["Instructions"] = json::array();                

                (*it)["BasicBlocks"].push_back(obj);
            }
        }        
    }
    else if(Atype == PTDump::AnalysisType::ContextSensitive){}
    return 1;
}
bool PTDump::PTWriter::PointsToInfoAt(llvm::Instruction* Inst)
{
    llvm::Function* currFunc = latestFunction.top();
    llvm::BasicBlock* currBB = latestBB.top();

    int Inst_id;
    int Inst_Count = 1;
    for(llvm::Instruction& I : *currBB)
    {
        if(&I == Inst){
            Inst_id = Inst_Count;
            break;
        }
        Inst_Count++;
    }

    if(Atype == PTDump::AnalysisType::FlowInsensitive){}
    else if(Atype == PTDump::AnalysisType::FlowSensitive)
    {                                
        json *ProcedureArray = &(writer["FlowSensitivePointsToInfo"]["Procedure"]);
        for(auto it = (*ProcedureArray).begin(); it != (*ProcedureArray).end(); it++)
        {
            if((*it)["Functionid"] == currFunc->getName().str())
            {
                for(auto i = ((*it)["BasicBlocks"]).begin(); i != ((*it)["BasicBlocks"]).end(); i++)
                {
                    if((*i)["BBid"] == currBB->getName().str())
                    {
                        json obj = json::object();                        

                        obj["Instructionid"] = Inst_id;
                        if(obj["PointsToSet"].empty())
                            obj["PointsToSet"] = json::array();
                        
                        (*i)["Instructions"].push_back(obj);
                    }
                }
            }
        }
    }
    else if(Atype == PTDump::AnalysisType::ContextSensitive){}
    return 1;
}
void PTDump::PTWriter::WriteToJson(std::string Filename)
{
    std::string fname = Filename + ".pt.json";
    std::ofstream out(fname);
    out << std::setw(4) << writer << std::endl;
}