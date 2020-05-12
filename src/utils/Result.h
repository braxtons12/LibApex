#pragma once

namespace apex {
	namespace utils {

		template<typename T, typename E>
			class Result {
				public:
					~Result();

					static Result<T, E> Ok(T ok);
					static Result<T, E> Err(E err);

					bool isOk();
					bool isErr();

					T get();
					T ok();
					E err();
				private:
					Result(T ok);
					Result(E err);
					Result();

					T mOk;
					E mErr;
					bool mIsOk = false;
			};
	}
}
