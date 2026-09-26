#pragma once

#include <algorithm>
#include <array>
#include <compare>
#include <optional>
#include <string>

#define MNUT_FIRST_ARG(FIRST_ARG, ...) FIRST_ARG

#define MNUT_PUT_ENUM_PART(ENUM_NAME_, ...) ENUM_NAME_ MNUT_FIRST_ARG(__VA_ARGS__),

#define MNUT_PUT_INTERNAL_ENUM(ENUM_TYPE_, FOREACH_ENUM_) \
    enum class InternalEnum : ENUM_TYPE_ { FOREACH_ENUM_(MNUT_PUT_ENUM_PART) };

#define MNUT_PUT_STATIC_STR_FUNC_PART(ENUM_NAME_, ...) \
    case InternalEnum::ENUM_NAME_:                     \
        return str_##ENUM_NAME_;
#define MNUT_PUT_STATIC_STR_FUNC_PART_STRS(ENUM_NAME_, ...) \
    const char* str_##ENUM_NAME_ = #ENUM_NAME_;

#define MNUT_PUT_STATIC_STR_FUNC(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_)           \
    static constexpr std::string_view str(ENUM_NAME_ VALUE_) {                    \
        FOREACH_ENUM_(MNUT_PUT_STATIC_STR_FUNC_PART_STRS) switch (VALUE_.value) { \
            FOREACH_ENUM_(MNUT_PUT_STATIC_STR_FUNC_PART)                          \
            default:                                                              \
                return "";                                                        \
        }                                                                         \
    }

#define MNUT_PUT_STATIC_FROM_STR_FUNC_PART(ENUM_NAME_, ...) \
    if (INPUT_STR_ == std::string_view(#ENUM_NAME_)) {      \
        return FancyEnumClass::ENUM_NAME_();                \
    }

#define MNUT_PUT_STATIC_FROM_STR_FUNC(ENUM_NAME_, FOREACH_ENUM_)                         \
    static constexpr std::optional<ENUM_NAME_> from(const std::string_view INPUT_STR_) { \
        FOREACH_ENUM_(MNUT_PUT_STATIC_FROM_STR_FUNC_PART)                                \
        return std::optional<ENUM_NAME_>();                                              \
    }

#define MNUT_PUT_ENUM_FUNCS_PART(ENUM_NAME_, ...)        \
    static constexpr FancyEnumClass ENUM_NAME_() {       \
        return FancyEnumClass(InternalEnum::ENUM_NAME_); \
    }

#define MNUT_PUT_ENUM_SIZE_PART(ENUM_NAME_, ...) 1 +

#define MNUT_PUT_ENUM_SIZE(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_) \
    static constexpr ENUM_TYPE_ SIZE = FOREACH_ENUM_(MNUT_PUT_ENUM_SIZE_PART) 0;

#define MNUT_PUT_ENUM_VALUES_PART(ENUM_NAME_, ...) FancyEnumClass::ENUM_NAME_(),

#define MNUT_PUT_ENUM_VALUES(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_)                                \
    static constexpr std::array<ENUM_NAME_, ENUM_NAME_::SIZE> values() {                           \
        return std::array<ENUM_NAME_, ENUM_NAME_::SIZE>{FOREACH_ENUM_(MNUT_PUT_ENUM_VALUES_PART)}; \
    }

#define MNUT_PUT_FANCY_ENUM_HASH(ENUM_NAME_, ENUM_TYPE_)                        \
    namespace std {                                                             \
    template <>                                                                 \
    struct hash<ENUM_NAME_> {                                                   \
            std::size_t operator()(const ENUM_NAME_& enumName) const noexcept { \
                return std::hash<ENUM_TYPE_>{}(enumName.raw());                 \
            }                                                                   \
    };                                                                          \
    }

/**
 * Creates an enum class implementation with conversion and string methods.
 *
 * For each enum value defined in the FOREACH_ENUM_ macro, the ENUM_NAME_ class
 * exposes a static constexpr function that returns an instance of that enum.
 * Usage: ENUM_NAME_::VALUE() (e.g., EffectType::NONE())
 *
 * The ENUM_NAME_ class exposes:
 *   - raw(): Returns the underlying ENUM_TYPE_ value
 *   - str(): Returns string representation of the enum
 *   - from(string_view): Parses string to enum (returns optional)
 *   - Values: Returns array containing all enum instances
 *
 * Example:
 *   auto allValues = EffectType::values();  // std::array<EffectType, N>
 */
#define MNUT_PUT_FANCY_ENUM_BARE(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_, ...)                       \
    struct ENUM_NAME_ {                                                                            \
        private:                                                                                   \
            using FancyEnumClass = ENUM_NAME_;                                                     \
            MNUT_PUT_INTERNAL_ENUM(ENUM_TYPE_, FOREACH_ENUM_)                                      \
            InternalEnum value;                                                                    \
            constexpr ENUM_NAME_(const InternalEnum& value) : value(value) {}                      \
                                                                                                   \
        public:                                                                                    \
            /** Constructors (Default) */                                                          \
            constexpr ENUM_NAME_(const ENUM_NAME_&)            = default;                          \
            constexpr ENUM_NAME_(ENUM_NAME_&&)                 = default;                          \
            constexpr ENUM_NAME_& operator=(const ENUM_NAME_&) = default;                          \
            constexpr ENUM_NAME_& operator=(ENUM_NAME_&&)      = default;                          \
                                                                                                   \
            /** Default Operators */                                                               \
            constexpr bool operator==(const ENUM_NAME_&) const                  = default;         \
            constexpr std::strong_ordering operator<=>(const ENUM_NAME_&) const = default;         \
            using Type                                                          = ENUM_TYPE_;      \
            constexpr explicit operator Type() const { return static_cast<Type>(this->value); }    \
            constexpr operator InternalEnum() const {                                              \
                return static_cast<InternalEnum>(this->value);                                     \
            }                                                                                      \
            /** .raw returns underlying ENUM_TYPE_ */                                              \
            static constexpr ENUM_TYPE_ raw(ENUM_NAME_ input) { return static_cast<Type>(input); } \
            constexpr ENUM_TYPE_ raw() const { return ENUM_NAME_::raw(*this); }                    \
            /** SIZE is how many enums there are, not the biggest enum */                          \
            MNUT_PUT_ENUM_SIZE(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_)                              \
            MNUT_PUT_STATIC_STR_FUNC(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_)                        \
            constexpr std::string_view str() const { return ENUM_NAME_::str(*this); }              \
            MNUT_PUT_STATIC_FROM_STR_FUNC(ENUM_NAME_, FOREACH_ENUM_)                               \
            FOREACH_ENUM_(MNUT_PUT_ENUM_FUNCS_PART)                                                \
            MNUT_PUT_ENUM_VALUES(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_)                            \
            __VA_OPT__(__VA_ARGS__(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_))                         \
    };

#define MNUT_PUT_FANCY_ENUM(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_, ...)          \
    MNUT_PUT_FANCY_ENUM_BARE(ENUM_NAME_, ENUM_TYPE_, FOREACH_ENUM_, __VA_ARGS__) \
    MNUT_PUT_FANCY_ENUM_HASH(ENUM_NAME_, ENUM_TYPE_)
