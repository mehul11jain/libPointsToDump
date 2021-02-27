# libPointsToDump

- Created a standard format to dump Points-to-Analysis in the specified format.
- Provides API to any Analysis writer to dump the information in the standard format created.
- Can export the information using the API provided in the library back in LLVM and use the information for any purpose.
- This library also provides a test-suite using which one can analyze their soundness and precision score of their implementation.
- Standard format makes use of [JSON]() for representing the Points-to information.

### API Functionalities
- 
