#include "PTReader.h"

/**
 * @brief Construct a new PTDump::PTReader::PTReader object
 * 
 * @param fName Name of the file to be queried.
 */
PTDump::PTReader::PTReader(std::string fName)
{
    fileName = fName;
    PathToFile = "./";
    init();
}
/**
 * @brief Construct a new PTDump::PTReader::PTReader object
 * 
 * @param fName Name of the file to be queried.
 * @param path Path to the folder containing the file.
 */
PTDump::PTReader::PTReader(std::string fName, std::string path)
{
    fileName = fName;
    PathToFile = path;
    init();
}
/**
 * @brief Destroy the PTDump::PTReader::PTReader object
 * 
 */
PTDump::PTReader::~PTReader()
{
}
/**
 * @brief Initializa a JSON Object using the input JSON File.
 * 
 */
void PTDump::PTReader::init()
{
    std::ifstream input_json_file(PathToFile + fileName + ".pt.json");
    input_json_file >> reader;
}
