#pragma once
#include <iostream>
#include <exception>

#define runtime_msg_assert(condition,message) if (!(condition)) { std::cout << message; throw std::runtime_error(message); }