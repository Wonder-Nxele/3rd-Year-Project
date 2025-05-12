#pragma once
#include <iostream>
#include <string>
#include <memory>

template<typename T>
class FileLoader {
private:
    std::unique_ptr<T> object;

public:
    FileLoader() : object(std::make_unique<T>()) {}

    // Member function to load the file into the internal object
    bool load(T& target, const std::string& file_name) {
        if (!target.openFromFile(file_name)) {
            std::cerr << "Error loading file: " << file_name << std::endl;
            return false;
        }
    }

    // Static template function to load any file into any compatible type
    template<typename U>
    static bool loadFile(U& target, const std::string& file_name) {
        if (!target.loadFromFile(file_name)) {
            std::cerr << "Error loading file: " << file_name << std::endl;
            return false;
        }
        return true;
    }
};
