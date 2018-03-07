//
// Created by kobus on 2016/02/11.
//

#pragma once


#include <stdexcept>

#define Except(a) Exception(a, __FILE__, __func__, __LINE__)

class Exception : public std::exception
{
public:
    /** Constructor (C strings).
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the \c char* lies
     *                 with the caller.
     */
    explicit Exception(const char* message, const char * file, const char* func, int line):
            msg_(message)
    {
        msg_.append("\r\nException from - ");
        msg_.append(file);
        msg_.append("\r\n");
        msg_.append(func);
        msg_.append(" : ");
        msg_.append(std::to_string(line));
        msg_.append("\r\n");
    }

    /** Constructor (C++ STL strings).
     *  @param message The error message.
     */
    explicit Exception(const std::string& message, const char * file, const char* func, int line):
            msg_(message)
    {
        msg_.append("\r\nException from - ");
        msg_.append(file);
        msg_.append("\r\n");
        msg_.append(func);
        msg_.append(" : ");
        msg_.append(std::to_string(line));
        msg_.append("\r\n");
    }

    /** Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~Exception() throw (){}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a \c const \c char*. The underlying memory
     *          is in posession of the \c Exception object. Callers \a must
     *          not attempt to free the memory.
     */
    virtual const char* what() const throw (){
        return msg_.c_str();
    }

protected:
    /** Error message.
     */
    std::string msg_;
};


