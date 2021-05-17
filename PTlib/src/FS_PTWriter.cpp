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
bool PTDump::FS_PTWriter::WritePointsToinfoAt(llvm::Function* currFunc, int LineNo, std::string FileName, const llvm::Value* Pointer, const llvm::Value* Pointee, PointeeType type)
{
    if(!currFunc || !Pointer || !Pointee){
        std::cout << "Invalid input given (got Null Pointer). kindly Check the input to AddPointsToinfoAt()\n";
        return 0;
    }    
    if(Atype == PTDump::AnalysisType::FlowSensitive)
    {
        if(latestFile.empty() || latestFile.top() != FileName)
        {
            latestFile.push(FileName);
            AddFileInfo(FileName);
        }
        if(latestFunction.empty() || latestFunction.top() != currFunc)
        {
            latestFunction.push(currFunc);
            AddProcedureInfo(currFunc); 
        }
        if(latestLineNo.empty() || latestLineNo.top() != LineNo)
        {
            latestLineNo.push(LineNo);
            PointsToInfoAt(LineNo);
        }        
        addPointsTo(Pointer, Pointee, type);
    }    
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
    if(Atype == PTDump::AnalysisType::FlowSensitive)
    {
        if(latestLineNo.empty()){
            llvm::outs() << "Invalid use of library no info about the line in source provided";
            return 0;
        }        

        llvm::Function* LastProc = latestFunction.top();
        std::string LastFile = latestFile.top();
        int LastLineNo = latestLineNo.top();        

        json *FileArray = &(writer["FlowSensitivePointsToInfo"]["File"]);
        for(auto it = (*FileArray).begin(); it != (*FileArray).end(); it++)
        {
            if((*it)["name"] == LastFile)
            {
                for(auto i = ((*it)["Procedure"]).begin(); i != ((*it)["Procedure"]).end(); i++)
                {
                    if((*i)["FunctionID"] == LastProc->getName().str())
                    {                                                
                        for(auto i2 = (*i)["ProgramPoints"].begin(); i2 != (*i)["ProgramPoints"].end(); i2++)
                        {
                            if((*i2)["LineNo"] == LastLineNo)
                            {
                                bool search_poiner = 0;
                                for(auto i3 = (*i2)["PointsToSet"].begin(); i3 != (*i2)["PointsToSet"].end(); i3++)
                                {
                                    if((*i3)["name"] == Pointer->getName().str())
                                    {
                                        json PointeeObj;
                                        PointeeObj["name"] = Pointee->getName().str();
                                        
                                        std::string type_str_2;
                                        llvm::raw_string_ostream rso2(type_str_2);
                                        if(llvm::isa<llvm::GlobalVariable>(Pointee))
                                            Pointee->getType()->getPointerElementType()->print(rso2);
                                        else
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
                                    if(llvm::isa<llvm::GlobalVariable>(Pointer))
                                        Pointer->getType()->getPointerElementType()->print(rso);
                                    else
                                        Pointer->getType()->print(rso);                               

                                    obj["type"] = rso.str();                                                                        
                                    json PointeeObj;
                                    PointeeObj["name"] = Pointee->getName().str();
                                    
                                    std::string type_str_2;
                                    llvm::raw_string_ostream rso2(type_str_2);
                                    if(llvm::isa<llvm::GlobalVariable>(Pointee))
                                        Pointee->getType()->getPointerElementType()->print(rso2);
                                    else
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
    std::string LastFile = latestFile.top();
    if(Atype == PTDump::AnalysisType::FlowSensitive)
    {
        json *FileArray = &(writer["FlowSensitivePointsToInfo"]["File"]);        
        for(auto it = (*FileArray).begin(); it != (*FileArray).end(); it++)
        {
            if ((*it)["name"] == LastFile)
            {
                json obj;
                obj["FunctionID"] = currFunc->getName().str();
                obj["ProgramPoints"] = json::array();
                (*it)["Procedure"].push_back(obj);
            }
            
        }
    }    
    return 1;
}
/**
 * @brief Add Information about the basic block to which the current Instruction belongs to.
 * 
 * @param currBB Current Basic Block to which current instruction belongs.
 * @return true: Action executed successfully.
 * @return false: Action Failed.
 */
bool PTDump::FS_PTWriter::AddFileInfo(std::string FileName)
{       
    if(Atype == PTDump::AnalysisType::FlowSensitive)
    {     
        std::string FName = FileName;
        if(writer["FlowSensitivePointsToInfo"]["File"].empty())
            writer["FlowSensitivePointsToInfo"]["File"] = json::array();
        
        json procObj = json::object();        
        procObj["name"] = FileName;
        if(procObj["Procedure"].empty())
            procObj["Procedure"] = json::array();        

        writer["FlowSensitivePointsToInfo"]["File"] += procObj;                
    }    
    return 1;
}
/**
 * @brief 
 * 
 * @param Inst 
 * @return true: Action executed successfully.
 * @return false: Action Failed. 
 */
bool PTDump::FS_PTWriter::PointsToInfoAt(int LineNo)
{
    std::string File = latestFile.top();
    llvm::Function* currFunc = latestFunction.top();    
    
    if(Atype == PTDump::AnalysisType::FlowSensitive)
    {                                
        json *FileArray = &(writer["FlowSensitivePointsToInfo"]["File"]);
        for(auto it = (*FileArray).begin(); it != (*FileArray).end(); it++)
        {
            if((*it)["name"] == File)
            {
                for(auto i = ((*it)["Procedure"]).begin(); i != ((*it)["Procedure"]).end(); i++)
                {
                    if((*i)["FunctionID"] == currFunc->getName().str())
                    {
                        json obj = json::object();                        

                        obj["LineNo"] = LineNo;
                        if(obj["PointsToSet"].empty())
                            obj["PointsToSet"] = json::array();
                        
                        (*i)["ProgramPoints"].push_back(obj);
                    }
                }
            }
        }
    }    
    return 1;
}
/**
 * @brief 
 * 
 */
bool PTDump::FS_PTWriter::WritePointsToinfoAt(llvm::Function* currFunc, int LineNo, std::string FileName, std::string Pointer, std::string Pointee,std::string PointerType, std::string TypeOfPointee , PointeeType type)
{
    if(!currFunc)
    {
        std::cout << "Invalid input given (got Null Pointer). kindly Check the input to WritePointsToinfoAt()\n";
        return 0;
    }    
    if(Atype == PTDump::AnalysisType::FlowSensitive)
    {
        if(latestFile.empty() || latestFile.top() != FileName)
        {
            latestFile.push(FileName);
            AddFileInfo(FileName);
        }
        if(latestFunction.empty() || latestFunction.top() != currFunc)
        {
            latestFunction.push(currFunc);
            AddProcedureInfo(currFunc); 
        }
        if(latestLineNo.empty() || latestLineNo.top() != LineNo)
        {
            latestLineNo.push(LineNo);
            PointsToInfoAt(LineNo);
        }
        addPointsTo(Pointer, Pointee, PointerType, TypeOfPointee, type);
    }
    return 1;        
}
/**
 * @brief 
 * 
 */
bool PTDump::FS_PTWriter::addPointsTo(std::string Pointer, std::string Pointee, std::string PointerType, std::string TypeOfPointee , PointeeType type)
{
    if(Atype == PTDump::AnalysisType::FlowSensitive)
    {
        if(latestLineNo.empty()){
            llvm::outs() << "Invalid use of library no info about the line in source provided";
            return 0;
        }        

        llvm::Function* LastProc = latestFunction.top();
        std::string LastFile = latestFile.top();
        int LastLineNo = latestLineNo.top();        

        json *FileArray = &(writer["FlowSensitivePointsToInfo"]["File"]);
        for(auto it = (*FileArray).begin(); it != (*FileArray).end(); it++)
        {
            if((*it)["name"] == LastFile)
            {
                for(auto i = ((*it)["Procedure"]).begin(); i != ((*it)["Procedure"]).end(); i++)
                {
                    if((*i)["FunctionID"] == LastProc->getName().str())
                    {                                                
                        for(auto i2 = (*i)["ProgramPoints"].begin(); i2 != (*i)["ProgramPoints"].end(); i2++)
                        {
                            if((*i2)["LineNo"] == LastLineNo)
                            {
                                bool search_poiner = 0;
                                for(auto i3 = (*i2)["PointsToSet"].begin(); i3 != (*i2)["PointsToSet"].end(); i3++)
                                {
                                    if((*i3)["name"] == Pointer)
                                    {
                                        json PointeeObj;
                                        PointeeObj["name"] = Pointee;                                                                                
                                        PointeeObj["type"] = TypeOfPointee;
                                        PointeeObj["PointeeType"] = (type == PTDump::MayPointee) ? "May" : "Must";
                                        (*i3)["PointeeSet"] += PointeeObj;
                                        search_poiner = 1;
                                    }
                                }
                                if(!search_poiner)
                                {
                                    json obj;
                                    obj["name"] = Pointer;                                                                                                      
                                    obj["type"] = PointerType;                   
                                    json PointeeObj;
                                    PointeeObj["name"] = Pointee;
                                    PointeeObj["type"] = TypeOfPointee;
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