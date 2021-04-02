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
     * @brief Class to query the generated .pt dump.
     * \par Provides Various functions to Dump the Points-to Information, Find May/MustPointee at a given Instruction, Creates a Points-to Graph at a given instruction.
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