#include <iostream>
#include <exception>

class glfw_error : public std::runtime_error {
private:
    const char* m_Message;

public:
    glfw_error(const char* message) : m_Message(message), std::runtime_error("glfw_error: " + std::string(message)) {};
    

    virtual char const* what () const throw() {
        std::string text = "glfw_error: " + std::string(m_Message);
        return text.c_str();
    };
};