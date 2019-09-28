#include "file_loader.h"

#include "log.h"

#include <string>

namespace ME {
    std::string loadFile(const std::string& filepath) {
        std::string contents{""};
        std::ifstream ifs(filepath);
        if (ifs.is_open()) {
            std::string line;
            while (std::getline(ifs, line))
            {
                contents += line + "\n";
            }
            ifs.close();
        }
        else {
            Log<FATAL>() << ("Failed to open file for reading: " + filepath);
        }
        return contents;
    }
}