# hetero-vector
Lightweight C++ heterogeneous container.

Design idea:
- Provide a base class `ValueBoxBase` for users to inherit
- User implements box wrapper, override virtual function
- No global type lookup table, no manual type registration
- Two styles: `header-only (.hpp)` / separate `.h + .cpp`

C++ version requirement: C++17

## Quick start
### Option 1: header-only
```cpp
#include "hetero_vector/hetero_vector.hpp"
```

### Option 2: separate h + cpp

```
#include "hetero_vector/hetero_vector.h"
```

Need link `hetero_vector.cpp` when compile.

## Build

```
cmake -B build
cmake --build build
./build/demo
```

## Features

- Store different types in one vector
- Type check at runtime by `std::type_index`
- Support container copy (clone virtual method)
- Easy to extend: inherit `ValueBoxBase`

## Example

See `example/main.cpp`

## License

MIT
