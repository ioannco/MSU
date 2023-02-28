//
// Created by Иван Черемисенов on 15.02.2023.
//

#include <iostream>
#include <cmath>

class BitString {
 public:
	BitString() {
		m_high = 0U;
		m_low = 0U;
	}

	BitString(uint32_t high, uint32_t low) {
		m_high = high;
		m_low = low;
	}

	BitString(uint32_t num) {
		m_high = 0U;
		m_low = num;
	}

	void Print() const {
		std::cout << *reinterpret_cast<const uint64_t*>(this) << std::endl;
	}

	void ShiftLeft(unsigned offset) {
		*reinterpret_cast<uint64_t*>(this) <<= offset;
	}

	void ShiftRight(unsigned offset) {
		*reinterpret_cast<uint64_t*>(this) >>= offset;
	}

	void NOT() {
		*reinterpret_cast<uint64_t*>(this) = ~*reinterpret_cast<uint64_t*>(this);
	}

	void AND(const BitString & other) {
		*reinterpret_cast<uint64_t*>(this) &= *reinterpret_cast<const uint64_t*>(&other);
	}

	void OR(const BitString & other) {
		*reinterpret_cast<uint64_t*>(this) |= *reinterpret_cast<const uint64_t*>(&other);
	}

	void XOR(const BitString & other) {
		*reinterpret_cast<uint64_t*>(this) ^= *reinterpret_cast<const uint64_t*>(&other);
	}

 private:
	uint32_t m_low;
	uint32_t m_high;
};
