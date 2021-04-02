#ifndef FS_PT_READER_
#define FS_PT_READER_
#include "PTReader.h"

namespace PTDump
{
    class FS_PTReader : public PTReader
    {

        public:
            FS_PTReader(std::string);
            FS_PTReader(std::string, std::string);
            ~FS_PTReader();
            std::vector<std::pair<std::string, std::string>> getMustPointsToPairsAt(llvm::Instruction*);
            std::vector<std::pair<std::string, std::string>> getMayPointsToPairsAt(llvm::Instruction*);
            void printToDot(llvm::Instruction*);
            bool isMustPointee(llvm::Instruction*, llvm::Value*, llvm::Value*);
            bool isMayPointee(llvm::Instruction*, llvm::Value*, llvm::Value*);
            std::vector<std::string> getPointeesOf(llvm::Instruction*, llvm::Value*);
            void printPointsToDump();
    };
}

#endif /* FS_PT_READER_ */
