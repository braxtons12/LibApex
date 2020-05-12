#include "Result.h"

namespace apex {
	namespace utils {

		template<typename T, typename E>
			Result<T, E>::~Result() {

			}

		template<typename T, typename E>
			Result<T, E> Result<T, E>::Ok(T ok) {
				return Result<T,E>(ok);
			}

		template<typename T, typename E>
			Result<T, E> Result<T, E>::Err(E err) {
				return Result<T, E>(err);
			}

		template<typename T, typename E>
			bool Result<T, E>::isOk() {
				return mIsOk;
			}

		template<typename T, typename E>
			bool Result<T, E>::isErr() {
				return !mIsOk;
			}

		template<typename T, typename E>
			T Result<T, E>::get() {
				return mOk;
			}

		template<typename T, typename E>
			T Result<T, E>::ok() {
				return mOk;
			}

		template<typename T, typename E>
			E Result<T, E>::err() {
				return mErr;
			}

		template<typename T, typename E>
			Result<T, E>::Result(T ok)
			: mOk(ok), mIsOk(true)
			{

			}

		template<typename T, typename E>
			Result<T, E>::Result(E err)
			: mErr(err), mIsOk(false)
			{

			}

		template<typename T, typename E>
			Result<T, E>::Result() {

			}
	}
}
