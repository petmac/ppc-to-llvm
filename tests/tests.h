#pragma once

#include "state.h"

#include "gtest/gtest.h"

#include <dlfcn.h>

class Tests : public testing::TestWithParam<std::tuple<const char *, size_t, size_t>> {
public:
	
	virtual void SetUp();
	
protected:
	
	std::unique_ptr<void, std::function<void(void *)>> dll;
	
	template <typename R, typename Address>
	void matches_expected() {
		typedef State<R, Address> State;
		typedef void RunFn(State *state);
		
		RunFn *const run = reinterpret_cast<RunFn *>(dlsym(dll.get(), "run"));
		ASSERT_NE(run, nullptr);
		
		State state = {};
		state.pc = 0x80000000; // TODO Get this from somewhere.
		(*run)(&state);
	}
};
