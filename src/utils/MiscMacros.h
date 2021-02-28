#define APEX_DECLARE_NON_HEAP_ALLOCATABLE()	   /** NOLINT(cppcoreguidelines-macro-usage): This isn't \
												  a function-like macro                      **/     \
	static auto operator new(std::size_t size) noexcept->void* = delete;                             \
	static auto operator new[](std::size_t size) noexcept->void* = delete;                           \
	static auto operator delete(void* obj) noexcept->void = delete;                                  \
	static auto operator delete[](void* obj) noexcept->void = delete;\
