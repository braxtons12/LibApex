#pragma once

#ifndef DYNAMICS_STATE
#define DYNAMICS_STATE

#include <functional>
#include <type_traits>
#include <utility>

#include "../../base/StandardIncludes.h"

namespace apex {
	namespace dsp {

		/// @brief Type to own and maintain all of a dynamics processor's shared state
		///
		/// @tparam T - The floating point type used for parameters
		/// @tparam AttackKind - The floating point or enum type used for attack times
		/// @tparam ReleaseKind - The floating point or enum type used for release times
		template<typename T, typename AttackKind, typename ReleaseKind>
			class DynamicsState {
				public:
					static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
					static_assert(std::is_floating_point<AttackKind>::value ||
							std::is_enum<AttackKind>::value,
							"AttacKind must be a floating point type or an enum");
					static_assert(std::is_floating_point<ReleaseKind>::value ||
							std::is_enum<ReleaseKind>::value,
							"ReleaseKind must be a floating point type or an enum");

					/// @brief The fields in the State callbacks can be registered for
					enum class Field {
						Attack,
						Release,
						Ratio,
						Threshold,
						KneeWidth,
						SampleRate
					};

					/// @brief Constructs a blank `DynamicsState` with everything zeroed
					DynamicsState() noexcept = default;

					/// @brief Constructs a `DynamicsState` with the given parameters
					///
					/// @param attack - The attack time, in Seconds if AttackKind is floating point
					/// @param release - The release time, in Seconds if ReleaseKind is floating point
					/// @param ratio - The ratio of the processing curve
					/// @param threshold - The threshold at which to begin processing, in Decibels
					/// @param kneeWidth - The knee width, in Decibels
					/// @param sampleRate - The sample rate, in Hertz
					DynamicsState(AttackKind attack, ReleaseKind release, T ratio, T threshold,
							T kneeWidth, size_t sampleRate) noexcept
						: mAttack(attack),
						mRelease(release),
						mRatio(ratio),
						mThreshold(threshold),
						mKneeWidth(kneeWidth),
						mSampleRate(sampleRate)
						{

						}

					/// @brief Move constructs the given `DynamicsState`
					///
					/// @param state - The `DynamicsState` to move
					DynamicsState(DynamicsState<T, AttackKind, ReleaseKind>&& state) noexcept = default;

					~DynamicsState() noexcept = default;

					/// @brief Sets the attack to the given value
					///
					/// @param attack - The new attack
					void setAttack(AttackKind attack) noexcept {
						mAttack = attack;

						for(auto callback : mAttackCallbacks) {
							callback(mAttack);
						}
					}

					/// @brief Returns the current attack
					///
					/// @return - The attack
					AttackKind getAttack() const noexcept {
						return mAttack;
					}

					/// @brief Sets the release to the given value
					///
					/// @param release - The new release
					void setRelease(ReleaseKind release) noexcept {
						mRelease = release;

						for(auto callback : mReleaseCallbacks) {
							callback(mRelease);
						}
					}

					/// @brief Returns the current release
					///
					/// @return - The release
					ReleaseKind getRelease() const noexcept {
						return mRelease;
					}

					/// @brief Sets the ratio to the given value
					///
					/// @param ratio - The new ratio
					void setRatio(T ratio) noexcept {
						mRatio = ratio;

						for(auto callback : mRatioCallbacks) {
							callback(mRatio);
						}
					}

					/// @brief Returns the current ratio
					///
					/// @return - The ratio
					T getRatio() const noexcept {
						return mRatio;
					}

					/// @brief Sets the threshold to the given value
					///
					/// @param threshold - The new threshold, in Decibels
					void setThreshold(T threshold) noexcept {
						mThreshold = threshold;

						for(auto callback : mThresholdCallbacks) {
							callback(mThreshold);
						}
					}

					/// @brief Returns the current threshold
					///
					/// @return - The threshold, in Decibels
					T getThreshold() const noexcept {
						return mThreshold;
					}

					/// @brief Sets the knee width to the given value
					///
					/// @param kneeWidth - The new knee width, in Decibels
					void setKneeWidth(T kneeWidth) noexcept {
						mKneeWidth = kneeWidth;

						for(auto callback : mKneeWidthCallbacks) {
							callback(mKneeWidth);
						}
					}

					/// @brief Returns the current knee width
					///
					/// @return - The knee width, in Decibels
					T getKneeWidth() const noexcept {
						return mKneeWidth;
					}

					/// @brief Sets the sample rate to the given value
					///
					/// @param sampleRate - The new sample rate, in Hertz
					void setSampleRate(size_t sampleRate) noexcept {
						mSampleRate = sampleRate;

						for(auto callback : mSampleRateCallbacks) {
							callback(mSampleRate);
						}
					}

					/// @brief Returns the current sample rate
					///
					/// @return - The sample rate, in Hertz
					size_t getSampleRate() const noexcept {
						return mSampleRate;
					}

					/// @brief Sets the first attack coefficient to the given value
					///
					/// @param attack - The new first attack coefficient
					void setAttackCoefficient1(T attack) noexcept {
						mAttackCoefficient1 = attack;
					}

					/// @brief Sets the second attack coefficient to the given value
					///
					/// @param attack - The new second attack coefficient
					void setAttackCoefficient2(T attack) noexcept {
						mAttackCoefficient2 = attack;
					}

					/// @brief Sets the attack coefficients to the given value
					///
					/// @param attack1 - The new first attack coefficient
					/// @param attack2 - The new second attack coefficient
					void setAttackCoefficients(T attack1, T attack2) noexcept {
						mAttackCoefficient1 = attack1;
						mAttackCoefficient2 = attack2;
					}

					/// @brief Returns the current first attack coefficient
					///
					/// @return - The first attack coefficient
					T getAttackCoefficient1() const noexcept {
						return mAttackCoefficient1;
					}

					/// @brief Returns the current second attack coefficient
					///
					/// @return - The second attack coefficient
					T getAttackCoefficient2() const noexcept {
						return mAttackCoefficient2;
					}

					/// @brief Returns the current attack coefficients
					///
					/// @return - The first and second attack coefficients, as a std::tuple
					std::tuple<T, T> getAttackCoefficients() const noexcept {
						return { mAttackCoefficient1, mAttackCoefficient2 };
					}

					/// @brief Sets the first release coefficient to the given value
					///
					/// @param release - The new first release coefficient
					void setReleaseCoefficient1(T release) noexcept {
						mReleaseCoefficient1 = release;
					}

					/// @brief Sets the second release coefficient to the given value
					///
					/// @param release - The new second release coefficient
					void setReleaseCoefficient2(T release) noexcept {
						mReleaseCoefficient2 = release;
					}

					/// @brief Sets the release coefficients to the given values
					///
					/// @param release1 - The new first release coefficient
					/// @param release2 - The new seconds release coefficient
					void setReleaseCoefficients(T release1, T release2) noexcept {
						mReleaseCoefficient1 = release1;
						mReleaseCoefficient2 = release2;
					}

					/// @brief Returns the current first release coefficient
					///
					/// @return - The first release coefficient
					T getReleaseCoefficient1() const noexcept {
						return mReleaseCoefficient1;
					}

					/// @brief Returns the current second release coefficient
					///
					/// @return - The second release coefficient
					T getReleaseCoefficient2() const noexcept {
						return mReleaseCoefficient2;
					}

					/// @brief Returns the current release coefficients
					///
					/// @return  - The first and second release coefficients, as a std::tuple
					std::tuple<T, T> getReleaseCoefficients() const noexcept {
						return { mReleaseCoefficient1, mReleaseCoefficient2 };
					}

					/// @brief Registers the given callback to be called on changes to the given field.
					/// The callback is called immediately to allow for synchronization with the current state.
					///
					/// @tparam F - The type associated with the given field indicated by `field`,
					/// eg: for `Ratio`, this could be `float` or `double` and for `Attack` this could be `float`,
					/// `double`, or an associated `enum`, depending on what was specified at construction time.
					/// The type of the field and this type MUST match (failure to do this will cause a compile-time error).
					///
					/// @param field - The field to register the callback on
					/// @param callback - The callback to register
					template<typename F>
						void registerCallback(Field field, std::function<void(F)> callback) noexcept {
							//ensure that F matches the field type
							static_assert((field == Field::Attack && std::is_same<F, AttackKind>::value) ||
									(field == Field::Release && std::is_same<F, ReleaseKind>::value) ||
									( (field == Field::Ratio || field == Field::Threshold || field == Field::KneeWidth)
									  && std::is_same<F, T>::value) || (field == Field::SampleRate && std::is_same<F, size_t>::value),
									"The callback type, `F`, must match the type of the field associated with `field`");

							//register the callback
							switch(field) {
								case Field::Attack: {
														callback(mAttack);
														mAttackCallbacks.push_back(callback);
													}
													break;
								case Field::Release: {
														 callback(mRelease);
														 mReleaseCallbacks.push_back(callback);
													 }
													 break;
								case Field::Ratio: {
													   callback(mRatio);
													   mRatioCallbacks.push_back(callback);
												   }
												   break;
								case Field::Threshold: {
														   callback(mThreshold);
														   mThresholdCallbacks.push_back(callback);
													   }
													   break;
								case Field::KneeWidth: {
														   callback(mKneeWidth);
														   mKneeWidthCallbacks.push_back(callback);
													   }
													   break;
								case Field::SampleRate: {
															callback(mSampleRate);
															mSampleRateCallbacks.push_back(callback);
														}
														break;
							}
						}

					DynamicsState<T, AttackKind, ReleaseKind>& operator=(
							DynamicsState<T, AttackKind, ReleaseKind>&& state) noexcept = default;

				private:
					///State variables

					///The attack, can be `float`, `double`, or an associated `enum`
					AttackKind mAttack = 0;
					///The release, can be `float, `double`, or an associated `enum`
					ReleaseKind mRelease = 0;
					///The ratio, `float` or `double`
					T mRatio = 1.1;
					///The threshold in Decibels, `float` or `double`
					T mThreshold = -12.0;
					///The knee width in Decibels, `float` or `double`
					T mKneeWidth = 6.0;
					///The first attack coefficient, `float` or `double`
					T mAttackCoefficient1 = 0.0;
					///The second attack coefficient, `float` or `double`
					T mAttackCoefficient2 = 0.0;
					///The first release coefficient, `float` or `double`
					T mReleaseCoefficient1 = 0.0;
					///The second release coefficient, `float` or `double`
					T mReleaseCoefficient2 = 0.0;
					///The sample rate in Hertz
					size_t mSampleRate = 44100;

					///Callback containers

					///Attack callbacks
					std::vector<std::function<void(AttackKind)>> mAttackCallbacks;
					///Release callbacks
					std::vector<std::function<void(ReleaseKind)>> mReleaseCallbacks;
					///Ratio callbacks
					std::vector<std::function<void(T)>> mRatioCallbacks;
					///Threshold callbacks
					std::vector<std::function<void(T)>> mThresholdCallbacks;
					///Knee width callbacks
					std::vector<std::function<void(T)>> mKneeWidthCallbacks;
					///Sample rate callbacks
					std::vector<std::function<void(size_t)>> mSampleRateCallbacks;

					JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DynamicsState)
			};
	}
}

#endif //DYNAMICS_STATE
