#include <vector>

/*
 * Simple function interface to define how a function works in each OSs JavaScript runtime
 * In the case of Windows, this means taking the IDispatch interface and replicating that
 * In the case of OSX, <define interface type>
*/
class Function {
public:
	/*
	* Different datatypes are needed for functions. In JavaScript we really only have a few types.
	* Number, String, Boolean, Array, Function, Object, Buffer
	* By mapping these to a datatype, we can hold these in a flat vector for easy an easy check
	*/
	enum DataType {
		Int      = 0,
		LInt     = 1,
		Float    = 2,
		Double   = 3,
		String   = 4,
		Func     = 5,
		Array    = 6,
		Buffer   = 7
	};
	/*
	* Going to make Returns from functions act like Unix systems. Instead of throwing the error here,
	* we are going to map them to numbers that we can then have the implementor decide on how to handle them.
	*/
	enum ReturnTypes {
		Ok		        = 0,
		InternalFailure = 1,
		UnkownFailure   = 2,
		NotEnoughArgs   = 3,
		WrongArgs       = 4,
		UserFailure     = 5
	};

	/*
	* Implementation of this function should actually run the function and deal with what needs to be done at that time
	*/
	virtual ReturnTypes run() = 0;
	/*
	* Some functions may need to wait a certain amount of time before they need to actually be run.
	*/
	virtual void wait(int) = 0;
	/*
	* I/O and others may need to be run on a separate thread, this should be implemented in such cases. If an actual implementation of
	* function does not need this, then a simple NotImplemented error should be thrown since it should not be used.
	*/
	virtual void runOnThread() = 0;
private:
	int numArgs;
	std::vector<DataType> args;
	DataType returnValue;
};