#pragma once
#include <utility>
#include <optional>

template <typename Mtx, typename Cont>
using guarded_pair = std::pair<Mtx, Cont>;

template<typename T>
using opt_ref = std::optional<std::reference_wrapper<T>>;
