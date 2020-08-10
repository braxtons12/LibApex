#define APEX_DECLARE_NON_HEAP_ALLOCATABLE()                        \
	static auto operator new(std::size_t size) noexcept->void* {   \
		juce::ignoreUnused(size);                                  \
		return nullptr;                                            \
	}                                                              \
                                                                   \
	static auto operator new[](std::size_t size) noexcept->void* { \
		juce::ignoreUnused(size);                                  \
		return nullptr;                                            \
	}                                                              \
                                                                   \
	static auto operator delete(void* obj) noexcept->void {        \
		juce::ignoreUnused(obj);                                   \
	}                                                              \
                                                                   \
	static auto operator delete[](void* obj) noexcept->void {      \
		juce::ignoreUnused(obj);                                   \
	}\
