#pragma once
#include "com/box/StringMap.hpp"
#include "res/Prefab/PrefabEnum.hpp"
#include "dbg/Assert.hpp"
#include "com/box/String.hpp"
#include "com/box/Array.hpp"
#include "com/utils/Utils.hpp"

namespace rpg::res {

enum class ComponentMemberEnum
{
    Children,
    Mesh,
    Material, 
    Scale,
    ENUM_END
};

const com::EnumMap<ComponentMemberEnum::ENUM_END, com::String<res::PREFAB_FILE_LINE_LEN_MAX>> COMPONENT_DATA_ENUM_TO_STR
{
    { ComponentMemberEnum::Children,  "Children"      },
    { ComponentMemberEnum::Mesh,      "Mesh"          },
    { ComponentMemberEnum::Scale,     "Scale"         },
    { ComponentMemberEnum::Material,  "Material"      },
};
const auto COMPONENT_DATA_STR_TO_ENUM = com::StringMapFromEnumMap<ComponentMemberEnum, res::PREFAB_FILE_LINE_LEN_MAX>(COMPONENT_DATA_ENUM_TO_STR);


struct ComponentMemberPair
{
    com::String<res::PREFAB_FILE_LINE_LEN_MAX> key;
    com::String<res::PREFAB_FILE_LINE_LEN_MAX> val;

    auto get_data() const
    {
        auto const componentDataEnum = res::COMPONENT_DATA_STR_TO_ENUM.GetOptional(key.Data());
        dbg::Assert(componentDataEnum, "no componentDataEnum");
        struct { 
            char const * const key_cstr;
            char const * const val_cstr;
            ComponentMemberEnum componentDataEnum;
        } rtn = { .key_cstr = key.Data(), .val_cstr = val.Data(), .componentDataEnum = *componentDataEnum };
        return rtn;
    }
};


constexpr auto COMPONENT_DATA_PAIR_MAX = 10;
using ComponentMemberPairs =  com::Array<ComponentMemberPair, COMPONENT_DATA_PAIR_MAX>;

inline auto LineStrToComponentDataPair(chars_t line) -> ComponentMemberPair const 
{
    ComponentMemberPair pair {};

    auto len = com::StrLen(line);

    idx_t idx = 0;
    while(line[idx++] != '=')
        dbg::Assert(line[idx] != 0, "invalid component data pair");

    pair.key.Append(&line[0]  , idx - 1);
    pair.val.Append(&line[idx], len - idx);

    return pair;
}

//multiple values in one line to array (end of line must be null terminated)
template<auto VAL_COUNT, auto VAL_LEN>
auto ValStrToValArray(chars_t valStr)
{
    com::Array<com::String<VAL_LEN>, VAL_COUNT> arr;
    
    idx_t beginIdx = 0;
    idx_t endIdx   = 0;

    while(valStr[endIdx] != '\0') 
    {
        if (valStr[endIdx] == ',') {
            arr.AppendElement(&valStr[beginIdx], endIdx - beginIdx);
            beginIdx = endIdx + 1;
        }
        ++endIdx;
    }
    arr.AppendElement(&valStr[beginIdx], endIdx - beginIdx); //last one

    return arr;
}

} //ns