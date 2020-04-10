#include "MiniSTL/Profiler.hpp"
#include "MiniSTL/Array.hpp"

namespace mini::profile {

    mini::container::Array<double, 1024>* history = nullptr;

    void AllocateProfileHistory()
    {
        history = new mini::container::Array<double, 1024>; //todo: unique ptr
    }

    void PrintProfileHistory()
    {
        FOR_ARRAY((*history), i) {
            std::cout << (*history)[i] << "\n";
        }
    }

    template<Mode MODE>
    void ScopeProfile<MODE>::StoreProfileData(const double ms)
    {
        history->EmplaceBack(ms);
    }
    template void ScopeProfile<Mode::Print>::StoreProfileData(const double);
    template void ScopeProfile<Mode::Store>::StoreProfileData(const double);

}