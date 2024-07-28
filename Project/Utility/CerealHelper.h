#pragma once

#include "Utility/CerealExtention.h"

// シリアライズ毎回書くのめんどいのでマクロ化
// ヘルパーマクロ
#define EXPAND(x) x
#define GET_NTH_ARG(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, N, ...) N
#define MACRO_CHOOSER(...) EXPAND(GET_NTH_ARG(__VA_ARGS__, MACRO10, MACRO9, MACRO8, MACRO7, MACRO6, MACRO5, MACRO4, MACRO3, MACRO2, MACRO1, ))

// 各数の引数に対応するマクロ（例外処理付き）
#define MACRO1(arg1) try { archive(NVP(arg1)); } catch (const std::exception&) {}
#define MACRO2(arg1, arg2) MACRO1(arg1) try { archive(NVP(arg2)); } catch (const std::exception&) {}
#define MACRO3(arg1, arg2, arg3) MACRO2(arg1, arg2) try { archive(NVP(arg3)); } catch (const std::exception&) {}
#define MACRO4(arg1, arg2, arg3, arg4) MACRO3(arg1, arg2, arg3) try { archive(NVP(arg4)); } catch (const std::exception&) {}
#define MACRO5(arg1, arg2, arg3, arg4, arg5) MACRO4(arg1, arg2, arg3, arg4) try { archive(NVP(arg5)); } catch (const std::exception&) {}
#define MACRO6(arg1, arg2, arg3, arg4, arg5, arg6) MACRO5(arg1, arg2, arg3, arg4, arg5) try { archive(NVP(arg6)); } catch (const std::exception&) {}
#define MACRO7(arg1, arg2, arg3, arg4, arg5, arg6, arg7) MACRO6(arg1, arg2, arg3, arg4, arg5, arg6) try { archive(NVP(arg7)); } catch (const std::exception&) {}
#define MACRO8(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) MACRO7(arg1, arg2, arg3, arg4, arg5, arg6, arg7) try { archive(NVP(arg8)); } catch (const std::exception&) {}
#define MACRO9(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) MACRO8(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) try { archive(NVP(arg9)); } catch (const std::exception&) {}
#define MACRO10(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) MACRO9(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) try { archive(NVP(arg10)); } catch (const std::exception&) {}

// メインマクロ
#define SERIALIZE(...) \
friend class cereal::access; \
template<typename TArchive> \
void serialize(TArchive& archive) \
{ \
    EXPAND(MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)) \
}

// CEREAL_NVP長いのでNVPへ
#define NVP(...) CEREAL_NVP(__VA_ARGS__)
#define MAKE_NVP(name, value) cereal::make_nvp(name, value)