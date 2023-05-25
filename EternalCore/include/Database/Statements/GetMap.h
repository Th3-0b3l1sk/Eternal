#pragma once
#include "IStatement.h"
namespace Eternal
{
    namespace Database
    {
        class GetMap : public IStatement
        {
        public:
#pragma pack(push, 1)
            struct Info
            {
                uint16_t _uid;
                uint16_t _doc_id;
                uint32_t _type;
                uint32_t _owner_id;
                uint32_t _weather;
                uint16_t _portal_x;
                uint16_t _portal_y;
                uint32_t _reborn_map;
                int32_t  _reborn_portal;
                uint32_t _light;
            };
#pragma pack(pop)

        public:
            GetMap();
            GetMap(uint16_t doc_id);
            virtual ~GetMap() = default;
            virtual SQLRETURN bind() override;
            virtual std::vector<std::unique_ptr<uint8_t[]>> fetch() override;
            virtual void hook_stmt(std::string& stmt) override;
        }; 
    }
}
