#include "PTWriter.h"

bool PTDump::PTWriter::addPointsTo(llvm::Value* Pointer, llvm::Value* Pointee, PointeeType type)
{
    // To implement    
    return 1;
}
void PTDump::PTWriter::WriteToJson(std::string Filename){
    std::string fname = Filename + ".pt.json";
    std::ofstream out(fname);
    out << writer << std::endl;

}