# MacNut Style Guide

This style guide is maintained for readability and consistency within the
codebase. Contribution is a tacit agreement to adhere to these rules. Rules are
automated where possible, and denoted as such in the following guide.

NOTE: This style guide was taken from another project I (@mward25) was working on.
Some parts may not be relevant.

<!--
/// clang-format off
-->

## Table of Contents

<!--
/// clang-format on
-->

<!-- TOC start (generated with https://github.com/derlin/bitdowntoc) -->

- [Unless otherwise specified below, follow the Google C++ Style Guide](#unless-otherwise-specified-below-follow-the-google-c-style-guide)
- [Code Syntax](#code-syntax)
  - [Includes and Namespace](#includes-and-namespace)
  - [Namespaces](#namespaces)
  - [Classes, Functions, Variables, and Macros](#classes-functions-variables-and-macros)
  - [Enums](#enums)
  - [For Loops](#for-loops)
  - [Variable Naming](#variable-naming)
  - [Comments](#comments)
    - [Documentation and Explanation](#documentation-and-explanation)
    - [TODOs and FIXMEs](#todos-and-fixmes)
  - [X-Macros](#x-macros)
    - [X-Macro Definition Files](#x-macro-definition-files)
    - [X-Macro Generation Files](#x-macro-generation-files)
    - [X-Macro Implementation Files](#x-macro-implementation-files)
    - [Plain Header Files](#plain-header-files)
    - [Plain C++ Files](#plain-c-files)
    - [File Naming](#file-naming)
    - [X-Macro Macro Naming](#x-macro-macro-naming)
    - [FOREACH (X-Macro) Calls](#foreach-x-macro-calls)

<!-- TOC end -->

---

### Unless otherwise specified below, follow [the Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

---

### Code Syntax

#### Includes and Namespace

```cpp
/// Use pragma once instead of header guards
#pragma once

/// Sort includes alphabetically and prioritize <headers> over "headers"
/// **AUTOMATED**
#include <string>
#include <thing>
#include "Actor.hpp"
#include "Reactor.hpp"

/// Multiple include blocks are permitted for organization at the coder's
/// discretion
#include <iomanip>
#include <vector>
#include "Zoologist.hpp"

/// Namespace:
/// using namespace is not allowed. If a namespace is too long, simply rename it
/// in the current scope:
namespace bOpt = boost::program_options;
/// DO NOT DO THIS: "using namespace boost::program_options;"
```

#### Namespaces

All classes must be organized into appropriate namespaces using lowercase,
colon-separated naming. Namespaces follow the application structure:
`macnut::<domain>::<subdomain>`

**In Header Files (.hpp):** Wrap class definitions in namespace blocks

```cpp
#pragma once

#include <QObject>

namespace macnut::db {

class DatabaseManager : public QObject {
    /// ... class definition
};

}  // namespace macnut::db

namespace macnut::db::model {

class SongsBySetlist : public QSqlQueryModel {
    /// ... class definition
};

}  // namespace macnut::db::model
```

**In Implementation Files (.cpp):** Wrap class implementation in namespace.

```cpp
#include "SongsBySetlist.hpp"

namespace macnut::db::model {

SongsBySetlist::SongsBySetlist(QObject* parent)
    : QSqlQueryModel(parent) {
    /// DatabaseManager is accessible via parent namespace
    DatabaseManager::instance()->getDatabase();
}

}  // namespace macnut::db::model
```

**Guidelines:**

- Always use full namespace qualification in headers (no `using namespace` at
  file scope)
- Use `using namespace` in .cpp files for parent namespaces to avoid redundant
  qualification
- Organize by functionality: `macnut::db`, `macnut::ui`, `macnut::core`,
  etc.
- Subdivide large modules: `macnut::db::model`, `macnut::db::query`, etc.
- Close namespace blocks with comments indicating the full namespace path

#### Classes, Functions, Variables, and Macros

```cpp
/// const variables are in SCREAMING_SNAKE_CASE
const int BIG_NUM = 1000;
/// Macros are also in SCREAMING_SNAKE_CASE unless the macro references something.
/// Macro parameters are in SCREAMING_SNAKE_CASE with a postfixed '_'. If a parameter references an object, and the macro includes a screming clause before the object, then the object is written verbosely.
#define USELESS_MACRO_PRINT(USELESS_)                                                        \
    std::cout << #USELESS_ << "is USELESS_, you probably shouldn't use " << #USELESS_ << ".\n" \
              << "If you must use " << #USELESS_ << " do so very carefully" << std::endl;
/// In this case, STR_TO is the screaming element, and cout is written verbosely as it refers to std::cout
#define PRINT_cout(STR_TO_cout_) std::cout << #STR_TO_cout_ << std::endl;
/// Notice how here, INPUT does not reference anything, so it is not lower cased.
#define PRINT_cout(INPUT_) std::cout << #INPUT_ << std::endl;

/// If the macro references something, then the reference is written verbosely:
#define STRING_TestClass "TestClass"

/// Classes in PascalCase
class TestClass {
    /// Always include these 3 sections in this order, even if they aren't being
    /// used
private:
protected:
public:
    /// Functions in camelCase
    virtual void test() = 0;
    /// Use 4 space indentation ** AUTOMATED **/

    /// Destructors must be virtual unless the class is final. See
    /// https://www.geeksforgeeks.org/c-final-specifier/ for
    /// information about the "final" keyword.
    virtual ~TestClass() {}
};

class MoreTest : public TestClass{
private:
    std::string memberVariable;

protected:
public:
    /// IMPORTANT: Always use the override operator when you want overriding.
    virtual void test() override {
        /// Variables are in camelCase.
        std::string testStr = "";
        /// Any number over 9999 shall have the literal separators as shown
        /// below
        /// ** AUTOMATED **/
        int theAlmostLongNumber = 9850;
        int theLongNumber       = 1'000'000;
        /// When referencing member variables within a class, always use this->
        this->memberVariable = "value";
    };

    /// Setters: Parameter name should match the member variable name
    void setMemberVariable(const std::string& memberVariable) {
        this->memberVariable = memberVariable;
    }

     virtual ~MoreTest() {}
}
```

#### Enums

```cpp
/**
 * Enums shall be defined in header files
 */
enum class ActorType {  /// Definitions shall be in PascalCase
    /// Main TYPES  /// Categories are permitted
    PLAYER,         /// values shall be in SCREAMING_SNAKE_CASE
    ENEMY,
    /// Auxiliary Types
    SLIME
};

/**
 * All enum's shall be enum classes (where possible)
 * See advantages and uses here: https://www.geeksforgeeks.org/enum-classes-in-c-and-their-advantage-over-enum-datatype/
 */
enum class FavoriteType {
    /// Main TYPES
    PLAYER,
    ENEMY,
    /// Auxiliary Types
    SLIME
};

Alternatively, fancier macro classes (such as FANCY_ENUM from this project) can be utilized if such features are desired.
```

#### For Loops

```cpp
/// For Loops:
/// All kinds of for loops are allowed, if you use a for each loop, do so like
/// this:
for (ClassName _className : Container) {
    /// Do Stuff
};

/// Also, using auto is only allowed for for loops with iterators:
/// (even here, auto is discouraged).
for (auto it = container.begin(); it != container.end(); it++) {
    /// Do Stuff
};

/// **Iterator increment**: Prefer post-increment `it++` over pre-increment in for loops.
```

#### Variable Naming

**Hard rule**: Single-letter and two-letter variable names are prohibited except
in the following specific cases:

- Integer loop counters: `i`, `j`, `k`
- Iterator variables: `it`, `jt`, `kt`
- Template type parameters: `T`, `V`, `P`
- Mathematical functions where variable name truly does not mean anything (I.e.,
  sqrt(n), or dot(a, b)).
- All other variables must have descriptive, full names (e.g., `name`,
  `selection`, `count`, `string`).

**Rationale**: Abbreviations like `n`, `nm`, `slctn`, `cnt` sacrifice
readability and create cognitive overhead. The codebase should use full words
consistently.

#### Comments

##### Documentation and Explanation

```cpp
/// This type of comment is for actual commentary, note there are three slashes
// Use this kind of comment to comment out code, note there are two slashes

/** For Multiline Comments: */
/** Use this for actual commentary, note there are two asterisks */

/*
std::string tmp{
    "this kind of comment is used for commenting out code\n"
    "Note, only one asterisks."};
*/
```

##### TODOs and FIXMEs

`TODO`s and `FIXME`s are meant to be temporary, they shall not be present in any
code in a pull request. If any file that would be checked _must_ otherwise
contain TODOs, add it to your TODO-checker exclude list.

```cpp
/// @TODO An intended fix/change (I would like this to happen)
/// @FIXME This is the source of a major problem (This code blows the program to smithereens, remedy and fix)
```

#### X-Macros

The codebase uses X-Macros (FOREACH-style macros) extensively for code
generation. This section documents the conventions.

##### X-Macro Definition Files

**Extension:** `FooDef.hpp`

**Contents:** The X-macro definition that enumerates all entries

**Format:**

```cpp
/**
 * X(animal, sound)
 */
#define FOREACH_Animal(X) \
  X(dog, woof)           \
  X(cat, meow)           \
  X(bird, chirp)
```

**Example:**

```cpp
// AnimalDef.hpp
#define FOREACH_Animal(X) \
  X(dog, woof)           \
  X(cat, meow)           \
  X(bird, chirp)
```

##### X-Macro Generation Files

**Extension:** `FooGen.hpp`

**Contents:** Header declaration generation macros

```cpp
/// Declaration for one column
#define PUT_ANIMAL_HEADER(NAME_, SAYS_)                          \
    void NAME_();                                                \
    Q_INVOCABLE std::string get_##NAME_##_asString() const {     \
        return #SAYS_;                                           \
    }

/// Implementation for one column
#define PUT_ANIMAL_IMPL(NAME_, SAYS_)                            \
    void NAME_() { std::cout << #NAME_ << std::endl; }           \
    std::string get_##NAME_##_asString() const { return #SAYS_; }
```

##### X-Macro Implementation Files

**Extension:** `FooGen.impl.hpp`

**Contents:** Implementation generation macros

```cpp
#define PUT_ANIMAL_IMPL(NAME_, SAYS_)                            \
    namespace macnut::db::animal {                             \
    std::string NAME_::getName() const { return #NAME_; }        \
    std::string NAME_::getSays() const { return #SAYS_; }        \
    } /* namespace macnut::db::animal */
```

##### Plain Header Files

**Extension:** `Foo.hpp`

**Contents:** Header the end user includes. Generates class definitions from
Definition and Gen macros.

```cpp
#pragma once

#include "AnimalDef.hpp"
#include "AnimalGen.hpp"

FOREACH_Animal(PUT_ANIMAL_HEADER);
```

##### Plain C++ Files

**Extension:** `Foo.cpp`

**Contents:** Instantiates implementation macros for classes generated by the
header.

```cpp
#include "Animal.hpp"
#include "AnimalGen.impl.hpp"

FOREACH_Animal(PUT_ANIMAL_IMPL);
```

##### File Naming

| File Type           | Pattern           | Purpose                            |
| ------------------- | ----------------- | ---------------------------------- |
| Definition          | `FooDef.hpp`      | X-macro enumerators                |
| Definition (cpp)    | `FooDef.cpp`      | Blank (build system)               |
| Generation (header) | `FooGen.hpp`      | Header generation macros           |
| Generation (impl)   | `FooGen.impl.hpp` | Implementation generation macros   |
| Generation (cpp)    | `FooGen.cpp`      | Blank (build system)               |
| Plain header        | `Foo.hpp`         | Instantiates header macros         |
| Plain cpp           | `Foo.cpp`         | Instantiates implementation macros |

**Examples:**

- `Animal.hpp` / `Animal.cpp` / `AnimalGen.hpp` / `AnimalGen.impl.hpp` /
  `AnimalDef.hpp`
- `Fruit.hpp` / `Fruit.cpp` / `FruitGen.hpp` / `FruitGen.impl.hpp` /
  `FruitDef.hpp`
- `Vehicle.hpp` / `Vehicle.cpp` / `VehicleGen.hpp` / `VehicleGen.impl.hpp` /
  `VehicleDef.hpp`

##### X-Macro Macro Naming

**Convention**: X-macro macros come in two forms:

- `PUT_FOO` = callback - passed as the function argument to `FOREACH`
- `PUT_FOO_IMPL` = generates a complete function implementation

**Examples:**

- `PUT_ANIMAL_HEADER` / `PUT_ANIMAL_IMPL`
- `PUT_FRUIT_HEADER` / `PUT_FRUIT_IMPL`
- `PUT_VEHICLE_HEADER` / `PUT_VEHICLE_IMPL`

##### FOREACH (X-Macro) Calls

When a FOREACH-style macro is used as a **statement** (i.e. not inside an
expression or parameter list), it must be followed by a semicolon. Without it,
clang-format cannot tell where the macro call ends and treats the next line as
its loop body, indenting it by one level.

```cpp
/// CORRECT - semicolon signals end of statement; next line formats at same indent
FOREACH_column_(PUT_EXTRACT_FROM_MAP);
return this->insertRowWithValues(FOREACH_column_(PUT_PASS_VALUE) 0);

/// WRONG - clang-format indents `return` as if it were the foreach body
FOREACH_column_(PUT_EXTRACT_FROM_MAP)
    return this->insertRowWithValues(FOREACH_column_(PUT_PASS_VALUE) 0);
```

Inside an expression or argument list no semicolon is needed:

```cpp
/// CORRECT - FOREACH is part of an expression here, not a statement
[&]<bool has_name = (FOREACH_column_(PUT_HAS_NAME_MEMBER) false)>() { ... };
```
