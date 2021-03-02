# libPointsToDump

- Created a standard format to dump Points-to-Analysis in the specified format.
- Provides API to any Analysis writer to dump the information in the standard format created.
- Can export the information using the API provided in the library back in LLVM and use the information for any purpose.
- This library also provides a test-suite using which one can analyze their soundness and precision score of their implementation.
- Standard format makes use of [JSON]() for representing the Points-to information.

#### Public members

```
 enum PointeeType{
    MustPointee,
    MayPointee,
 }  
```
### API Functionalities
#### Function to Create the standard .pt dump.
- `addPointsTo(llvm::Value*, llvm::Value*, PointeeType::MustPointee)` : To add must Point-to information to the pt dump.
- `addPointsTo(llvm::Value*, llvm::Value*, PointeeType::MayPointee)` : To add may Point-to information to the pt dump.
- `PointsToInfoAt(llvm::Instruction*)`: To specify the instruction for which points-to relation is being defined.
- `AddProcdureInfo(llvm::Function*)`: To specify the Function in which the instruction is present.
- `AddBasicBlockInfo(llvm::BasicBlock*)`: To specify the Basic Block of current function we are specifying points-to information for.
