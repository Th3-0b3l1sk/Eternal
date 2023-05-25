#pragma once
#include <utility>

template <typename Mtx, typename Cont>
using guarded_pair = std::pair<Mtx, Cont>;
