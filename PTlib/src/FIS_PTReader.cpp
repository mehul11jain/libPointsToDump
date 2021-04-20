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

}

bool PTDump::FIS_PTReader::isPointee(const llvm::Value* Pointer, const llvm::Value* Pointee)
{
    return 1;
}