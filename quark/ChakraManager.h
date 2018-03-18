#include <string>
#include "ChakraCore.h"

class ChakraManager {
public:
	ChakraManager() {
		JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);
		JsCreateContext(runtime, &context);

		//setup the current context
		JsSetCurrentContext(context);
		currentSourceContext = 0;
	}
	~ChakraManager() {
		//Dispose of the runtime, eventually we will make this a managed resource under the RAII guidelines for C++14
		JsSetCurrentContext(JS_INVALID_REFERENCE);
		JsDisposeRuntime(runtime);
	}
	std::string RunTestScript();
private:
	JsRuntimeHandle runtime;
	JsContextRef context;
	unsigned currentSourceContext;
};