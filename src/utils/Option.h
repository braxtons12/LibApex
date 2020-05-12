#pragma once

namespace apex {
	namespace utils {

		template<typename T>
			class Option {
				public:
					~Option();

					static Option<T> Some(T some);
					static Option<T> None();

					bool isSome();
					bool isNone();

					T get();

				private:
					Option(T some);
					Option();

					T mSome;
					bool mIsSome = false;
			};
	}
}
