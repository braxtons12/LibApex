#include "Option.h"

namespace apex {
	namespace utils {

		template<typename T>
			Option<T>::~Option() {

			}

		template<typename T>
			Option<T> Option<T>::Some(T some) {
				return Option(some);
			}

		template<typename T>
			Option<T> Option<T>::None() {
				return Option();
			}

		template<typename T>
			bool Option<T>::isSome() {
				return mIsSome;
			}

		template<typename T>
			bool Option<T>::isNone() {
				return !mIsSome;
			}

		template<typename T>
			T Option<T>::get() {
				return mSome;
			}

		template<typename T>
			Option<T>::Option(T some) {
				mSome = some;
				mIsSome = true;
			}

		template<typename T>
			Option<T>::Option() {

			}
	}
}
