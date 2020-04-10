#pragma once
#include <chrono>
#include <iostream>
//#include <iomanip>

#define PRINT_PROFILE_FUNCTION()  mini::profile::ScopeProfile<mini::profile::Mode::Print> scopeProfilePrint { __FUNCTION__ };
#define PRINT_PROFILE_SCOPE(name) mini::profile::ScopeProfile<mini::profile::Mode::Print> scopeProfilePrint { name };
#define STORE_PROFILE_FUNCTION()  mini::profile::ScopeProfile<mini::profile::Mode::Store> scopeProfileStore { __FUNCTION__ };
#define STORE_PROFILE_SCOPE(name) mini::profile::ScopeProfile<mini::profile::Mode::Store> scopeProfileStore { name };

namespace mini::profile
{
    void PrintProfileHistory();
    void AllocateProfileHistory();

    enum class Mode { Print, Store };
    template<Mode MODE> struct ScopeProfile
    {
        ScopeProfile(const char* pName = "") 
            : begin { std::chrono::high_resolution_clock::now() }
            , name  { pName }
        {;}
        ~ScopeProfile()
        { 
            const auto delta = (std::chrono::high_resolution_clock::now() - begin);
            const auto ms    = std::chrono::duration<double, std::milli>(delta);
            if constexpr (MODE == Mode::Print) std::cout << name << ": " << ms.count() << "ms\n"; //std::setprecision(8)
            if constexpr (MODE == Mode::Store) StoreProfileData(ms.count());
        }

        const std::chrono::high_resolution_clock::time_point begin;
        const char* const name;

    private:
        static void StoreProfileData(const double ms);
    };
    
}