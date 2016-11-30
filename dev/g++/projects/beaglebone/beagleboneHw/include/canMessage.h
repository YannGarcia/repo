/* CAN message description
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
#if !defined(__CAN_MESSAGE_H__)
#define __CAN_MESSAGE_H__

#include <vector>

namespace beagleboneHw {

  namespace can {

    class canMessage { 
	public:
	  enum CANFormat : unsigned char {
	    CANStandard = 0x00,
	    CANExtended = 0x01,
	    CANAny = 0x02
	  };

	  enum CANType : unsigned char {
	    CANData   = 0x00,
	    CANRemote = 0x01
	  };
	  
	private:
	  /** 29 bit identifier
	   */
      unsigned int _id;
	  /** Data field
	   */
      std::vector<unsigned char> _data;
	  /** 0 - STANDARD, 1- EXTENDED IDENTIFIER
	   */
      CANFormat _format;
	  /** 0 - DATA FRAME, 1 - REMOTE FRAME  
	   */
      CANType _type;
	  
    public:
      /** Constructor.
       */
      canMessage(const unsigned int p_id, const std::vector<unsigned char> & p_data, const CANFormat p_format, const CANType p_type) : _data(0x00, 8) {
	    _id = p_id;
		_data.assign(p_data, 8);
		_format = p_format;
		_type = p_type;
      };
      /** Constructor.
       */
      canMessage(const unsigned int p_id, const CANFormat p_format) : _data(0x00, 8) {
	    _id = p_id;
		_format = p_format;
		_type = CANRemote;
      };
      /** Destructor
       * If managed, the /CS pin is set to 1 before to release it
       */
      virtual ~canMessage() { _data.clear(); };
	  
    }; // End of class canMessage

  } // End of namespace can

} // End of namespace beagleboneHw

using namespace beagleboneHw::can;

#endif // __CAN_MESSAGE_H__
