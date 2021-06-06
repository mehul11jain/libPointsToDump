#include "FS_PTReader.h"

/**
 * @brief Construct a new PTDump::FS_PTReader::FS_PTReader object
 * 
 * @param fName Name of the file to be queried.
 */
PTDump::FS_PTReader::FS_PTReader(std::string fName) : PTDump::PTReader::PTReader(fName)
{
    
}
/**
 * @brief Construct a new PTDump::FS_PTReader::FS_PTReader object
 * 
 * @param fName Name of the file to be queried.
 * @param path Path to the folder containing the file.
 */
PTDump::FS_PTReader::FS_PTReader(std::string fName, std::string path) : PTDump::PTReader::PTReader(fName, path)
{
    
}
/**
 * @brief Destroy the PTDump::PTReader::PTReader object
 * 
 */
PTDump::FS_PTReader::~FS_PTReader()
{
}
/**
 * @brief Returns all Must Points-to piars at a given Instruction.
 * 
 * @param LineNo Line number(in source file) at which Points-to Pairs needs to be queried.
 * @param Fname Source File for which we are specifying the information.
 * @return std::vector<std::pair<std::string, std::string>> All Must points-to pairs.
 */
std::vector<std::pair<std::string, std::string>> PTDump::FS_PTReader::getMustPointsToPairsAt(int LineNo, std::string Fname)
{

    std::vector<std::pair<std::string, std::string>> result;        
    
    if (!reader["FlowSensitivePointsToInfo"].empty())
    {
        auto FileList = reader["FlowSensitivePointsToInfo"]["File"];
        for (auto file : FileList)
        {
            if (file["name"] == Fname)
            {
                auto procList = file["Procedure"];
                for (auto proc : procList)
                {
                    for(auto pp : proc["ProgramPoints"])
                    {
                        if(pp["LineNo"] == LineNo)
                        {
                            auto PTSet = pp["PointsToSet"];
                            for (auto Pointer : PTSet)
                            {
                                for (auto Pointee : Pointer["PointeeSet"])
                                {
                                    if (Pointee["PointeeType"] == "Must")
                                        result.push_back(std::make_pair(Pointer["name"], Pointee["name"]));
                                }
                            }
                        }                        
                    }
                }
            }
        }
    }    
    return result;
}
/**
 * @brief Returns All may Points-to pairs at a given instruction.
 * 
 * @param LineNo Line number(in source file) at which Points-to Pairs needs to be queried.
 * @param Fname Source File for which we are quering the information.
 * @return std::vector<std::pair<std::string, std::string>> All may Points-to pairs.
 */
std::vector<std::pair<std::string, std::string>> PTDump::FS_PTReader::getMayPointsToPairsAt(int LineNo, std::string Fname)
{

    std::vector<std::pair<std::string, std::string>> result;        
    
    if (!reader["FlowSensitivePointsToInfo"].empty())
    {
        auto FileList = reader["FlowSensitivePointsToInfo"]["File"];
        for (auto file : FileList)
        {
            if (file["name"] == Fname)
            {
                auto procList = file["Procedure"];
                for (auto proc : procList)
                {
                    for(auto pp : proc["ProgramPoints"])
                    {
                        if(pp["LineNo"] == LineNo)
                        {
                            auto PTSet = pp["PointsToSet"];
                            for (auto Pointer : PTSet)
                            {
                                for (auto Pointee : Pointer["PointeeSet"])
                                {
                                    if (Pointee["PointeeType"] == "May")
                                        result.push_back(std::make_pair(Pointer["name"], Pointee["name"]));
                                }
                            }
                        }                        
                    }
                }
            }
        }
    }    
    return result;
}
/**
 * @brief Prints the points-to graph at a given Program Points.
 * 
 * @param LineNo Line number(in source file) at which Points-to Information needs to be queried.
 * @param Fname Source File for which we are quering the information.
 */
void PTDump::FS_PTReader::printToDot(int LineNo, std::string Fname)
{
    std::vector<std::pair<std::string, std::string>> MayPointsToSet, MustPointsToSet;
    
    MayPointsToSet = getMayPointsToPairsAt(LineNo, Fname);
    MustPointsToSet = getMustPointsToPairsAt(LineNo, Fname);    

    std::set<std::string> Nodes;
    for (auto x : MayPointsToSet)
    {
        Nodes.insert(x.first);
        Nodes.insert(x.second);
    }
    for (auto x : MustPointsToSet)
    {
        Nodes.insert(x.first);
        Nodes.insert(x.second);
    }
    std::string str;
    llvm::raw_string_ostream ss(str);    
    std::map<std::string, int> NodeMap;
    std::ofstream fstr;
    fstr.open("test.dot");
    fstr << "digraph {\n";
    fstr << "label=\" Points-To graph At " << ss.str() << "\";";
    fstr << "\nrankdir=\"LR\";\n";
    fstr << "node[shape=circle, width=0.5, color=lightblue2, style=filled];\n";
    int i = 1;
    for (auto n : Nodes)
    {
        NodeMap[n] = i;
        fstr << i++ << " [label=\"" << n << "\"]\n";
    }
    for (auto x : MayPointsToSet)
    {
        fstr << NodeMap[x.first] << " -> " << NodeMap[x.second] << ";\n";
    }
    for (auto x : MustPointsToSet)
    {
        fstr << NodeMap[x.first] << " -> " << NodeMap[x.second] << ";\n";
    }
    fstr << "}\n";
    fstr.close();
}
/**
 * @brief Checks whether a given pointee has a 'may' relationship with the pointer variable or not.
 * 
 * @param LineNo Line number(in source file) at which Points-to Information needs to be queried.
 * @param Fname Source File for which we are quering the information.
 * @param Pointer Pointer for which may-pointee needs to be checked,
 * @param Pointee Pointee to be checked whether is may-pointee or not.
 * @return true 
 * @return false 
 */
bool PTDump::FS_PTReader::isMayPointee(int LineNo, std::string Fname, llvm::Value *Pointer, llvm::Value *Pointee)
{
    std::vector<std::pair<std::string, std::string>> MayPointsToSet = getMayPointsToPairsAt(LineNo, Fname);
    std::string PointerStr = Pointer->getName().str();
    std::string PointeeStr = Pointee->getName().str();
    std::pair<std::string, std::string> Pointer_Pointee_pair = std::make_pair(PointerStr, PointeeStr);

    for (auto pairs : MayPointsToSet)
    {
        if (pairs == Pointer_Pointee_pair)
        {
            return 1;
        }
    }
    return 0;
}
/**
 * @brief Checks whether a given pointee has a 'must' relationship with the pointer variable or not.
 * 
 * @param LineNo Line number(in source file) at which Points-to Information needs to be queried.
 * @param Fname Source File for which we are quering the information.
 * @param Pointer Pointer for which must-pointee needs to be checked,
 * @param Pointee Pointee to be checked whether is must-pointee or not.
 * @return true 
 * @return false 
 */
bool PTDump::FS_PTReader::isMustPointee(int LineNo, std::string Fname, llvm::Value *Pointer, llvm::Value *Pointee)
{
    std::vector<std::pair<std::string, std::string>> MustPointsToSet = getMustPointsToPairsAt(LineNo, Fname);
    std::string PointerStr = Pointer->getName().str();
    std::string PointeeStr = Pointee->getName().str();
    std::pair<std::string, std::string> Pointer_Pointee_pair = std::make_pair(PointerStr, PointeeStr);

    for (auto pairs : MustPointsToSet)
    {
        if (pairs == Pointer_Pointee_pair)
        {
            return 1;
        }
    }
    return 0;
}
/**
 * @brief Prints the enitre points-to information in a Result.log file.
 * 
 */
void PTDump::FS_PTReader::printPointsToDump()
{
    std::ofstream fstr;
    fstr.open("Result.log");    
    if (!reader["FlowSensitivePointsToInfo"].empty())
    {        
        auto ProcList = reader["FlowSensitivePointsToInfo"]["Procedure"];
        for (auto proc : ProcList)
        {
            fstr << "Procedure:\t" << proc["Functionid"] << "\n";
            for (auto BB : proc["BasicBlocks"])
            {
                fstr << "\tBasic Block:\t" << BB["BBid"] << "\n";
                fstr << "\t\tIn[" << BB["BBid"] << "]: {";
                auto FirstInst = *(BB["Instructions"].begin());
                int cnt = 0;
                for (auto i : FirstInst["PointsToSet"])
                {
                    for (auto Pointee : i["PointeeSet"])
                    {
                        if (cnt != 0)
                            fstr << ", ";
                        fstr << "( " << i["name"] << ", " << Pointee["name"] << " )";
                        cnt++;
                    }
                }
                fstr << " }\n";
                fstr << "\t\tOut[" << BB["BBid"] << "]: {";
                auto LastInst = BB["Instructions"].back();
                cnt = 0;
                for (auto i : LastInst["PointsToSet"])
                {
                    for (auto Pointee : i["PointeeSet"])
                    {
                        if (cnt != 0)
                            fstr << ", ";
                        fstr << "( " << i["name"] << ", " << Pointee["name"] << " )";
                        cnt++;
                    }
                }
                fstr << " }\n\n";
            }
        }
    }    
    fstr.close();
}
/**
 * @brief Finds all the Pointees of a pointer at a given Program Point.
 * 
 * @param LineNo Line number(in source file) at which Points-to Pairs needs to be queried.
 * @param Fname Source File for which we are quering the information.
 * @param Pointer Pointer for which all the Pointees are needed.
 * @return std::vector<std::string> 
 */
std::vector<std::string> PTDump::FS_PTReader::getPointeesOf(int LineNo, std::string Fname, llvm::Value* Pointer)
{
    std::vector<std::string> result;

    if (!reader["FlowSensitivePointsToInfo"].empty())
    {
        auto FileList = reader["FlowSensitivePointsToInfo"]["File"];
        for(auto file : FileList)
        {
            if (file["name"] == Fname)
            {
                auto procList = file["Procedure"];
                for (auto proc : procList)
                {
                    for(auto pp : proc["ProgramPoints"])
                    {
                        if(pp["LineNo"] == LineNo)
                        {
                            auto PTSet = pp["PointsToSet"];
                            for (auto Pointer : PTSet)
                            {
                                for (auto Pointee : Pointer["PointeeSet"])
                                {                                    
                                    result.push_back(Pointee["name"]);
                                }
                            }
                        }                        
                    }
                }
            }
        }
    }    
    return result;
}