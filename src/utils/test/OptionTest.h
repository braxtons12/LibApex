#pragma once

#include <tuple>

#include "../Error.h"
#include "../OptionAndResult.h"
#include "gtest/gtest.h"

namespace apex::utils::test {

	TEST(OptionTest, someMapping) {
		auto some = Some(true);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		ASSERT_TRUE(
			some.map<bool>([](const bool some_value) noexcept -> bool { return some_value; })
				.isSome());
		ASSERT_FALSE(
			some.map<bool>([](const bool some_value) noexcept -> bool { return some_value; })
				.isNone());
		ASSERT_TRUE(
			some.mapOr<bool>([](const bool some_value) noexcept -> bool { return some_value; },
							 false));
		ASSERT_TRUE(
			some.mapOrElse<bool>([](const bool some_value) noexcept -> bool { return some_value; },
								 []() noexcept -> bool { return false; }));
	}

	TEST(OptionTest, noneMapping) {
		auto none = None<bool>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		ASSERT_TRUE(
			none.map<bool>([](const bool some_value) noexcept -> bool { return some_value; })
				.isNone());
		ASSERT_FALSE(
			none.map<bool>([](const bool some_value) noexcept -> bool { return some_value; })
				.isSome());
		ASSERT_FALSE(
			none.mapOr<bool>([](const bool some_value) noexcept -> bool { return some_value; },
							 false));
		ASSERT_FALSE(
			none.mapOrElse<bool>([](const bool some_value) noexcept -> bool { return some_value; },
								 []() noexcept -> bool { return false; }));
	}

	TEST(OptionTest, someOkOrValue) {
		auto some = Some(true);
		auto error = Error("TestErrorMessage");

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());

		auto res = some.okOr(std::move(error));
		ASSERT_TRUE(res.isOk());
		ASSERT_TRUE(res.unwrap());
	}

	TEST(OptionTest, someOkOrPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto some = Some(value);
		auto error = Error("TestErrorMessage");

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());

		auto res = some.okOr(std::move(error));
		ASSERT_TRUE(res.isOk());
		auto ok = res.ok();
		ASSERT_TRUE(ok.isSome());
		auto* unwrapped = ok.unwrap();
		ASSERT_TRUE(unwrapped != nullptr);
		ASSERT_TRUE(*unwrapped);
	}

	TEST(OptionTest, noneOkOrValue) {
		auto none = None<bool>();
		auto error = Error("TestErrorMessage");

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());

		auto res = none.okOr(std::move(error));
		ASSERT_TRUE(res.isErr());
		ASSERT_TRUE(res.unwrapErr().messageAsStdString() == std::string("TestErrorMessage"));
	}

	TEST(OptionTest, noneOkOrReference) {
		auto none = None<bool>();
		auto error = Error("TestErrorMessage");
		auto& errorRef = error;

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());

		auto res = none.okOr(std::forward<Error>(errorRef));
		ASSERT_TRUE(res.isErr());
		ASSERT_TRUE(res.unwrapErr().messageAsStdString() == std::string("TestErrorMessage"));
	}

	TEST(OptionTest, someOkOrElseValue) {
		auto some = Some(true);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());

		auto res = some.okOrElse<Error>([]() { return Error("TestErrorMessage"); });
		ASSERT_TRUE(res.isOk());
		ASSERT_TRUE(res.unwrap());
	}

	TEST(OptionTest, someOkOrElsePointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto some = Some(value);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());

		auto res = some.okOrElse<Error>([]() { return Error("TestErrorMessage"); });
		ASSERT_TRUE(res.isOk());
		auto* unwrapped = res.unwrap();
		ASSERT_TRUE(unwrapped != nullptr);
		ASSERT_TRUE(*unwrapped);
	}

	TEST(OptionTest, noneOkOrElseValue) {
		auto none = None<bool>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());

		auto res = none.okOrElse<Error>([]() { return Error("TestErrorMessage"); });
		ASSERT_TRUE(res.isErr());
		ASSERT_TRUE(res.unwrapErr().messageAsStdString() == std::string("TestErrorMessage"));
	}

	TEST(OptionTest, noneOkOrElseReference) {
		auto none = None<bool>();
		auto error = Error("TestErrorMessage");

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());

		auto res = none.okOrElse<Error>([&error]() { return std::forward<Error>(error); });
		ASSERT_TRUE(res.isErr());
		ASSERT_TRUE(res.unwrapErr().messageAsStdString() == std::string("TestErrorMessage"));
	}

	TEST(OptionTest, someUnwrapValue) {
		auto some = Some(true);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		ASSERT_TRUE(some.unwrap());
	}

	TEST(OptionTest, someUnwrapPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto some = Some(value);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());

		auto* unwrapped = some.unwrap();
		ASSERT_TRUE(unwrapped != nullptr);
		ASSERT_TRUE(*unwrapped);
	}

	TEST(OptionTest, noneUnwrap) {
		auto none = None<bool>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());

		ASSERT_DEATH(std::ignore = none.unwrap(), "unwrap called on a None, terminating");
	}

	TEST(OptionTest, someUnwrapOrValue) {
		auto some = Some(true);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		ASSERT_TRUE(some.unwrapOr(false));
	}

	TEST(OptionTest, someUnwrapOrPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* someValue = new bool(true);
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* noneValue = new bool(false);
		auto some = Some(someValue);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		auto* unwrapped = some.unwrapOr(noneValue);
		ASSERT_TRUE(unwrapped != nullptr);
		ASSERT_TRUE(*unwrapped);
	}

	TEST(OptionTest, noneUnwrapOrValue) {
		auto none = None<bool>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		ASSERT_FALSE(none.unwrapOr(false));
	}

	TEST(OptionTest, noneUnwrapOrPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* noneValue = new bool(false);
		auto none = None<bool*>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		auto* unwrapped = none.unwrapOr(noneValue);
		ASSERT_TRUE(unwrapped != nullptr);
		ASSERT_FALSE(*unwrapped);
	}

	TEST(OptionTest, someUnwrapOrElseValue) {
		auto some = Some(true);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		ASSERT_TRUE(some.unwrapOrElse([]() { return false; }));
	}

	TEST(OptionTest, someUnwrapOrElsePointer) {

		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* someValue = new bool(true);
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* noneValue = new bool(false);
		auto some = Some(someValue);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		auto* unwrapped = some.unwrapOrElse([noneValue]() { return noneValue; });
		ASSERT_TRUE(unwrapped != nullptr);
		ASSERT_TRUE(*unwrapped);
	}

	TEST(OptionTest, noneUnwrapOrElseValue) {
		auto none = None<bool>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		ASSERT_FALSE(none.unwrapOrElse([]() { return false; }));
	}

	TEST(OptionTest, noneUnwrapOrElsePointer) {
		auto none = None<bool*>();
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* noneValue = new bool(false);

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		auto* unwrapped = none.unwrapOrElse([noneValue]() { return noneValue; });
		ASSERT_TRUE(unwrapped != nullptr);
		ASSERT_FALSE(*unwrapped);
	}

	TEST(OptionTest, someGetMutValue) {
		auto some = Some(true);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		auto* gottenMut = some.getMut();
		ASSERT_TRUE(*gottenMut);
		*gottenMut = false;
		gottenMut = some.getMut();
		ASSERT_FALSE(*gottenMut);
	}

	TEST(OptionTest, someGetMutPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* someValue = new bool(true);
		auto some = Some(someValue);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		auto* gottenMut = some.getMut();
		ASSERT_TRUE(*gottenMut);
		*gottenMut = false;
		gottenMut = some.getMut();
		ASSERT_FALSE(*gottenMut);
	}

	TEST(OptionTest, noneGetMutValue) {
		auto none = None<bool>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		ASSERT_DEATH(std::ignore = none.getMut(), "getMut called on a None, terminating");
	}

	TEST(OptionTest, noneGetMutPointer) {
		auto none = None<bool*>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		ASSERT_DEATH(std::ignore = none.getMut(), "getMut called on a None, terminating");
	}

	TEST(OptionTest, someGetConstValue) {
		auto some = Some(true);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		const auto* gottenConst = some.getConst();
		ASSERT_TRUE(*gottenConst);
		//*gottenConst = false; won't compile, as desired
	}

	TEST(OptionTest, someGetConstPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* someValue = new bool(true);
		auto some = Some(someValue);

		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		const auto* gottenConst = some.getConst();
		ASSERT_TRUE(*gottenConst);
		//*gottenConst = false; won't compile, as desired
	}

	TEST(OptionTest, noneGetConstValue) {
		auto none = None<bool>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		ASSERT_DEATH(std::ignore = none.getConst(), "getConst called on a None, terminating");
	}

	TEST(OptionTest, noneGetConstPointer) {
		auto none = None<bool*>();

		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		ASSERT_DEATH(std::ignore = none.getConst(), "getConst called on a None, terminating");
	}

	// NOLINTNEXTLINE(misc-definitions-in-headers)
	auto someMoveTest(Option<bool*>&& some) noexcept -> void {
		ASSERT_TRUE(some.isSome());
		ASSERT_FALSE(some.isNone());
		auto* unwrapped = some.unwrap();
		ASSERT_TRUE(unwrapped != nullptr);
		ASSERT_TRUE(*unwrapped);
	}

	TEST(OptionTest, someMovePointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* someValue = new bool(true);
		{
			auto some = Some(someValue);
			someMoveTest(std::move(some));
		}
	}

	// NOLINTNEXTLINE(misc-definitions-in-headers, readability-function-cognitive-complexity)
	auto noneMoveTest(Option<bool*>&& none) noexcept -> void {
		ASSERT_TRUE(none.isNone());
		ASSERT_FALSE(none.isSome());
		ASSERT_DEATH(std::ignore = none.unwrap(), "unwrap called on a None, terminating");
	}

	TEST(OptionTest, noneMovePointer) {
		auto none = None<bool*>();
		noneMoveTest(std::move(none));
	}
} // namespace apex::utils::test
