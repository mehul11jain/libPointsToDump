#ifndef PT_WRITER_
#define PT_WRITER_
#include <iostream>
#include <fstream>
#include <stack>
#include <nlohmann/json.hpp>
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include <iomanip>

using json = nlohmann::json;

namespace PTDump
{
    enum AnalysisType
    {
        FlowInsensitive,
        FlowSensitive,
        ContextSensitive
    };
    enum PointeeType
    {
        MayPointee,
        MustPointee
    };

    class PTWriter
    {
        json writer;
        AnalysisType Atype;
        std::stack<llvm::BasicBlock*> latestBB;
        std::stack<llvm::Function*> latestFunction;        

    public:
        PTWriter()
        {
            Atype = FlowInsensitive;
            writer["FlowInsensitivePointsToInfo"] = json::object();
            writer["FlowSensitivePointsToInfo"] = json::object();
            writer["ContextSensitivePointsToInfo"] = json::object();
        }
        PTWriter(AnalysisType type)
        {
            Atype = type;
            writer["FlowInsensitivePointsToInfo"] = json::object();
            writer["FlowSensitivePointsToInfo"] = json::object();
            writer["ContextSensitivePointsToInfo"] = json::object();
        }
        bool addPointsTo(llvm::Value *, llvm::Value *, PointeeType);
        bool PointsToInfoAt(llvm::Instruction *);
        bool AddPointsToinfoAt(llvm::Function*, llvm::BasicBlock*, llvm::Instruction*, llvm::Value*, llvm::Value*, PointeeType);
        bool AddProcedureInfo(llvm::Function *);
        bool AddBasicBlockInfo(llvm::BasicBlock *);
        bool isValidPTFormat(json);
        void WriteToJson(std::string);
    };
};
#endif