#pragma once

#include <vector>

template<class DataType>
class RingBuffer {
public:
	using buffer_type = std::vector<DataType>;

	inline RingBuffer(int size) : Buffer(size, DataType()) {};

	inline buffer_type::const_iterator Current() {
		return Current;
	};

	inline buffer_type::const_iterator begin() {
		return Buffer.begin();
	};

	inline buffer_type::const_iterator end() {
		return Buffer.end();
	}

	const DataType& Pop() {
		auto ref = *Current;
		if (--Current == Buffer.crend()) {
			Current = std::move(Buffer.crbegin());
		}
		return ref;
	};

	const void Push(DataType&& value) {
		if (++Current == Buffer.cend()) {
			Current = std::move(Buffer.cbegin());
		}
		Buffer[Current] = std::forward<DataType>(value);
	};

private:
	buffer_type Buffer;
	buffer_type::const_iterator Current;
};







