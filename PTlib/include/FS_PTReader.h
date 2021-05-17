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
            std::vector<std::pair<std::string, std::string>> getMustPointsToPairsAt(int LineNo, std::string Fname);
            std::vector<std::pair<std::string, std::string>> getMayPointsToPairsAt(int LineNo, std::string Fname);
            void printToDot(int LineNo, std::string Fname);
            bool isMustPointee(int LineNo, std::string Fname, llvm::Value*, llvm::Value*);
            bool isMayPointee(int LineNo, std::string Fname, llvm::Value*, llvm::Value*);
            std::vector<std::string> getPointeesOf(int LineNo, std::string Fname, llvm::Value*);
            void printPointsToDump();
    };
}

#endif /* FS_PT_READER_ */
