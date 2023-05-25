#include "Entities/item.h"

namespace Eternal
{
    namespace Entities
    {
        Item::Item()
            : _id{}, _type{}, _owner_id{}, _player_id{},
            _durability{}, _durability_limit{}
            ,_position{ POS_INVALID_POS }, 
            _gem1{}, _gem2{}, _magic1{}, _magic2{}, _magic3{}, _restrian{}, _bless{},
            _enchant{}, _locked{}, _color{}
        {
            
        }

        Item::Item(Database::GetUserItems::Info* data)
            : Item()
        {
            _id               = data->id;
            _type             = data->type;
            _owner_id         = data->owner_id;
            _player_id        = data->player_id;
            _durability       = data->dura;
            _durability_limit = data->dura_limit;
            _ident            = data->ident;
            _position = data->position;
            _gem1     = data->gem1;
            _gem2     = data->gem2;
            _magic1   = data->magic1;
            _magic2   = data->magic2;
            _magic3   = data->magic3;
            _restrian = data->restrain;
            _bless    = data->bless;
            _enchant  = data->enchant;
            _locked   = data->locked;
            _color    = data->color;
        }
    }
}
