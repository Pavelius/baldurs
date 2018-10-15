#pragma once

template<typename T> struct iterator {
	T*					current;
	unsigned			size;
public:
	constexpr iterator(T* current, unsigned size) : current(current), size(size) {}
	constexpr T&		operator*() const { return *current; }
	constexpr bool		operator!=(const iterator& e) const { return e.current != current; }
	constexpr void		operator++() { current = (T*)((char*)current + size); }
};
