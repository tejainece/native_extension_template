#include <stdlib.h>
#include <string>
#include <dart_api.h>

/// Handles error
Dart_Handle handleError(Dart_Handle handle) {
	if (Dart_IsError(handle))
		Dart_PropagateError(handle);
	return handle;
}

// Native functions get their arguments in a Dart_NativeArguments structure
// and return their results with Dart_SetReturnValue.
void get5(Dart_NativeArguments arguments) {
	Dart_Handle result = handleError(Dart_NewInteger(5));
	Dart_SetReturnValue(arguments, result);
}

void add(Dart_NativeArguments arguments) {
	// Check argument count
	if (Dart_GetNativeArgumentCount(arguments) != 2) {
		Dart_ThrowException(
				Dart_NewStringFromCString("Invalid argument count!"));
		return;
	}

	// Check type
	Dart_Handle argA = handleError(Dart_GetNativeArgument(arguments, 0));
	Dart_Handle argB = handleError(Dart_GetNativeArgument(arguments, 1));
	if (!Dart_IsInteger(argA) || !Dart_IsInteger(argB)) {
		Dart_ThrowException(
				Dart_NewStringFromCString("Invalid argument type!"));
		return;
	}

	// Check if the integers fit
	{
		bool fits = true;
		bool tempFits = false;
		handleError(Dart_IntegerFitsIntoInt64(argA, &tempFits));
		fits &= tempFits;
		handleError(Dart_IntegerFitsIntoInt64(argB, &tempFits));
		fits &= tempFits;
		if (!fits) {
			Dart_ThrowException(
					Dart_NewStringFromCString("Too long integers!"));
			return;
		}
	}

	int64_t valA = 0;
	int64_t valB = 0;
	handleError(Dart_IntegerToInt64(argA, &valA));
	handleError(Dart_IntegerToInt64(argB, &valB));
	int64_t res = valA + valB;

	Dart_SetReturnValue(arguments, handleError(Dart_NewInteger(res)));
}

/// Resolves function calls from Dart side
Dart_NativeFunction callResolver(Dart_Handle name, int argc,
		bool* auto_setup_scope) {
	// If we fail, we return NULL, and Dart throws an exception.
	if (!Dart_IsString(name))
		return NULL;
	Dart_NativeFunction result = NULL;
	const char* cname;
	handleError(Dart_StringToCString(name, &cname));

	std::string sname(cname);

	if (std::string(cname) == "get5")
		result = get5;

	if (std::string(cname) == "add")
		result = add;
	return result;
}

/// This will be automatically called by Dart during initialization
DART_EXPORT Dart_Handle helloworld_Init(Dart_Handle parent_library) {
	if (Dart_IsError(parent_library))
		return parent_library;

	Dart_Handle result_code = Dart_SetNativeResolver(parent_library,
			callResolver, NULL);
	if (Dart_IsError(result_code))
		return result_code;

	return Dart_Null();
}
