#include "IR/ir.hpp"
#include <iostream>

int main() {
	IR::Function f;
	f.m_name = "hi";
	std::cout << f.m_name << '\n';
}
