#include "Msg/MsgData.h"

namespace Eternal
{
    namespace Msg
    {
        MsgData::MsgData(uint32_t year, uint32_t month, uint32_t day_of_year, uint32_t day,
            uint32_t hour, uint32_t minute, uint32_t second)
            : NetMsg(sizeof(Info)), 
            _info((Info*)this->_buffer.get())
        {
            _info->header.type   = MsgType::MSG_DATA;
            _info->header.length = sizeof(Info);
            _info->year          = year; // TODO: check results
            _info->month         = month;
            _info->day_of_year   = day_of_year;
            _info->day           = day;
            _info->hour          = hour;
            _info->minute        = minute;
            _info->second        = second;
        }

        void MsgData::process(Server& server, uint32_t con_id)
        {
            // TODO: impl
        }
    }
}
