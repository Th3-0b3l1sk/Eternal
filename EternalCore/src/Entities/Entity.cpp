#include "Entities/Entity.h"

namespace Eternal
{
    namespace Entities
    {
        void Entity::clear_bc_set()
        {
            std::unique_lock lock(_view_set.first);
            auto& view_set = _view_set.second;
            for (auto entity = view_set.begin(); entity != view_set.end(); ) {
                entity->second->remove_from_bc_set(shared_from_this());
                auto tmp = entity++;
                view_set.erase(tmp);
                
                // TODO: check if the entity is a player then send ACTION_LEAVE_MAP (me leaving)
            }
        }

        void Entity::remove_from_bc_set(std::shared_ptr<Entity> entity)
        {
            std::unique_lock lock(_view_set.first);
            auto& view_set = _view_set.second;
            auto it = view_set.find(entity->get_id());
            if (it != view_set.end())
                view_set.erase(it);

            // TODO: check if the entity is a player then send ACTION_LEAVE_MAP (him leaving)
        }

        void Entity::add_to_bc_set(std::shared_ptr<Entity> entity)
        {
            std::unique_lock lock(_view_set.first);
            auto& view_set = _view_set.second;
            if (view_set.find(entity->get_id()) != view_set.end())
                return;

            view_set[entity->get_id()] = entity;

            // TODO: check if player
            entity->inform(shared_from_this());
        }
    }
}
