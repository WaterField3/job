#pragma once

#include <cereal/types/base_class.hpp>

#include "Utility/CerealExtention.h"

// シリアライズ毎回書くのめんどいのでマクロ化
// ヘルパーマクロ
#define CEREAL_COMPONENT_EXPAND(x) x
#define CEREAL_COMPONENT_GET_NTH_ARG(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, N, ...) N
#define CEREAL_COMPONENT_MACRO_CHOOSER(...) CEREAL_COMPONENT_EXPAND(CEREAL_COMPONENT_GET_NTH_ARG(__VA_ARGS__, CEREAL_COMPONENT_MACRO10, CEREAL_COMPONENT_MACRO9, CEREAL_COMPONENT_MACRO8, CEREAL_COMPONENT_MACRO7, CEREAL_COMPONENT_MACRO6, CEREAL_COMPONENT_MACRO5, CEREAL_COMPONENT_MACRO4, CEREAL_COMPONENT_MACRO3, CEREAL_COMPONENT_MACRO2, CEREAL_COMPONENT_MACRO1, ))

// 各数の引数に対応するマクロ（例外処理付き）
#define CEREAL_COMPONENT_MACRO1(arg1) try { archive(CEREAL_COMPONENT_NVP(arg1)); } catch (const std::exception&) {}
#define CEREAL_COMPONENT_MACRO2(arg1, arg2) CEREAL_COMPONENT_MACRO1(arg1) try { archive(CEREAL_COMPONENT_NVP(arg2)); } catch (const std::exception&) {}
#define CEREAL_COMPONENT_MACRO3(arg1, arg2, arg3) CEREAL_COMPONENT_MACRO2(arg1, arg2) try { archive(CEREAL_COMPONENT_NVP(arg3)); } catch (const std::exception&) {}
#define CEREAL_COMPONENT_MACRO4(arg1, arg2, arg3, arg4) CEREAL_COMPONENT_MACRO3(arg1, arg2, arg3) try { archive(CEREAL_COMPONENT_NVP(arg4)); } catch (const std::exception&) {}
#define CEREAL_COMPONENT_MACRO5(arg1, arg2, arg3, arg4, arg5) CEREAL_COMPONENT_MACRO4(arg1, arg2, arg3, arg4) try { archive(CEREAL_COMPONENT_NVP(arg5)); } catch (const std::exception&) {}
#define CEREAL_COMPONENT_MACRO6(arg1, arg2, arg3, arg4, arg5, arg6) CEREAL_COMPONENT_MACRO5(arg1, arg2, arg3, arg4, arg5) try { archive(CEREAL_COMPONENT_NVP(arg6)); } catch (const std::exception&) {}
#define CEREAL_COMPONENT_MACRO7(arg1, arg2, arg3, arg4, arg5, arg6, arg7) CEREAL_COMPONENT_MACRO6(arg1, arg2, arg3, arg4, arg5, arg6) try { archive(CEREAL_COMPONENT_NVP(arg7)); } catch (const std::exception&) {}
#define CEREAL_COMPONENT_MACRO8(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) CEREAL_COMPONENT_MACRO7(arg1, arg2, arg3, arg4, arg5, arg6, arg7) try { archive(CEREAL_COMPONENT_NVP(arg8)); } catch (const std::exception&) {}
#define CEREAL_COMPONENT_MACRO9(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) CEREAL_COMPONENT_MACRO8(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) try { archive(CEREAL_COMPONENT_NVP(arg9)); } catch (const std::exception&) {}
#define CEREAL_COMPONENT_MACRO10(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) CEREAL_COMPONENT_MACRO9(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) try { archive(CEREAL_COMPONENT_NVP(arg10)); } catch (const std::exception&) {}

// メインマクロ
#define SERIALIZE_COMPONENT(...) \
friend class cereal::access; \
template<typename TArchive> \
void serialize(TArchive& archive) \
{ \
    archive(cereal::base_class<TMF::Component>(this)); \
    CEREAL_COMPONENT_EXPAND(CEREAL_COMPONENT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)) \
}

// CEREAL_NVP長いのでNVPへ
#define CEREAL_COMPONENT_NVP(...) CEREAL_NVP(__VA_ARGS__)
//#define MAKE_NVP(name, value) cereal::make_nvp(name, value)
