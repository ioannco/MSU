//
// Created by Иван Черемисенов on 14.02.2023.
//

#include <iostream>
#include <cmath>

class Vector_3d {
 public:
	Vector_3d() {
		m_x = m_y = m_z = 0.;
	}

	Vector_3d(double x, double y, double z) {
		m_x = x;
		m_y = y;
		m_z = z;
	}

	Vector_3d(double num) {
		m_x = num;
		m_y = m_z = 0.;
	}

	void print() const {
		std::cout << '(' << m_x << ',' << m_y << ',' << m_z << ')' << std::endl;
	}

	double euc_norm() const {
		return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
	}

	double max_norm() const {
		return std::max(std::max(abs(m_x), abs(m_y)), abs(m_z));
	}

	double get_x() const {
		return m_x;
	}

	double get_y() const {
		return m_y;
	}

	double get_z() const {
		return m_z;
	}

	void set(double x, double y, double z) {
		m_x = x;
		m_y = y;
		m_z = z;
	}

 private:
	double m_x, m_y, m_z;
};
