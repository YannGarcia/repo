/* Beaglebone Abstract storage description
 * Copyright (c) 2014 ygarcia, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#if !defined(__ABSTRACTSTORAGE_H__)
#define __ABSTRACTSTORAGE_H__

#include <stdexcept> // used for std::runtime_error
#include <string>
#include <vector>

#include "datum.h"

namespace beagleboneStorage {

  class abstractStorage {
  public:
    enum StorageFamily : unsigned char {
      binFile = 0x00,
      txtFile = 0x01,
      kmlFile = 0x02
    };

  protected:
    StorageFamily _family;

  public:
    abstractStorage() { };
    virtual ~abstractStorage() { };

    inline virtual const StorageFamily family() const { return _family; };
    virtual int initialize(const std::string & p_storageDevice) {  throw new std::runtime_error("Method shall be overwritten"); };
    virtual int uninitialize() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual int open() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void close() { throw new std::runtime_error("Method shall be overwritten"); };
    virtual void store(const std::vector<unsigned char> & p_data) {  throw new std::runtime_error("Method shall be overwritten"); };

  }; // End of class abstractStorage

} // End of namespace beagleboneStorage

using namespace beagleboneStorage;

#endif // __ABSTRACTSTORAGE_H__
