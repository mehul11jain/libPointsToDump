# libPointsToDump

- Created a standard format to dump Points-to-Analysis in the specified format.
- Provides API to any Analysis writer to dump the information in the standard format created.
- Can export the information using the API provided in the library back in LLVM and use the information for any purpose.
- This library also provides a test-suite using which one can analyze their soundness and precision score of their implementation.
- Standard format makes use of [JSON](https://www.json.org/json-en.html) for representing the Points-to information.
- Sample .pt dump can be accessed [here](./sample.pt.json).
- We use [this](https://github.com/nlohmann/json) library to serialize/deserialize the input.

#### Public members

```
 enum PointeeType{
    MustPointee,
    MayPointee,
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
     - `std::vector<std::pair<llvm::Value*,llvm::Value*>> getMustPointsPairsAt(llvm::Instruction*)`
       - Finds all the Must Points-to pairs from the .pt dump for the given instruction.
       - Returns the vector of pointer-pointee pair.
     - `std::vector<std::pair<llvm::Value*,llvm::Value*>> getMayPointsPairsAt(llvm::Instruction*)`
       - Finds all the Must Points-to pairs from the .pt dump for the given instruction.
       - Returns the vector of pointer-pointee pair.
     - `void PrintPointsToDump(...)`
       - Prints the points-to information in a file.
     - `void printToDot(...)`
       - Prints the points-to information in form of a dot file to visualize it as points-to graph.
     - `bool isMustPointee(llvm::Instruction, llvm::Value*, llvm::Value*)`
       - Return 1 if the specified pointer-pointee pair has a Must relationship at the given instruction.
     - `bool isMayPointee(llvm::Instruction, llvm::Value*, llvm::Value*)`
       - Return 1 if the specified pointer-pointee pair has a May relationship at the given instruction. 
     - `std::vector<llvm::Value*> getpointeeOf(llvm::Instruction, llvm::Value*)`
       - Return a vector of all the pointees of a given pointer at the specified instruction.
    
- #### Functions to evaluate the Points-to Analysis implmentation for soundness and Precision.
