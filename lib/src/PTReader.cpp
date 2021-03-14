#include "PTReader.h"

/**
 * @brief Construct a new PTDump::PTReader::PTReader object
 * 
 * @param fName Name of the file to be queried.
 */
PTDump::PTReader::PTReader(std::string fName)
{
    fileName = fName;
    PathToFile = "./";
    init();
}
/**
 * @brief Construct a new PTDump::PTReader::PTReader object
 * 
 * @param fName Name of the file to be queried.
 * @param path Path to the folder containing the file.
 */
PTDump::PTReader::PTReader(std::string fName, std::string path)
{
    fileName = fName;
    PathToFile = path;
    init();
}
/**
 * @brief Destroy the PTDump::PTReader::PTReader object
 * 
 */
PTDump::PTReader::~PTReader()
{
}
/**
 * @brief Initializa a JSON Object using the input JSON File.
 * 
 */
void PTDump::PTReader::init()
{
    std::ifstream input_json_file(PathToFile + fileName + ".pt.json");
    input_json_file >> reader;
}
/**
 * @brief Returns all Must Points-to piars at a given Instruction.
 * 
 * @param Inst intruction at which points-to info is queried.
 * @return std::vector<std::pair<std::string, std::string>> All Must points-to pairs.
 */
std::vector<std::pair<std::string, std::string>> PTDump::PTReader::getMustPointsToPairsAt(llvm::Instruction *Inst)
{

    std::vector<std::pair<std::string, std::string>> result;
    llvm::BasicBlock *BBOfInst = Inst->getParent();
    llvm::Function *FunctionOfInst = Inst->getFunction();

    int Inst_id;
    int Inst_Count = 1;
    for (llvm::Instruction &I : *BBOfInst)
    {
        if (&I == Inst)
        {
            Inst_id = Inst_Count;
            break;
        }
        Inst_Count++;
    }

    if (!reader["FlowInsensitivePointsToInfo"].empty())
    {
    }
    else if (!reader["FlowSensitivePointsToInfo"].empty())
    {
        auto ProcList = reader["FlowSensitivePointsToInfo"]["Procedure"];
        for (auto proc : ProcList)
        {
            if (proc["Functionid"] == FunctionOfInst->getName())
            {
                auto BBList = proc["BasicBlocks"];
                for (auto BB : BBList)
                {
                    if (BB["BBid"] == BBOfInst->getName())
                    {
                        auto InstSet = BB["Instructions"];
                        for (auto I : InstSet)
                        {
                            if (I["Instructionid"] == Inst_id)
                            {
                                auto PTSet = I["PointsToSet"];
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
    }
    else if (!reader["ContextSensitivePointsToInfo"].empty())
    {
    }
    return result;
}
/**
 * @brief Returns All may Points-to pairs at a given instruction.
 * 
 * @param Inst Instruction at which points-to info is queried.
 * @return std::vector<std::pair<std::string, std::string>> All may Points-to pairs.
 */
std::vector<std::pair<std::string, std::string>> PTDump::PTReader::getMayPointsToPairsAt(llvm::Instruction *Inst)
{

    std::vector<std::pair<std::string, std::string>> result;
    llvm::BasicBlock *BBOfInst = Inst->getParent();
    llvm::Function *FunctionOfInst = Inst->getFunction();

    int Inst_id;
    int Inst_Count = 1;
    for (llvm::Instruction &I : *BBOfInst)
    {
        if (&I == Inst)
        {
            Inst_id = Inst_Count;
            break;
        }
        Inst_Count++;
    }

    if (!reader["FlowInsensitivePointsToInfo"].empty())
    {
    }
    else if (!reader["FlowSensitivePointsToInfo"].empty())
    {
        auto ProcList = reader["FlowSensitivePointsToInfo"]["Procedure"];
        for (auto proc : ProcList)
        {
            if (proc["Functionid"] == FunctionOfInst->getName())
            {
                auto BBList = proc["BasicBlocks"];
                for (auto BB : BBList)
                {
                    if (BB["BBid"] == BBOfInst->getName())
                    {
                        auto InstSet = BB["Instructions"];
                        for (auto I : InstSet)
                        {
                            if (I["Instructionid"] == Inst_id)
                            {
                                auto PTSet = I["PointsToSet"];
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
    }
    else if (!reader["ContextSensitivePointsToInfo"].empty())
    {
    }
    return result;
}
/**
 * @brief Prints the points-to graph at a given instruction.
 * 
 * @param Inst The Instruction at which the points-to graph needs to be printed.
 */
void PTDump::PTReader::printToDot(llvm::Instruction *Inst)
{
    std::vector<std::pair<std::string, std::string>> MayPointsToSet, MustPointsToSet;
    MayPointsToSet = getMayPointsToPairsAt(Inst);
    MustPointsToSet = getMustPointsToPairsAt(Inst);

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
    ss << *Inst;
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
    fstr << "}\n";
    fstr.close();
}
/**
 * @brief Determines whether a given pointee is a may-pointee or not.
 * 
 * @param Inst Instruction at which the information is needed.
 * @param Pointer Pointer for which may-pointee needs to be checked,
 * @param Pointee Pointee to be checked whether is may-pointee or not.
 * @return true 
 * @return false 
 */
bool PTDump::PTReader::isMayPointee(llvm::Instruction *Inst, llvm::Value *Pointer, llvm::Value *Pointee)
{
    std::vector<std::pair<std::string, std::string>> MayPointsToSet = getMayPointsToPairsAt(Inst);
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
 * @brief Determines whether a given pointee is a must pointee or not.
 * 
 * @param Inst Instruction at which the information is needed.
 * @param Pointer Pointer for which must-pointee needs to be checked,
 * @param Pointee Pointee to be checked whether is must-pointee or not.
 * @return true 
 * @return false 
 */
bool PTDump::PTReader::isMustPointee(llvm::Instruction *Inst, llvm::Value *Pointer, llvm::Value *Pointee)
{
    std::vector<std::pair<std::string, std::string>> MustPointsToSet = getMustPointsToPairsAt(Inst);
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
 * @brief Prints the enitre points-to information in a result.log file.
 * 
 */
void PTDump::PTReader::printPointsToDump()
{
    std::ofstream fstr;
    fstr.open("Result.log");
    if (!reader["FlowInsensitivePointsToInfo"].empty())
    {
    }
    else if (!reader["FlowSensitivePointsToInfo"].empty())
    {
        auto ProcList = reader["FlowSensitivePointsToInfo"]["Procedure"];
        for (auto proc : ProcList)
        {
            fstr << "Procedure:\t" << proc["Functionid"] << "\n";
            for (auto BB : proc["BasicBlocks"])
            {
                fstr << "Basic Block:\t" << BB["BBid"] << "\n";
                fstr << "In[" << BB["BBid"] << "]: {";
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
                fstr << "Out[" << BB["BBid"] << "]: {";
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
    else if (!reader["ContextSensitivePointsToInfo"].empty())
    {
    }
    fstr.close();
}
/**
 * @brief Finds all the Pointees of a given pointer at a given instruction.
 * 
 * @param Inst Instruction at which the information is required.
 * @param Pointer Pointer for which all the Pointees are needed.
 * @return std::vector<std::string> 
 */
std::vector<std::string> PTDump::PTReader::getPointeesOf(llvm::Instruction* Inst, llvm::Value* Pointer)
{
    std::vector<std::string> result;
    llvm::BasicBlock* BBOfInst = Inst->getParent();
    llvm::Function* FunctionOfInst = Inst->getFunction();

    int Inst_id;
    int Inst_Count = 1;
    for (llvm::Instruction &I : *BBOfInst)
    {
        if (&I == Inst)
        {
            Inst_id = Inst_Count;
            break;
        }
        Inst_Count++;
    }

    if (!reader["FlowInsensitivePointsToInfo"].empty())
    {
    }
    else if (!reader["FlowSensitivePointsToInfo"].empty())
    {
        auto ProcList = reader["FlowSensitivePointsToInfo"]["Procedure"];
        for(auto proc : ProcList)
        {
            if(proc["Functionid"] == FunctionOfInst->getName().str())
            {
                for(auto BB : proc["BasicBlocks"])
                {
                    if(BB["BBid"] == BBOfInst->getName())
                    {
                        for(auto I : BB["Instructions"])
                        {
                            if(I["Instructionid"] == Inst_id)
                            {
                                auto PTSet = I["PointsToSet"];
                                for(auto ptr : PTSet)
                                {
                                    if(ptr["name"] == Pointer->getName().str())
                                    {
                                        for(auto pointees : ptr["PointeeSet"])
                                        {
                                            result.push_back(pointees["name"]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (!reader["ContextSensitivePointsToInfo"].empty())
    {
    }
    return result;
}