#pragma once
#include <fstream>
#include <mutex>
#include <string>

class Logfile {
public:
    Logfile(const std::string& filename);
    ~Logfile();
    void write(const std::string& message);

private:
    std::ofstream m_file;
    std::mutex m_mutex;
};