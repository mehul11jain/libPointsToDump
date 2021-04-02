#ifndef FS_PT_WRITER_
#define FS_PT_WRITER_
#include "PTWriter.h"
namespace PTDump{

    class FS_PTWriter : public PTWriter
    {
        std::stack<llvm::BasicBlock*> latestBB;
        std::stack<llvm::Function*> latestFunction;  
        std::stack<llvm::Instruction*> latestInst;

        public:
            FS_PTWriter();
            FS_PTWriter(AnalysisType type, std::string File, std::string path = "./");
            ~FS_PTWriter();
            bool addPointsTo(const llvm::Value *,const llvm::Value *, PointeeType);
            bool PointsToInfoAt(llvm::Instruction *);
            bool AddPointsToinfoAt(llvm::Function*, llvm::BasicBlock*, llvm::Instruction*, const llvm::Value*, const llvm::Value*, PointeeType);
            bool AddProcedureInfo(llvm::Function *);
            bool AddBasicBlockInfo(llvm::BasicBlock *);
            void WriteToJson(std::string);
            bool isValidPTFormat();
    };

}
#endif /* _FS_PT_WRITER_ */
