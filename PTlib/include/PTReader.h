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
    /**
     * @brief Base Class for creating analysis specific classes to query the generated .pt dump.
     *
     */
    class PTReader
    {
        protected:
            json reader;
            std::string fileName;
            std::string PathToFile;
        public:
            PTReader(std::string);
            PTReader(std::string, std::string);
            void init();
            ~PTReader();            
            virtual void printPointsToDump() = 0;
    };

}// PTDump namespace ends.
#endif // !_PT_READER