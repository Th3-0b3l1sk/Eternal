#include "MsgAccount.h"

namespace Eternal
{
    namespace Msg
    {
        MsgAccount::MsgAccount(uint8_t* buffer, size_t len)
            : NetMsg(buffer, len), 
            _info((Info*)this->_buffer)
        { }

    }
}

