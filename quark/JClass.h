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
	JClass(std::string name, std::unordered_map<std::bitset<4>, std::vector<std::string>> m) : className(name), methods(m)
	{};
	void addStaticMethod(std::vector<std::string> m);
	void removeStaticMethod(std::vector<std::string> m);

	void addInstanceMethod(std::vector<std::string> m);
	void removeInstanceMethod(std::vector<std::string> m);

	void addStaticProperty(std::vector<std::string> m);
	void removeStaticProperty(std::vector<std::string> m);

	void addMethodProperty(std::vector<std::string> m);
	void removeMethodProperty(std::vector<std::string> m);

private:
	std::string className;
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
	void addItem(const char type, const std::string name);
	void removeItem(const char type, const std::string name);
};