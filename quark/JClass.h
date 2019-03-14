#pragma once
#include <unordered_map>
#include <bitset>
#include <vector>
#include <optional>

const char STATIC_METHOD = 0x01;
const char INSTANCE_METHOD = 0x02;
const char STATIC_PROPERTIES = 0x04;
const char METHOD_PROPERTIES = 0x08;

class JClass {
public:
	JClass(std::unordered_map<std::bitset<4>, std::vector<std::string>> m) : methods(m)
	{}
private:
	std::unordered_map<std::bitset<4>, std::vector<std::string>> methods;
	std::optional<unsigned long long> getType(char type, const std::string name) {
		if (type > 0x08) {
			return {};
		}
		std::vector<std::string> temp = methods[type];
		auto res = std::find(temp.begin(), temp.end(), &name);
		if (res != temp.end())
		{
			return type * std::distance(temp.begin(), res);
		}
		return {};
	}
};