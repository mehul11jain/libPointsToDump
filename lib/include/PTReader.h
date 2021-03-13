#ifndef _PT_READER_
#define _PT_READER_
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
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
    class PTReader
    {
        json reader;
        std::string fileName;
        std::string PathToFile;
        public:
            PTReader(std::string);
            PTReader(std::string, std::string);
            void init();
            ~PTReader();
            std::vector<std::pair<std::string, std::string>> getMustPointsToPairsAt(llvm::Instruction*);
            std::vector<std::pair<std::string, std::string>> getMayPointsToPairsAt(llvm::Instruction*);
            void printPointsToDump();
            void printToDot(llvm::Instruction*);
            bool isMustPointee(llvm::Instruction*, llvm::Value*, llvm::Value*);
            bool isMayPointee(llvm::Instruction*, llvm::Value*, llvm::Value*);
            std::vector<std::string> getPointeesOf(llvm::Instruction*, llvm::Value*);
    };

}// PTDump namespace ends.
#endif // !_PT_READER