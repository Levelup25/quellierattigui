/**
 * @file ServiceException.cpp
 * @author Philippe-Henri Gosselin
 * @date 9 décembre 2015
 * @copyright CNRS
 */

#include "ServiceException.h"
using namespace server;
using namespace std;

ServiceException::ServiceException(HttpStatus status, string msg)
    : httpStatus(status), msg(msg) {}

HttpStatus ServiceException::status() const {
  return httpStatus;
}

const char* ServiceException::what() const {
  return msg.c_str();
}
