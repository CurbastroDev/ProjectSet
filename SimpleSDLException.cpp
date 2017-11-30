#include <SDL2/SDL.h>
#include "SimpleSDLException.h"

SimpleSDLException::SimpleSDLException():
  exception(), msg(SDL_GetError()) {}

SimpleSDLException::SimpleSDLException(const std::string& m):
  exception(), msg(m) {}

SimpleSDLException::~SimpleSDLException() throw() {}

const char* SimpleSDLException::what() const throw() {
    return msg.c_str();
}
