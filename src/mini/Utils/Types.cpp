#include "mini/Utils/Types.hpp"

//#include "mini/Resources/ResourceManager.hpp"
#include "mini/Box/Array.hpp"

namespace mini
{
    template<typename T>
    chars_t StringOfType<T>::Get()
    {
        //if constexpr(std::is_same_v<T, res::HostResources>) return "ResourceManager";

        return "no type info";
    }
    //template chars_t StringOfType<mini::res::HostResources>::Get();
    //! issue: no template types work good but types with templates we would need to type all out ...


}//ns