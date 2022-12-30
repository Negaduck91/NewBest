#include <ctime>
#include <fstream>
#include <mutex>
#include <string>

class Logfile
{
public:
    static Logfile& getInstance(){
        static Logfile instance;
        return instance;
    }

    void write(const std::string& message){
        
        std::lock_guard<std::mutex> lock(m_mutex);

        // Get the current time
        std::time_t t = std::time(nullptr);
        char timeStr[64];
        std::strftime(timeStr, sizeof timeStr, "%Y-%m-%d %H:%M:%S", std::localtime(&t));

        m_ofstream << timeStr << ": " << message << std::endl;
    }

private:
    Logfile()
        : m_ofstream("log.txt",  std::ios::out | std::ios::app){}

    ~Logfile(){
        m_ofstream.close();
    }

    Logfile(const Logfile&) = delete;
    Logfile& operator=(const Logfile&) = delete;

    std::ofstream m_ofstream;
    std::mutex m_mutex;
};