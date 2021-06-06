#ifndef _FIS_PT_READER_
#define _FIS_PT_READER_
#include "PTReader.h"
namespace PTDump
{
    /**
     * @brief Provides functions to query the information from the JSON file for Flow-Insensitive Analysis.
     * 
     */
    class FIS_PTReader : public PTReader
    {
        public:
            FIS_PTReader(std::string);
            FIS_PTReader(std::string, std::string);
            ~FIS_PTReader();
            int sizeOfPtSet(const llvm::Value*);
            void printToDot();
            void printPointsToDump();
            void printAllPts(const llvm::Value*);
            bool isPointee(const llvm::Value*, const llvm::Value*);
    };
};
#endif /* _FIS_PT_READER_ */
