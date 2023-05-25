#pragma once
#include "NetMsg.h"
#include "Entities/Player.h"

namespace Eternal
{
    namespace Msg
    {
        class MsgPlayer :
            public NetMsg
        {
        public:
#pragma pack(push, 1)
            struct Info
            {
                Msg::Header header;
                uint32_t unique_id;
                uint32_t look;
                union {
                    uint64_t status;
                    struct {
                        uint16_t statuary_life;
                        uint16_t statuary_frame;
                    };
                };
                union {
                    int32_t synid_rank;
                    int32_t owner_uid;
                };
                int32_t garment_type;
                int32_t armet_type;
                int32_t armor_type;
                int32_t rweapon_type;
                int32_t lweapon_type;
                int8_t _padding1[4];
                uint16_t life;
                uint16_t level;
                uint16_t pos_x;
                uint16_t pos_y;
                uint16_t hair;
                uint8_t dir;
                uint8_t pose;
                uint16_t metempsychosis;
                uint16_t player_level;
                int8_t _padding2[4];
                uint32_t nobility_rank;
                uint32_t nobility_uid;
                uint32_t nobility_pos;
                int8_t _padding3[4];
                uint16_t armet_color;
                uint16_t armor_color;
                int8_t _padding4[2];
                uint8_t strings[1];
            };

#pragma pack(pop)
        public:
            MsgPlayer(std::shared_ptr<uint8_t[]>&& data, size_t len);

            MsgPlayer(Entities::Player& player);

        public:
            virtual void process(Server& server, uint32_t con_id) override;

        private:
            Info* _info;
        };
    }
}
