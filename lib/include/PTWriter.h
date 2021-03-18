#ifndef PT_WRITER_
#define PT_WRITER_
#include <iostream>
#include <fstream>
#include <stack>
#include <nlohmann/json.hpp>
#include <iomanip>
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

using json = nlohmann::json;

namespace PTDump
{
    /**
     * @brief Specifies the Type of Analysis for which we are storing the points-to information for.
     * 
     */
    enum AnalysisType
    {
        FlowInsensitive,
        FlowSensitive,
        ContextSensitive
    };
    /**
     * @brief Specifies the Type of Pointee i.e Either may/must.
     * 
     */
    enum PointeeType
    {
        MayPointee,
        MustPointee
    };
    /**
     * @brief A class to Dump the points-to information in the standard .pt format.
     * 
     */
    class PTWriter
    {
        json writer;
        AnalysisType Atype;
        std::string fileName;
        std::stack<llvm::BasicBlock*> latestBB;
        std::stack<llvm::Function*> latestFunction;  
        std::stack<llvm::Instruction*> latestInst;        

    public:
        PTWriter();
        ~PTWriter();        
        PTWriter(AnalysisType, std::string);        
        bool addPointsTo(const llvm::Value *,const llvm::Value *, PointeeType);
        bool PointsToInfoAt(llvm::Instruction *);
        bool AddPointsToinfoAt(llvm::Function*, llvm::BasicBlock*, llvm::Instruction*, const llvm::Value*, const llvm::Value*, PointeeType);
        bool AddProcedureInfo(llvm::Function *);
        bool AddBasicBlockInfo(llvm::BasicBlock *);
        bool isValidPTFormat(json);
        void WriteToJson(std::string);
    };
};
#endif