#include "PTWriter.h"

/**
 * @brief Construct a new PTDump::PTWriter::PTWriter object.
 * \par Default Constructor for creating a PTWriter object.
 * \par Analysis type is FlowInsensitive by default.
 * \par fileName is named as 'default' by default.
 */
PTDump::PTWriter::PTWriter()
{
    Atype = FlowInsensitive;
    fileName = "default";
    writer["FlowInsensitivePointsToInfo"] = json::object();
    writer["FlowSensitivePointsToInfo"] = json::object();
    writer["ContextSensitivePointsToInfo"] = json::object();
}
/**
 * @brief Construct a new PTDump::PTWriter::PTWriter object
 * 
 * @param type Defines the Analysis Type(Flow-Sensitive/Flow-Insensitive)
 * @param path Absolute path to the location to write the JSON file.
 * @param File represents the name of the JSON file to be written.
 */
PTDump::PTWriter::PTWriter(PTDump::AnalysisType type, std::string File, std::string path)
{
    Atype = type;
    fileName = File;
    Output_File_Path = path;
    writer["FlowInsensitivePointsToInfo"] = json::object();
    writer["FlowSensitivePointsToInfo"] = json::object();
    writer["ContextSensitivePointsToInfo"] = json::object();
}
/**
 * @brief Destroy the PTDump::PTWriter::PTWriter object
 * 
 */
PTDump::PTWriter::~PTWriter()
{
    
}