#include "TypeDesc.hpp"

// add to makefile
//TypeDescriptor::TypeDescriptor() : _type{nullptr} {}
//TypeDescriptor::TypeDescriptor(types type) : _type{type} {}

void TypeDescriptor::printValue() {
    // If desc is of datatype NumberDescriptor, this function
    // will print the value that is stored in it. Otherwise,
    // it just returns
    
    // Use dynamic_cast to down-cast, from typedescriptor to 
    // NumberDescriptor
    NumberDescriptor *nDesc = dynamic_cast<NumberDescriptor *>(this);
    // dynamic cast will return a nullptr if 
    // desc is not of datatype NumberDescriptor
    if(nDesc == nullptr) {
        StringDescriptor *sDesc = dynamic_cast<StringDescriptor *>(this);
        if (sDesc == nullptr) {
            return;
        }
        else {
            std::cout << std::setw(10) << sDesc->value.stringValue << std::endl;
        }
    }
    else {
    // desc must have been of type NumberDescriptor
    if( nDesc->type() == TypeDescriptor::INTEGER)
        std::cout << std::setw(10) << nDesc->value.intValue << std::endl;
    else if( nDesc->type() == TypeDescriptor::DOUBLE)
        std::cout << std::setw(10) << nDesc->value.doubleValue << std::endl;
    else if( nDesc->type() == TypeDescriptor::BOOL)
        std::cout << std::setw(10) << nDesc->value.boolValue << std::endl;
    else
        std::cout << "Misconfigured union type." << std::endl;
    }
}

bool TypeDescriptor::checkIfInt() {
    NumberDescriptor *nDesc = dynamic_cast<NumberDescriptor *>(this);
    // dynamic cast will return a nullptr if 
    // desc is not of datatype NumberDescriptor
    if(nDesc == nullptr) {
            return false;
    }
    else {
        return true;
    }
    
}

bool TypeDescriptor::checkIfStr() {
    StringDescriptor *sDesc = dynamic_cast<StringDescriptor *>(this);
    if(sDesc == nullptr) {
        return false;
    }
    else {
        return true;
    }
}

