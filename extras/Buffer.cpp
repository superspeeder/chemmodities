#include "Buffer.hpp"

unsigned int formMode(VertBufTargetAction action, VertBufIOMode iomode) {
    return 0x88E0 + static_cast<int>(action) + static_cast<int>(iomode);
}