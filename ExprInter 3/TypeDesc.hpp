#include <string>
#include <iomanip>
#include <iostream>
#include <vector>

// add to makefile
struct TypeDescriptor {
    // The base class for datatype descriptors
    
    // This ENUM can expand to include additional types
    enum types {INTEGER, DOUBLE, BOOL, STRING, ARRAY};
    
    TypeDescriptor(types type): _type{type} {}
    
    types &type() { return _type; }
    
    void printValue();
    
    bool checkIfInt();
    
    bool checkIfStr();
    
    virtual ~TypeDescriptor() {}
    
    private:
        types _type;
};

struct NumberDescriptor: public TypeDescriptor {
    // Number descriptor includes integer, double, and boolean datatypes
    
    NumberDescriptor(types descType): TypeDescriptor(descType) {}
    union {
        int intValue;
        double doubleValue;
        short boolValue;
    } value;
};

struct StringDescriptor: public TypeDescriptor {
    // String descriptor uses only strings
    
    StringDescriptor(types descType): TypeDescriptor(descType) {}
    struct {
        std::string stringValue;
    } value;
};

struct ArrayDescriptor: public TypeDescriptor {
    ArrayDescriptor(types descType): TypeDescriptor(descType) {}
    struct {
        std::vector<int> intArr;
        std::vector<std::string> stringArr;
    } value;
};