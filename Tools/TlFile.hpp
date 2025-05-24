#ifndef TLFILE_HPP
#define TLFILE_HPP

#include "../Storage.hpp"

using namespace std;

namespace TlFile {
    fstream openFile(const string& filePath);
}

#endif
