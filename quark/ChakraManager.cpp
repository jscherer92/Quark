#include "ChakraManager.h"

std::string ChakraManager::RunTestScript() {
	JsValueRef result;
	//example script, eventually we will replace this with what is input from the user
	std::wstring script = L"(() => { return \'Hello Quarks!\';})()";
	//run our example script
	JsRunScript(script.c_str(), currentSourceContext++, L"", &result);

	//convert return to string even if string
	JsValueRef resultJSString;
	JsConvertValueToString(result, &resultJSString);

	//project script back to C++
	const wchar_t *resultWC;
	size_t stringLen;
	JsStringToPointer(resultJSString, &resultWC, &stringLen);

	std::wstring resultW(resultWC);
	return std::string(resultW.begin(), resultW.end());
}