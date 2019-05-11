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

void ArrayDescriptor::pop(int index){
    ArrayDescriptor * desc = dynamic_cast<ArrayDescriptor *>(this);

    if(desc->value.intArr.empty()) {
        std::vector<std::string> tempArr = desc->value.stringArr;
        tempArr.pop_back();
        desc->value.stringArr = tempArr;
        //desc->value.stringArr.erase(index);
    }
    else if(desc->value.stringArr.empty()) {
        std::vector<int> tempArr = desc->value.intArr;
        tempArr.pop_back();
        desc->value.intArr = tempArr;
    }
        //desc->value.intArr.erase(index)
}

void ArrayDescriptor::push(int index) {
    ArrayDescriptor * desc = dynamic_cast<ArrayDescriptor *>(this);

    std::vector<int> tempArr = desc->value.intArr;
    tempArr.push_back(index);

    for (int i = 0; i < tempArr.size(); i++) {
        std::cout << "Prining temparr elements" << tempArr[i] << std::endl;
    }
    desc->value.intArr = tempArr;

}

void ArrayDescriptor::push(std::string index) {
    ArrayDescriptor * desc = dynamic_cast<ArrayDescriptor *>(this);

    std::vector<std::string> tempArr = desc->value.stringArr;
    tempArr.push_back(index);
    for (int i = 0; i < tempArr.size(); i++) {
        std::cout << "Prining temparr elements" << tempArr[i] << std::endl;
    }
    desc->value.stringArr = tempArr;
        //desc->value.stringArr.erase(index);

}

