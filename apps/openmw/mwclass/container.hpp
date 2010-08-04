#ifndef GAME_MWCLASS_CONTAINER_H
#define GAME_MWCLASS_CONTAINER_H

#include "../mwworld/class.hpp"

namespace MWClass
{
    class Container : public MWWorld::Class
    {
        public:

            virtual std::string getName (const MWWorld::Ptr& ptr) const;
            ///< \return name (the one that is to be presented to the user; not the internal one);
            /// can return an empty string.

            virtual MWWorld::ContainerStore<MWWorld::RefData>& getContainerStore (
                const MWWorld::Ptr& ptr) const;
            ///< Return container store

            static void registerSelf();
    };
}

#endif
