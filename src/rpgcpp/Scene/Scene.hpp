#include "mini/Debug/Logger.hpp"
#include "mini/Box/String.hpp"

namespace rpgcpp::scene
{
    struct Scene
    {
        mini::box::String<10> name { "Scene1" };


        inline void Update()
        {
            LOG("update ", name);
        }

        inline void Render()
        {

        }

    };

}//ns