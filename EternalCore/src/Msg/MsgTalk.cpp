#include "./Msg/MsgTalk.h"
#include "Util/StringPacker.h"
#include <timeapi.h>

namespace Eternal
{
    namespace Msg
    {
        MsgTalk::MsgTalk(std::shared_ptr<uint8_t[]>&& msg, size_t len)
            : NetMsg(std::move(msg), len)
        {
            helper(0, 0);
        }

        MsgTalk::MsgTalk(std::string sender_name, std::string recipient_name, std::string suffix, std::string message, uint32_t tone, uint32_t color)
            : NetMsg(sizeof(Info) +
                sender_name.length() + recipient_name.length() +
                suffix.length()  + message.length() - UINT8_C(4 * 1))   //  4 * char[] placeholders
        {
            helper(tone, color);

            auto ptr = (Info*)_buffer.get();
            StringPacker packer(&ptr->str_count);
            packer.AddString(sender_name);
            packer.AddString(recipient_name);
            packer.AddString(suffix);
            packer.AddString(message);
        }

        void MsgTalk::helper(uint32_t tone, uint32_t color)
        {
            auto info = (Info*)_buffer.get();
            info->header.length = _size;
            info->header.type = MsgType::MSG_TALK;
            info->argb = color;
            info->tone = tone;
            info->style = 0;
            info->recipient_mesh = 0;
            info->sender_mesh = 0;
            info->identity = timeGetTime();
        }

        void MsgTalk::process(Eternal::Server& server)
        {

        }


    }
}

