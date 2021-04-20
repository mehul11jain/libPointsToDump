#ifndef PT_WRITER_
#define PT_WRITER_
#include <iostream>
#include <fstream>
#include <stack>
#include <set>
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
        protected:
            json writer;
            AnalysisType Atype;
            std::string Output_File_Path;
            std::string fileName;                

        public:
            PTWriter();
            ~PTWriter();          
            PTWriter(AnalysisType type, std::string File, std::string path);                
            virtual bool isValidPTFormat() = 0;
    };    
    
};
#endif