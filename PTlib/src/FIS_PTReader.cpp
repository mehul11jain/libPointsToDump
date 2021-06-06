#include "FIS_PTReader.h"
/**
 * @brief Construct a new ptdump::FIS_PTReader::FIS_PTReader object
 * 
 * @param fName Name of the file to be queried.
 */
PTDump::FIS_PTReader::FIS_PTReader(std::string fName) : PTDump::PTReader::PTReader(fName)
{

}
/**
 * @brief Construct a new ptdump::FIS_PTReader::FIS_PTReader object
 * 
 * @param fName Name of the file to be queried.
 * @param path Path to the folder containing the file.
 */
PTDump::FIS_PTReader::FIS_PTReader(std::string fName, std::string path) : PTDump::PTReader::PTReader(fName, path)
{

}
/**
 * @brief Destroy the new ptdump::FIS_PTReader::FIS_PTReader object
 * 
 */
PTDump::FIS_PTReader::~FIS_PTReader()
{
}
/**
 * @brief Computes the size of points-to set of a pointer variable.
 * 
 * @param Pointer Pointer variable whose points-to set's size is to be computed
 * @return int The size of the points-to set of the pointer variable passed as parameter.
 */
int PTDump::FIS_PTReader::sizeOfPtSet(const llvm::Value* Pointer)
{
    int size;
    if(llvm::isa<llvm::GlobalVariable>(Pointer))
    {
        if(!Pointer->getType()->getPointerElementType()->isPointerTy()) return 0;
        else
        {
            auto PTGraph = reader["FlowInsensitivePointsToInfo"]["PointsToGraph"];
            for(auto pt : PTGraph)
            {
                if(pt["name"] == Pointer->getName().str())
                {
                    size = pt["PointsToSet"].size();
                    break;
                }
            }
        }
    }
    else
    {
        if(!Pointer->getType()->isPointerTy()) return 0;
        else
        {
            auto PTGraph = reader["FlowInsensitivePointsToInfo"]["PointsToGraph"];
            const llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction>(Pointer);
            std::string fName = I->getFunction()->getName().str();
            for(auto pt : PTGraph)
            {
                if(fName == pt["procedure"] && pt["name"] == Pointer->getName().str())
                {
                    size = pt["PointsToSet"].size();
                    break;
                }                
            }
        }
    }
    return size;
}
/**
 * @brief Prints all pointees of the specified pointer variable.
 * 
 * @param Pointer Pointer variable whose pointees are to be printed.
 */
void PTDump::FIS_PTReader::printAllPts(const llvm::Value* Pointer)
{
    
    if(llvm::isa<llvm::GlobalVariable>(Pointer))
    {
        if(!Pointer->getType()->getPointerElementType()->isPointerTy()) return ;
        else
        {
            auto PTGraph = reader["FlowInsensitivePointsToInfo"]["PointsToGraph"];
            for(auto pt : PTGraph)
            {
                if(pt["name"] == Pointer->getName().str())
                {
                    auto ptset = pt["PointsToSet"];
                    std::cout << "{ ";
                    for(auto it : ptset)
                    {
                        std::cout << it["name"] << ", ";
                    }
                    std::cout << " }\n";
                    return;
                }
            }
        }
    }
    else
    {
        if(!Pointer->getType()->isPointerTy()) return ;
        else
        {
            auto PTGraph = reader["FlowInsensitivePointsToInfo"]["PointsToGraph"];
            const llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction>(Pointer);
            std::string fName = I->getFunction()->getName().str();
            for(auto pt : PTGraph)
            {
                if(fName == pt["procedure"] && pt["name"] == Pointer->getName().str())
                {
                    auto ptset = pt["PointsToSet"];
                    std::cout << "{ ";
                    for(auto it : ptset)
                    {
                        std::cout << it["name"] << ", ";
                    }
                    std::cout << " }\n";
                }                
            }
        }
    }
}
/**
 * @brief prints the points to information available in the JSON file.
 * 
 */
void PTDump::FIS_PTReader::printPointsToDump()
{
    auto PTGraph = reader["FlowInsensitivePointsToInfo"]["PointsToGraph"];
    std::cout << "\n------------------------------------------------------ Points-To Information----------------------------------------------\n\n";
    for(auto pt : PTGraph)
    {
        std::cout << pt["name"] << ": {";
        int i = 0;
        for(auto it : pt["PointsToSet"])
        {
            if(i != 0)
            {
                std::cout <<  ", ";
            }
            std::cout << it["name"];
            i++;
        }
        std::cout << " }\n";        
    }
    std::cout << "\n-------------------------------------------------------------------------------------------------------------------------\n\n";
}
/**
 * @brief prints the points-to information(graph) in a test.dot file
 * 
 */
void PTDump::FIS_PTReader::printToDot()
{
    std::map<std::string, int> ValID;
    std::string dash = "-";
    int i = 1;
    for(auto pt : reader["FlowInsensitivePointsToInfo"]["PointsToGraph"])
    {
        std::string pt_string;
        pt_string = pt["name"];
        pt_string += "-";
        pt_string += pt["procedure"];

        if(pt["procedure"] == "" && ValID.find(pt["name"]) == ValID.end())
            ValID[pt["name"]] = i++;
        else if(pt["procedure"] != "" && ValID.find(pt_string) == ValID.end())
            ValID[pt_string] = i++;

        for(auto it : pt["PointsToSet"])
        {
            std::string it_string;
            it_string = it["name"];
            it_string += "-";
            it_string += it["procedure"];
            if(it["procedure"] == "" && ValID.find(it["name"]) == ValID.end())
                ValID[it["name"]] = i++;
            else if(it["procedure"] != "" && ValID.find(it_string) == ValID.end())
                ValID[it_string] = i++;
        }
    }
    std::ofstream fstr;
    fstr.open("test.dot");
    fstr << "digraph {\n";    
    fstr << "\trankdir=\"LR\";\n";
    fstr << "\tnode[shape=circle, width=0.5, color=lightblue2, style=filled];\n";
    for(auto it : ValID)
    {
        fstr << it.second << "[label=\"" << it.first << "\"];\n";
    }
    for(auto pt : reader["FlowInsensitivePointsToInfo"]["PointsToGraph"])
    {
        std::string pt_name;        
        pt_name = pt["name"];
        if(pt["procedure"] != "")
        {
            pt_name += "-"; 
            pt_name += pt["procedure"];
        }

        for(auto it : pt["PointsToSet"])
        {
            std::string pts_string;            
            pts_string = it["name"];
            if(it["procedure"] != "")
            {
                pts_string += "-";
                pts_string += it["procedure"];
            }

            fstr << ValID[pt_name] << "->" << ValID[pts_string] << ";\n";
        }
    }
    fstr << "}\n";
    fstr.close();
}
/**
 * @brief Checks whether the given pointer variable points to the given pointee variable.
 * 
 * @param Pointer The pointer variable,
 * @param Pointee The pointee variable.
 * @return true 
 * @return false 
 */
bool PTDump::FIS_PTReader::isPointee(const llvm::Value* Pointer, const llvm::Value* Pointee)
{
    bool is_global_pointer = llvm::isa<llvm::GlobalVariable>(Pointer);
    bool is_global_pointee = llvm::isa<llvm::GlobalVariable>(Pointee);
    if(is_global_pointer && is_global_pointee)
    {
        auto PTGraph = reader["FlowInsensitivePointsToInfo"]["PointsToGraph"];
        for(auto pt : PTGraph)
        {
            if(pt["name"] == Pointer->getName())
            {
                for(auto i : pt["PointsToSet"])
                {
                    if(i["name"] == Pointee->getName())
                    {
                        return 1;
                    }
                }
            }
        }

    }
    else if(!is_global_pointer && is_global_pointee)
    {
        const llvm::Function* fun = llvm::dyn_cast<llvm::Instruction>(Pointer)->getFunction();
        auto PTGraph = reader["FlowInsensitivePointsToInfo"]["PointsToGraph"];
        for(auto pt : PTGraph)
        {
            if(pt["name"] == Pointer->getName() && fun->getName().str() == pt["procedure"])
            {
                for(auto i : pt["PointsToSet"])
                {
                    if(i["name"] == Pointee->getName())
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else if(is_global_pointer && !is_global_pointee)
    {
        const llvm::Function* fun = llvm::dyn_cast<llvm::Instruction>(Pointee)->getFunction();
        auto PTGraph = reader["FlowInsensitivePointsToInfo"]["PointsToGraph"];
        for(auto pt : PTGraph)
        {
            if(pt["name"] == Pointer->getName())
            {
                for(auto i : pt["PointsToSet"])
                {
                    if(i["name"] == Pointee->getName() && fun->getName().str() == i["procedure"])
                    {
                        return 1;
                    }
                }
            }
        }
    }
    else
    {
        const llvm::Function* fun  = llvm::dyn_cast<llvm::Instruction>(Pointee)->getFunction();
        const llvm::Function* fun2 = llvm::dyn_cast<llvm::Instruction>(Pointer)->getFunction();
        auto PTGraph = reader["FlowInsensitivePointsToInfo"]["PointsToGraph"];
        for(auto pt : PTGraph)
        {
            if(pt["name"] == Pointer->getName() && fun2->getName().str() == pt["procedure"])
            {
                for(auto i : pt["PointsToSet"])
                {
                    if(i["name"] == Pointee->getName() && fun->getName().str() == i["procedure"])
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}