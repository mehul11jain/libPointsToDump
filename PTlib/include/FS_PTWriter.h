#ifndef FS_PT_WRITER_
#define FS_PT_WRITER_
#include "PTWriter.h"
namespace PTDump{

    /**
     * @brief Provides infrastructure to specify points-to information for Flow-Sensitive Pointer Analysis.
     * 
     */
    class FS_PTWriter : public PTWriter
    {
        std::stack<llvm::Function*> latestFunction;  
        std::stack<std::string> latestFile;
        std::stack<int> latestLineNo;

        public:
            FS_PTWriter();
            FS_PTWriter(AnalysisType type, std::string File, std::string path = "./");
            ~FS_PTWriter();
            bool addPointsTo(const llvm::Value *, const llvm::Value *, PointeeType);
            bool addPointsTo(std::string Pointer, std::string Pointee, std::string PointerType, std::string TypeOfPointee , PointeeType);
            bool PointsToInfoAt(int LineNo);
            bool WritePointsToinfoAt(llvm::Function*, int lineNo, std::string FileName, const llvm::Value*, const llvm::Value*, PointeeType);
            bool WritePointsToinfoAt(llvm::Function*, int lineNo, std::string FileName, std::string Pointer, std::string Pointee, std::string PointerType, std::string TypeOfPointee, PointeeType);
            bool AddProcedureInfo(llvm::Function *);
            bool AddFileInfo(std::string);
            void WriteToJson(std::string);
            bool isValidPTFormat();
    };

}
#endif /* _FS_PT_WRITER_ */
