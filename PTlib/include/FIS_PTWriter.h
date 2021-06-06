#ifndef _FIS_PT_WRITER_
#define _FIS_PT_WRITER_
#include "PTWriter.h"
namespace PTDump
{
    /**
     * @brief Provides infrastructure to specify points-to information for Flow-Insensitive Pointer Analysis.
     * 
     */
    class FIS_PTWriter : public PTWriter
    {
        std::map<const llvm::Value*,std::set<const llvm::Value*>> PointsToGraph;
        public:
            FIS_PTWriter();
            FIS_PTWriter(AnalysisType type, std::string File, std::string path = "./");
            ~FIS_PTWriter();
            void addEdge(const llvm::Value*,const llvm::Value*);
            const llvm::Function* findFunction(const llvm::Value*);
            void WriteToJson();
            bool isValidPTFormat();    
    };
}
#endif /* _FIS_PT_WRITER_ */
