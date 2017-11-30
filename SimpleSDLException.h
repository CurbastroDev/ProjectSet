#ifndef SIMPLESDLEXCEPTION_H
#define SIMPLESDLEXCEPTION_H

#include <exception>
#include <string>

class SimpleSDLException: public std::exception {
    public:
        SimpleSDLException();
        SimpleSDLException(const std::string&);
        virtual ~SimpleSDLException() throw();
        virtual const char* what() const throw();
    private:
        std::string msg;
};

#endif // SIMPLESDLEXCEPTION_H
