#include "FIS_PTReader.h"
PTDump::FIS_PTReader::FIS_PTReader(std::string fName) : PTDump::PTReader::PTReader(fName)
{

}
PTDump::FIS_PTReader::FIS_PTReader(std::string fName, std::string path) : PTDump::PTReader::PTReader(fName, path)
{

}
PTDump::FIS_PTReader::~FIS_PTReader()
{
}

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