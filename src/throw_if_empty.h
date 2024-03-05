#ifndef CHESS_THROW_IF_EMPTY_H
#define CHESS_THROW_IF_EMPTY_H

#include <optional>
#include <utility> // For std::forward.

// Source: https://indii.org/blog/is-type-instantiation-of-template/
// NOTE: `Template` cannot have non-type template parameters.
template <typename Type, template <typename...> typename Template>
inline constexpr bool is_instance_of{ false };

template <template <typename...> typename Template, typename... Params>
inline constexpr bool is_instance_of<Template<Params...>, Template>{ true };

/// This function is a more explicit wrapper around `std::optional<T>::value`.
/// This makes it clear that an exception will be thrown if you attempt to get
/// a value out of an empty optional object.
template <typename OptionalRef>
auto&& throw_if_empty(OptionalRef&& optional) {
	// Check that `Optional` is actually `std::optional<T>` for some `T`.
	using Optional = std::remove_cv_t<std::remove_reference_t<OptionalRef>>;
	static_assert(is_instance_of<Optional, std::optional>);
	return std::forward<OptionalRef>(optional).value(); // NOLINT
}

#endif
