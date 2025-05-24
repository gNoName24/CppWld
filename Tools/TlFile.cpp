#include "TlFile.hpp"

fstream TlFile::openFile(const string& filePath) {
    fstream file(filePath, std::ios::in | std::ios::out | std::ios::app);
    return file;
}
