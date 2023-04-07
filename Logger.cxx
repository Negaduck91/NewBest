#include <chrono>
#include <ctime>
#include "Logger.hxx"

Logfile::Logfile(const std::string& filename)
    : m_file(filename, std::ios::out | std::ios::app)
{}

Logfile::~Logfile() {
    m_file.close();
}

void Logfile::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    m_file << std::ctime(&time) << message << std::endl;
}