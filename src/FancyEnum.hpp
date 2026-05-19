#pragma once

#include <array>
#include <compare>
#include <optional>
#include <string>

#define MNUT_PUT_ENUM_PART(enumName, ...) enumName __VA_ARGS__,

#define MNUT_PUT_INTERNAL_ENUM(EnumType, FOREACH_ENUM) \
    enum class InternalEnum : EnumType { FOREACH_ENUM(PUT_ENUM_PART) };

#define MNUT_PUT_STATIC_STR_FUNC_PART(enumName, ...) \
    case InternalEnum::enumName:                \
        return str_##enumName;
#define MNUT_PUT_STATIC_STR_FUNC_PART_STRS(enumName, ...) static const char* str_##enumName = #enumName;

#define MNUT_PUT_STATIC_STR_FUNC(EnumName, EnumType, FOREACH_ENUM)            \
    static constexpr std::string_view str(EnumName val) {                \
        FOREACH_ENUM(PUT_STATIC_STR_FUNC_PART_STRS) switch (val.value) { \
            FOREACH_ENUM(PUT_STATIC_STR_FUNC_PART)                       \
            default:                                                     \
                return "";                                               \
        }                                                                \
    }

#define MNUT_PUT_STATIC_FROM_STR_FUNC_PART(enumName, ...) \
    if (inputStr == std::string_view(#enumName)) {   \
        return FancyEnumClass::enumName();           \
    }

#define MNUT_PUT_STATIC_FROM_STR_FUNC(EnumName, FOREACH_ENUM)                             \
    static constexpr std::optional<EnumName> from(const std::string_view inputStr) { \
        FOREACH_ENUM(PUT_STATIC_FROM_STR_FUNC_PART)                                  \
        return std::optional<EnumName>();                                            \
    }

#define MNUT_PUT_ENUM_FUNCS_PART(enumName, ...) \
    static constexpr FancyEnumClass enumName() { return FancyEnumClass(InternalEnum::enumName); }

#define MNUT_PUT_ENUM_SIZE_PART(enumName, ...) 1 +

#define MNUT_PUT_ENUM_SIZE(EnumName, EnumType, FOREACH_ENUM) \
    static constexpr EnumType SIZE = FOREACH_ENUM(PUT_ENUM_SIZE_PART) 0;

#define MNUT_PUT_ENUM_VALUES_PART(enumName, ...) FancyEnumClass::enumName(),

#define MNUT_PUT_ENUM_VALUES(EnumName, EnumType, FOREACH_ENUM)                                \
    static constexpr std::array<EnumName, EnumName::SIZE> values() {                     \
        return std::array<EnumName, EnumName::SIZE>{FOREACH_ENUM(PUT_ENUM_VALUES_PART)}; \
    }

/**
 * Creates an enum class implementation with conversion and string methods.
 *
 * For each enum value defined in the FOREACH_ENUM macro, the EnumName class
 * exposes a static constexpr function that returns an instance of that enum.
 * Usage: EnumName::VALUE() (e.g., EffectType::NONE())
 *
 * The EnumName class exposes:
 *   - raw(): Returns the underlying EnumType value
 *   - str(): Returns string representation of the enum
 *   - from(string_view): Parses string to enum (returns optional)
 *   - Values: Returns array containing all enum instances
 *
 * Example:
 *   auto allValues = EffectType::values();  // std::array<EffectType, N>
 */
#define MNUT_PUT_FANCY_ENUM(EnumName, EnumType, FOREACH_ENUM)                                        \
    struct EnumName {                                                                           \
        private:                                                                                \
            using FancyEnumClass = EnumName;                                                    \
            MNUT_PUT_INTERNAL_ENUM(EnumType, FOREACH_ENUM)                                           \
            InternalEnum value;                                                                 \
            constexpr EnumName(const InternalEnum& value) : value(value) {}                     \
                                                                                                \
        public:                                                                                 \
            /** Constructors (Default) */                                                       \
            constexpr EnumName(const EnumName&)            = default;                           \
            constexpr EnumName(EnumName&&)                 = default;                           \
            constexpr EnumName& operator=(const EnumName&) = default;                           \
            constexpr EnumName& operator=(EnumName&&)      = default;                           \
                                                                                                \
            /** Default Operators */                                                            \
            constexpr bool operator==(const EnumName&) const                  = default;        \
            constexpr std::strong_ordering operator<=>(const EnumName&) const = default;        \
            using Type                                                        = EnumType;       \
            constexpr explicit operator Type() const { return static_cast<Type>(this->value); } \
            constexpr operator InternalEnum() const {                                           \
                return static_cast<InternalEnum>(this->value);                                  \
            }                                                                                   \
            /** .raw returns underying EnumType */                                              \
            static constexpr EnumType raw(EnumName input) { return static_cast<Type>(input); }  \
            constexpr EnumType raw() const { return EnumName::raw(*this); }                     \
            /** SIZE is how many enums there are, not the biggest enum */                       \
            MNUT_PUT_ENUM_SIZE(EnumName, EnumType, FOREACH_ENUM)                                     \
            MNUT_PUT_STATIC_STR_FUNC(EnumName, EnumType, FOREACH_ENUM)                               \
            constexpr std::string_view str() const { return EnumName::str(*this); }             \
            MNUT_PUT_STATIC_FROM_STR_FUNC(EnumName, FOREACH_ENUM)                                    \
            FOREACH_ENUM(PUT_ENUM_FUNCS_PART)                                                   \
            MNUT_PUT_ENUM_VALUES(EnumName, EnumType, FOREACH_ENUM)                                   \
    };                                                                                          \
    template <>                                                                                 \
    struct std::hash<EnumName> {                                                                \
            std::size_t operator()(const EnumName& s) const noexcept {                          \
                return std::hash<EnumType>{}(s.raw());                                          \
            }                                                                                   \
    };
