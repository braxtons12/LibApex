#pragma once

#include <tuple>

#include "../Error.h"
#include "../OptionAndResult.h"
#include "gtest/gtest.h"

namespace apex::utils::test {

	TEST(ResultTest, okGetMutValue) {
		auto ok = Ok(true);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(static_cast<bool>(ok));
		ASSERT_FALSE(ok.isErr());

		auto* gottenMut = ok.getMut();
		ASSERT_TRUE(gottenMut != nullptr);
		ASSERT_TRUE(*gottenMut);
		*gottenMut = false;
		gottenMut = ok.getMut();
		ASSERT_TRUE(gottenMut != nullptr);
		ASSERT_FALSE(*gottenMut);
	}

	TEST(ResultTest, okGetMutPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(static_cast<bool>(ok));
		ASSERT_FALSE(ok.isErr());

		auto* gottenMut = ok.getMut();
		ASSERT_TRUE(gottenMut != nullptr);
		ASSERT_TRUE(*gottenMut);
		*gottenMut = false;
		gottenMut = ok.getMut();
		ASSERT_TRUE(gottenMut != nullptr);
		ASSERT_FALSE(*gottenMut);
	}

	TEST(ResultTest, errGetMutValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(std::move(error));

		ASSERT_TRUE(err.isErr());
		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(static_cast<bool>(err));
		ASSERT_DEATH(std::ignore = err.getMut(), "getMut called on an Error result, terminating");
	}

	TEST(ResultTest, errGetMutPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool*, Error>(std::move(error));

		ASSERT_TRUE(err.isErr());
		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(static_cast<bool>(err));
		ASSERT_DEATH(std::ignore = err.getMut(), "getMut called on an Error result, terminating");
	}

	TEST(ResultTest, okGetConstValue) {
		auto ok = Ok(true);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(static_cast<bool>(ok));
		ASSERT_FALSE(ok.isErr());

		auto* gottenMut = ok.getConst();
		ASSERT_TRUE(gottenMut != nullptr);
		ASSERT_TRUE(*gottenMut);
	}

	TEST(ResultTest, okGetConstPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(static_cast<bool>(ok));
		ASSERT_FALSE(ok.isErr());

		const auto* gottenMut = ok.getConst();
		ASSERT_TRUE(gottenMut != nullptr);
		ASSERT_TRUE(*gottenMut);
	}

	TEST(ResultTest, errGetConstValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(std::move(error));

		ASSERT_TRUE(err.isErr());
		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(static_cast<bool>(err));
		ASSERT_DEATH(std::ignore = err.getConst(),
					 "getConst called on an Error result, terminating");
	}

	TEST(ResultTest, errGetConstPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool*, Error>(std::move(error));

		ASSERT_TRUE(err.isErr());
		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(static_cast<bool>(err));
		ASSERT_DEATH(std::ignore = err.getConst(),
					 "getConst called on an Error result, terminating");
	}

	TEST(ResultTest, okUnwrapValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_TRUE(ok.unwrap());
	}

	TEST(ResultTest, okUnwrapPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		auto* ptr = ok.unwrap();
		ASSERT_TRUE(*ptr);
	}

	TEST(ResultTest, errUnwrapValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(std::move(error));

		ASSERT_TRUE(err.isErr());
		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);

		ASSERT_DEATH(std::ignore = err.unwrap(), "unwrap called on an Error result, terminating");
	}

	TEST(ResultTest, errUnwrapPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool*, Error>(std::move(error));

		ASSERT_TRUE(err.isErr());
		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);

		ASSERT_DEATH(std::ignore = err.unwrap(), "unwrap called on an Error result, terminating");
	}

	TEST(ResultTest, okUnwrapOrValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_TRUE(ok.unwrapOr(false));
	}

	TEST(ResultTest, okUnwrapOrPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value2 = new bool(false);
		ASSERT_TRUE(*ok.unwrapOr(value2));
	}

	TEST(ResultTest, errUnwrapOrValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_FALSE(err.unwrapOr(false));
	}

	TEST(ResultTest, errUnwrapOrPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool*, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value2 = new bool(false);
		ASSERT_FALSE(*err.unwrapOr(value2));
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		delete value2;
	}

	TEST(ResultTest, okUnwrapOrElseValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_TRUE(ok.unwrapOrElse([]() noexcept -> bool { return false; }));
	}

	TEST(ResultTest, okUnwrapOrElsePointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value2 = new bool(false);
		ASSERT_TRUE(*ok.unwrapOrElse([val = value2]() noexcept -> bool* { return val; }));

		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		delete value;
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		delete value2;
	}

	TEST(ResultTest, errUnwrapOrElseValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_FALSE(err.unwrapOrElse([]() noexcept -> bool { return false; }));
	}

	TEST(ResultTest, errUnwrapOrElsePointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool*, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(false);
		ASSERT_FALSE(*err.unwrapOrElse([val = value]() noexcept -> bool* { return val; }));

		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		delete value;
	}

	TEST(ResultTest, okUnwrapErrValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_DEATH(std::ignore = ok.unwrapErr(), "unwrapErr called on an Ok result, terminating");
	}

	TEST(ResultTest, okUnwrapErrPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_DEATH(std::ignore = ok.unwrapErr(), "unwrapErr called on an Ok result, terminating");
	}

	TEST(ResultTest, errUnwrapErrValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_EQ(err.unwrapErr().messageAsStdString(), "TestErrorMessage");
	}

	TEST(ResultTest, errUnwrapErrPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error*>(&error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_EQ(err.unwrapErr()->messageAsStdString(), "TestErrorMessage");
	}

	TEST(ResultTest, okokValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		auto maybe_ok = ok.ok();
		ASSERT_TRUE(maybe_ok.isSome());
		ASSERT_TRUE(maybe_ok.unwrap());
	}

	TEST(ResultTest, okokPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		auto maybe_ok = ok.ok();
		ASSERT_TRUE(maybe_ok.isSome());
		ASSERT_TRUE(*maybe_ok.unwrap());

		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		delete value;
	}

	TEST(ResultTest, errokValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		auto maybe_ok = err.ok();
		ASSERT_TRUE(maybe_ok.isNone());
		ASSERT_DEATH(std::ignore = maybe_ok.unwrap(), "unwrap called on a None, terminating");
	}

	TEST(ResultTest, errokPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error*>(&error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		auto maybe_ok = err.ok();
		ASSERT_TRUE(maybe_ok.isNone());
		ASSERT_DEATH(std::ignore = maybe_ok.unwrap(), "unwrap called on a None, terminating");
	}

	TEST(ResultTest, okerrValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		auto maybe_ok = ok.err();
		ASSERT_TRUE(maybe_ok.isNone());
		ASSERT_DEATH(std::ignore = maybe_ok.unwrap(), "unwrap called on a None, terminating");
	}

	TEST(ResultTest, okerrPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		auto maybe_ok = ok.err();
		ASSERT_TRUE(maybe_ok.isNone());
		ASSERT_DEATH(std::ignore = maybe_ok.unwrap(), "unwrap called on a None, terminating");
	}

	TEST(ResultTest, errerrValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		auto maybe_ok = err.err();
		ASSERT_TRUE(maybe_ok.isSome());
		ASSERT_EQ(maybe_ok.unwrap().messageAsStdString(), "TestErrorMessage");
	}

	TEST(ResultTest, errerrPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error*>(&error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		auto maybe_ok = err.err();
		ASSERT_TRUE(maybe_ok.isSome());
		ASSERT_EQ(maybe_ok.unwrap()->messageAsStdString(), "TestErrorMessage");
	}

	TEST(ResultTest, okMapValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_EQ(ok.map<int>([](const bool _val) noexcept -> int {
						std::ignore = _val;
						return 2;
					}).unwrap(),
				  2);
	}

	TEST(ResultTest, okMapPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_EQ(ok.map<int>([](const bool* _val) noexcept -> int {
						std::ignore = _val;
						return 2;
					}).unwrap(),
				  2);
	}

	TEST(ResultTest, errMapValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_EQ(err.map<int>([](const bool _val) noexcept -> int {
						 std::ignore = _val;
						 return 2;
					 })
					  .unwrapErr()
					  .messageAsStdString(),
				  "TestErrorMessage");
	}

	TEST(ResultTest, errMapPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error*>(&error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_EQ(err.map<int>([](const bool _val) noexcept -> int {
						 std::ignore = _val;
						 return 2;
					 })
					  .unwrapErr()
					  ->messageAsStdString(),
				  "TestErrorMessage");
	}

	TEST(ResultTest, okMapOrValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_EQ(ok.mapOr<int>(
					  [](const bool _val) noexcept -> int {
						  std::ignore = _val;
						  return 2;
					  },
					  4),
				  2);
	}

	TEST(ResultTest, okMapOrPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_EQ(ok.mapOr<int>(
					  [](const bool* _val) noexcept -> int {
						  std::ignore = _val;
						  return 2;
					  },
					  4),
				  2);
	}

	TEST(ResultTest, errMapOrValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_EQ(err.mapOr<int>(
					  [](const bool _val) noexcept -> int {
						  std::ignore = _val;
						  return 2;
					  },
					  4),
				  4);
	}

	TEST(ResultTest, errMapOrPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error*>(&error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_EQ(err.mapOr<int>(
					  [](const bool _val) noexcept -> int {
						  std::ignore = _val;
						  return 2;
					  },
					  4),
				  4);
	}

	TEST(ResultTest, okMapOrElseValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_EQ(ok.mapOrElse<int>(
					  [](const bool _val) noexcept -> int {
						  std::ignore = _val;
						  return 2;
					  },
					  []() { return 4; }),
				  2);
	}

	TEST(ResultTest, okMapOrElsePointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		ASSERT_EQ(ok.mapOrElse<int>(
					  [](const bool* _val) noexcept -> int {
						  std::ignore = _val;
						  return 2;
					  },
					  []() { return 4; }),
				  2);
	}

	TEST(ResultTest, errMapOrElseValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_EQ(err.mapOrElse<int>(
					  [](const bool _val) noexcept -> int {
						  std::ignore = _val;
						  return 2;
					  },
					  []() { return 4; }),
				  4);
	}

	TEST(ResultTest, errMapOrElsePointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error*>(&error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		ASSERT_EQ(err.mapOrElse<int>(
					  [](const bool _val) noexcept -> int {
						  std::ignore = _val;
						  return 2;
					  },
					  []() { return 4; }),
				  4);
	}

	TEST(ResultTest, okMapErrValue) {
		auto value = true;
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		// NOLINTNEXTLINE(performance-unnecessary-value-param)
		ASSERT_EQ(ok.mapErr<Error>([](const Error _val) noexcept -> Error {
						std::ignore = _val;
						return Error("TestErrorMessage");
					}).unwrap(),
				  true);
	}

	TEST(ResultTest, okMapErrPointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		auto ok = Ok(value);

		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());

		// NOLINTNEXTLINE(performance-unnecessary-value-param)
		ASSERT_EQ(*ok.mapErr<Error>([](const Error _val) noexcept -> Error {
						 std::ignore = _val;
						 return Error("TestErrorMessage");
					 }).unwrap(),
				  true);
	}

	TEST(ResultTest, errMapErrValue) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error>(error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		// NOLINTNEXTLINE(performance-unnecessary-value-param)
		ASSERT_EQ(err.mapErr<Error>([](const Error _val) noexcept -> Error {
						 std::ignore = _val;
						 return Error("TestErrorMessage2");
					 })
					  .unwrapErr()
					  .messageAsStdString(),
				  "TestErrorMessage2");
	}

	TEST(ResultTest, errMapErrPointer) {
		auto error = Error("TestErrorMessage");
		auto err = Err<bool, Error*>(&error);

		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());

		// NOLINTNEXTLINE(performance-unnecessary-value-param)
		ASSERT_EQ(err.mapErr<Error>([](const Error* _val) noexcept -> Error {
						 std::ignore = _val;
						 return Error("TestErrorMessage2");
					 })
					  .unwrapErr()
					  .messageAsStdString(),
				  "TestErrorMessage2");
	}

	// NOLINTNEXTLINE(misc-definitions-in-headers)
	auto okMoveTest(Result<bool*, Error>&& ok) noexcept -> void {
		ASSERT_TRUE(ok.isOk());
		ASSERT_TRUE(ok);
		ASSERT_FALSE(ok.isErr());
		ASSERT_TRUE(*ok.unwrap());
	}

	TEST(ResultTest, okMovePointer) {
		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		auto* value = new bool(true);
		{
			auto ok = Ok(value);
			okMoveTest(std::move(ok));
		}

		// NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
		delete value;
	}

	// NOLINTNEXTLINE(misc-definitions-in-headers)
	auto errMoveTest(Result<bool*, Error*>&& err) noexcept -> void {
		ASSERT_FALSE(err.isOk());
		ASSERT_FALSE(err);
		ASSERT_TRUE(err.isErr());
		ASSERT_EQ(err.unwrapErr()->messageAsStdString(), "TestErrorMessage");
	}

	TEST(ResultTest, errMovePointer) {
		auto error = Error("TestErrorMessage");
		{
			auto err = Err<bool*, Error*>(&error);
			errMoveTest(std::move(err));
		}
	}
} // namespace apex::utils::test
