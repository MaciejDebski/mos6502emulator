#pragma once


namespace mos6502emu {
	template<class DataType>
	class RingBuffer {
	public:
		using buffer_type = std::vector<DataType>;

		inline RingBuffer(unsigned int size) : Buffer(size, DataType()), Current(Buffer.begin() - 1) {};

		inline typename buffer_type::const_iterator GetCurrent() {
			return Current;
		};

		inline void resize(unsigned int size) {
			Buffer.resize(size, DataType());
		};

		inline typename buffer_type::const_iterator begin() {
			return Buffer.begin();
		};

		inline typename buffer_type::const_iterator end() {
			return Buffer.end();
		}

		const DataType& Pop() {
			auto ref = *Current;
			if (--Current == Buffer.rend()) {
				Current = std::move(Buffer.rbegin());
			}
			return ref;
		};

		void Push(const DataType& value) {
			if (++Current == Buffer.end()) {
				Current = std::move(Buffer.begin());
			}
			*Current = value;
		};

	private:
		typename buffer_type Buffer;
		typename buffer_type::iterator Current;
	};
}








