
#ifndef timer_h
#define timer_h
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
// Created by emir kantul
/**
    Usefull elapsed time calculation tool between two curly bracket. Uses construction and destruction points to calculate total time.
 
    message = message to be shown on elapsed time results.
    show_destruction_message = true to show message, false otherwise.
    N = for operations that oprated many times. N is the number of repetitions.
 */
namespace timer
{
    class Timer
    {
    public:
        Timer( const std::string& message = "timer", bool show_destruction_message = true, long N = 1) :
        m_message(message),
        m_show_destruction_message(show_destruction_message),
        m_t_construct(std::chrono::high_resolution_clock::now()), N(N)
        {
        }
        long N;
        ~Timer()
        {
            if (m_show_destruction_message)
            {
                 std::string elapsed_time_message = get_elapsed_time_as_string();
                std::cout << m_message << ": " << elapsed_time_message << std::endl;
            }
        }

        std::string get_elapsed_time_as_string()
        {
             auto t_elapsed_in_nanosec = get_elapsed_time_in_nanoseconds();

            std::ostringstream sstream;

            sstream << t_elapsed_in_nanosec << " Nanoseconds";
            
            return sstream.str();
        }

        long get_elapsed_time_in_nanoseconds()
        {
            auto t_now = std::chrono::high_resolution_clock::now();
            return (std::chrono::duration_cast<std::chrono::nanoseconds>(t_now - m_t_construct).count()) / N;
        }


    private:
         std::string m_message;
         bool m_show_destruction_message;
         std::chrono::high_resolution_clock::time_point m_t_construct;
    };
}

#endif /* timer_h */
