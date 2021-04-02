#include "FS_PTWriter.h"

/**
 * @brief Construct a new PTDump::FS_PTWriter::FS_PTWriter object.
 * \par Default Constructor for creating a PTWriter object.
 * \par Analysis type is FlowInsensitive by default.
 * 
 */
PTDump::FS_PTWriter::FS_PTWriter() : PTDump::PTWriter::PTWriter()
{
    
}
/**
 * @brief Construct a new PTDump::FS_PTWriter::FS_PTWriter object
 * 
 * @param type Defines the Analysis Type.
 * @param path Absolute path to write in the file.
 * @param File represents the file name for json.
 */
PTDump::FS_PTWriter::FS_PTWriter(PTDump::AnalysisType type, std::string File, std::string path) : PTDump::PTWriter::PTWriter(type, File, path)
{
    
}
/**
 * @brief Destroy the PTDump::FS_PTWriter::FS_PTWriter object
 * 
 */
PTDump::FS_PTWriter::~FS_PTWriter()
{
    WriteToJson(fileName);
}
/**
 * @brief Wrapper Function for user to invoke to store the points-to information in json.
 * 
 * @param currFunc current Function under analysis.
 * @param currBB   current BasicBlock under analysis.
 * @param currInst current Instruction to be processed.
 * @param Pointer  Pointer variable for which pointee set needs to be updated.
 * @param Pointee Pointee to be added.
 * @param type Type of the Pointee i.e either Must or May.
 * @return true: Action executed successfully.
 * @return false: Action Failed. 
 */
bool PTDump::FS_PTWriter::AddPointsToinfoAt(llvm::Function* currFunc, llvm::BasicBlock* currBB, llvm::Instruction* currInst, const llvm::Value* Pointer, const llvm::Value* Pointee, PointeeType type)
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
/**
 * @brief Adds the Points-to Information in the json.
 * 
 * @param Pointer Pointer variable for which pointee set needs to be updated.
 * @param Pointee Pointee to be added.
 * @param type Type of the Pointee i.e either Must or May.
 * @return true: Action executed successfully.
 * @return false: Action Failed.
 */
 bool PTDump::FS_PTWriter::addPointsTo(const llvm::Value* Pointer, const llvm::Value* Pointee, PointeeType type)
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
/**
 * @brief Add the Function information of the instruction we are currently processing.
 * 
 * @param currFunc Function to which current Instruction belongs to.
 * @return true: Action executed successfully.
 * @return false: Action Failed. 
 */
bool PTDump::FS_PTWriter::AddProcedureInfo(llvm::Function* currFunc)
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
/**
 * @brief Add Information about the basic block to which the current Instruction belongs to.
 * 
 * @param currBB Current Basic Block to which current instruction belongs.
 * @return true: Action executed successfully.
 * @return false: Action Failed.
 */
bool PTDump::FS_PTWriter::AddBasicBlockInfo(llvm::BasicBlock* currBB)
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
/**
 * @brief 
 * 
 * @param Inst 
 * @return true: Action executed successfully.
 * @return false: Action Failed. 
 */
bool PTDump::FS_PTWriter::PointsToInfoAt(llvm::Instruction* Inst)
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
/**
 * @brief Creates a new JSON file and dump the points-to information into it.
 * 
 * @param Filename Name to be given to the newly created JSON file.
 */
void PTDump::FS_PTWriter::WriteToJson(std::string Filename)
{
    std::string fname = Filename + ".pt.json";
    std::ofstream out(Output_File_Path + fname);
    out << std::setw(4) << writer << std::endl;
}
bool PTDump::FS_PTWriter::isValidPTFormat()
{
    // To implement
    return 1;
}