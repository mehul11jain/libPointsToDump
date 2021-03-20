# libPointsToDump
[![Build Status](https://travis-ci.com/mehul11jain/libPointsToDump.svg?token=uaLKCyzaK4p9XPrt7zpP&branch=main)](https://travis-ci.com/mehul11jain/libPointsToDump)
[![CodeFactor](https://www.codefactor.io/repository/github/mehul11jain/libpointstodump/badge)](https://www.codefactor.io/repository/github/mehul11jain/libpointstodump)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
- Created a standard format to dump Points-to-Analysis in the specified format.
- Provides API to any Analysis writer to dump the information in the standard format created.
- Can export the information using the API provided in the library back in LLVM and use the information for any purpose.
- This library also provides a test-suite using which one can analyze their soundness and precision score of their implementation.
- Standard format makes use of [JSON](https://www.json.org/json-en.html) for representing the Points-to information.
- Sample .pt dump can be accessed [here](./sample.pt.json).
- We use [this](https://github.com/nlohmann/json) library to serialize/deserialize the input.

## Installation
-  First clone this repoisorty.
   ```
   git clone https://github.com/mehul11jain/libPointsToDump.git
   ```
-  Building the project
   - Make a folder build inside the cloned repository.
   ```
   mkdir build
   cd build
   ```
   - Run the following commands.
   ```
   cmake ..
   sudo make install  
   ```
## Using the library
-  To use this library we need working implementation of Pointer-analysis in llvm compiler tool-chain.
-  If the project is build using cmake genetated build files. Following statements need to be added to the CMakeLists.txt where the target is being generated.
   ```
   target_link_libraries(<TARGET_NAME>  PointsToDump)
   ```
-  We need to add the following lines of code to the outermost CMakeLists.txt
   ```
   add_library(PointsToDump SHARED IMPORTED)
   set_target_properties(PointsToDump PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libPointsToDump.so")
   ```


#### Public members

- To specify the type of pointer-pointee relation we have PointeeType defined:
   ```
      enum PointeeType{
        MustPointee,
        MayPointee
      } 
    ```
- Analysis can be classified into following types in our standard format:
  ```
    enum AnalysisType{
       FlowInSensitive,
       FlowSensitive,
       ContextSensitive
    }
  ```
  
### API Functionalities
* #### Functions to Create the standard .pt dump.
     - `bool addPointsTo(llvm::Value*, llvm::Value*, PointeeType)`
        - To add must Point-to information to the pt dump we must provide `PointeeType::MustPointee` as third argument.
        - To add may Point-to information to the pt dump we must provide `PointeeType::MayPointee` as third argument.
        - Returns 1 if success.     
     - `bool PointsToInfoAt(llvm::Instruction*)`
       - To specify the instruction for which points-to relation is being defined. 
       - Returns 1 if success.
     - `bool AddProcdureInfo(llvm::Function*)`
       - To specify the Function in which the instruction is present. 
       - Returns 1 if success.
     - `bool AddBasicBlockInfo(llvm::BasicBlock*)`
       - To specify the Basic Block of current function we are specifying points-to information for. 
       - Returns 1 if success.
     - `bool isValidPTFormat(...)`
       - To Check whether the .pt file created is according to the correct syntax.
       - Returns 1 if the .pt file is according to correct syntax.

- #### Functions to query the information from the .pt dump.
     - `std::vector<std::pair<std::string,std::string>> getMustPointsPairsAt(llvm::Instruction*)`
       - Finds all the Must Points-to pairs from the .pt dump for the given instruction.
       - Returns the vector of pointer-pointee pair.
     - `std::vector<std::pair<std::string,std::string>> getMayPointsPairsAt(llvm::Instruction*)`
       - Finds all the Must Points-to pairs from the .pt dump for the given instruction.
       - Returns the vector of pointer-pointee pair.
     - `void PrintPointsToDump()`
       - Prints the points-to information in a file.
     - `void printToDot(llvm::Instruction*)`
       - Prints the points-to information in form of a dot file to visualize it as points-to graph.
     - `bool isMustPointee(llvm::Instruction, llvm::Value*, llvm::Value*)`
       - Return 1 if the specified pointer-pointee pair has a Must relationship at the given instruction.
     - `bool isMayPointee(llvm::Instruction, llvm::Value*, llvm::Value*)`
       - Return 1 if the specified pointer-pointee pair has a May relationship at the given instruction. 
     - `std::vector<std::string> getPointeeOf(llvm::Instruction, llvm::Value*)`
       - Return a vector of all the pointees of a given pointer at the specified instruction.    
