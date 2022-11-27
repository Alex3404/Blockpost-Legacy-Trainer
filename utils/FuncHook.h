#pragma once
#include <MinHook.h>

class FuncHook
{
private:
	void* realFunctionPtr;
	void* originalFunc;
public:
	FuncHook(void* func, void* newFunc);
	~FuncHook();
	void Enable();
	void Disable();
	
	template <typename returnType, typename... argumentTypes>
	inline auto* GetCall()
	{
		return reinterpret_cast<returnType(*)(argumentTypes...)>(realFunctionPtr);
	}
};