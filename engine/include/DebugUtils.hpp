#pragma once

#include <string>

/**
 * A class that represents any object that contains common debugging hooks (getFullDebugInfo, getDebugTitle, etc)
 */
class DebuggableObject {

public:

    /**
     * A function to get a title that represents this object in a readable way, generally containing some identifiable data within it
     * @return the title
     */
    inline virtual std::string getDebugTitle() { return "<DebuggableObject>"; };

    /**
     * A function that returns a string containing detailed debug info about the current object
     * @return the detailed info string
     */
    inline virtual std::string getFullDebugInfo() { return "DebuggableObject\n"; };
};