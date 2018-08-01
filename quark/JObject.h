#pragma once
#include <vector>
#include <string>

class JObject {
private:
	std::vector<std::string> staticMethods = std::vector<std::string>();
	std::vector<std::string> instanceMethods = std::vector<std::string>();
	std::vector<std::string> staticProperties = std::vector<std::string>();
	std::vector<std::string> instanceProperties = std::vector<std::string>();
public:
	JObject() { }
	virtual bool setStaticMethods() = 0;
	virtual bool setInstanceMethods() = 0;
	virtual bool setStaticProperties() = 0;
	virtual bool setInstanceProperties() = 0;
	std::vector<std::string> getStaticMethods() {
		return staticMethods;
	}
	std::vector<std::string> getInstanceMethods() {
		return instanceMethods;
	}
	std::vector<std::string> getStaticProperties() {
		return staticProperties;
	}
	std::vector<std::string> getInstanceProperties() {
		return instanceProperties;
	}
protected:
	virtual bool executeMethod(std::string method) = 0;
};