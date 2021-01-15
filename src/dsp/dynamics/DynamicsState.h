#pragma once

#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

#include "../../base/StandardIncludes.h"

#ifndef DYNAMICS_STATE
	#define DYNAMICS_STATE

namespace apex::dsp {
	/// @brief The fields in the State callbacks can be registered for
	enum class DynamicsField
	{
		Attack,
		Release,
		Ratio,
		Threshold,
		KneeWidth,
		SampleRate,
		AutoRelease
	};

	/// @brief Determines if the given combination of template parameters are a valid combination
	/// for dynamics processor component types
	///
	/// @tparam FloatType - The floating point type to use for calculations (float or double)
	/// @tparam AttackKind - The type used to represent attack times, either FloatType or an enum
	/// @tparam ReleaseKind - The type used to represent release times, either FloatType or an enum
	///
	/// @return `true` if the combination is valid, `false` otherwise
	template<typename FloatType = float,
			 typename AttackKind = FloatType,
			 typename ReleaseKind = FloatType>
	static constexpr auto areDynamicsParamsValid() noexcept -> bool {
		auto constexpr isFloat = std::is_floating_point_v<FloatType>;
		auto constexpr isAttackValid
			= std::is_same_v<FloatType, AttackKind> || std::is_enum_v<AttackKind>;
		auto constexpr isReleaseValid
			= std::is_same_v<FloatType, ReleaseKind> || std::is_enum_v<ReleaseKind>;
		return isFloat && isAttackValid && isReleaseValid;
	}

	/// @brief Type to own and maintain all of a dynamics processor's shared state
	///
	/// @tparam FloatType - The floating point type used for parameters
	/// @tparam AttackKind - The floating point or enum type used for attack times
	/// @tparam ReleaseKind - The floating point or enum type used for release times
	template<
		typename FloatType = float,
		typename AttackKind = FloatType,
		typename ReleaseKind = FloatType,
		std::enable_if_t<areDynamicsParamsValid<FloatType, AttackKind, ReleaseKind>(), bool> = true>
	class DynamicsState {
	  public:
		using AttackCallback = std::function<void(AttackKind)>;
		using ReleaseCallback = std::function<void(ReleaseKind)>;
		using RatioCallback = std::function<void(FloatType)>;
		using ThresholdCallback = std::function<void(Decibels)>;
		using KneeWidthCallback = std::function<void(Decibels)>;
		using SampleRateCallback = std::function<void(Hertz)>;
		using AutoReleaseEnableCallback = std::function<void(bool)>;

		/// @brief Constructs a blank `DynamicsState` with everything zeroed
		constexpr DynamicsState() noexcept = default;

		/// @brief Constructs a `DynamicsState` with the given parameters
		///
		/// @param attack - The attack time, in Seconds if AttackKind is floating point
		/// @param release - The release time, in Seconds if ReleaseKind is floating point
		/// @param ratio - The ratio of the processing curve
		/// @param threshold - The threshold at which to begin processing, in Decibels
		/// @param kneeWidth - The knee width, in Decibels
		/// @param sampleRate - The sample rate, in Hertz
		DynamicsState(AttackKind attack,
					  ReleaseKind release,
					  FloatType ratio,
					  Decibels threshold,
					  Decibels kneeWidth,
					  Hertz sampleRate) noexcept
			: mAttack(attack), mRelease(release), mRatio(ratio), mThreshold(threshold),
			  mKneeWidth(kneeWidth), mSampleRate(sampleRate) {
		}

		/// @brief Copy onstructs a `DynamicsState` from the given one
		///
		/// @param state - The `DynamicsState` to copy
		DynamicsState(const DynamicsState& state) noexcept = default;

		/// @brief Move constructs the given `DynamicsState`
		///
		/// @param state - The `DynamicsState` to move
		DynamicsState(DynamicsState&& state) noexcept = default;

		~DynamicsState() noexcept = default;

		/// @brief Sets the attack to the given value
		///
		/// @param attack - The new attack
		inline auto setAttack(AttackKind attack) noexcept -> void {
			mAttack = attack;

			for(const auto& callback : mAttackCallbacks) {
				callback(mAttack);
			}
		}

		/// @brief Returns the current attack
		///
		/// @return - The attack
		[[nodiscard]] inline auto getAttack() const noexcept -> AttackKind {
			return mAttack;
		}

		/// @brief Sets the release to the given value
		///
		/// @param release - The new release
		inline auto setRelease(ReleaseKind release) noexcept -> void {
			mRelease = release;

			for(const auto& callback : mReleaseCallbacks) {
				callback(mRelease);
			}
		}

		/// @brief Returns the current release
		///
		/// @return - The release
		[[nodiscard]] inline auto getRelease() const noexcept -> ReleaseKind {
			return mRelease;
		}

		/// @brief Sets the ratio to the given value
		///
		/// @param ratio - The new ratio
		inline auto setRatio(FloatType ratio) noexcept -> void {
			mRatio = ratio;

			for(const auto& callback : mRatioCallbacks) {
				callback(mRatio);
			}
		}

		/// @brief Returns the current ratio
		///
		/// @return - The ratio
		[[nodiscard]] inline auto getRatio() const noexcept -> FloatType {
			return mRatio;
		}

		/// @brief Sets the threshold to the given value
		///
		/// @param threshold - The new threshold, in Decibels
		inline auto setThreshold(Decibels threshold) noexcept -> void {
			mThreshold = threshold;

			for(const auto& callback : mThresholdCallbacks) {
				callback(mThreshold);
			}
		}

		/// @brief Returns the current threshold
		///
		/// @return - The threshold, in Decibels
		[[nodiscard]] inline auto getThreshold() const noexcept -> Decibels {
			return mThreshold;
		}

		/// @brief Sets the knee width to the given value
		///
		/// @param kneeWidth - The new knee width, in Decibels
		inline auto setKneeWidth(Decibels kneeWidth) noexcept -> void {
			mKneeWidth = kneeWidth;

			for(const auto& callback : mKneeWidthCallbacks) {
				callback(mKneeWidth);
			}
		}

		/// @brief Returns the current knee width
		///
		/// @return - The knee width, in Decibels
		[[nodiscard]] inline auto getKneeWidth() const noexcept -> Decibels {
			return mKneeWidth;
		}

		/// @brief Sets the sample rate to the given value
		///
		/// @param sampleRate - The new sample rate, in Hertz
		inline auto setSampleRate(Hertz sampleRate) noexcept -> void {
			mSampleRate = sampleRate;

			for(const auto& callback : mSampleRateCallbacks) {
				callback(mSampleRate);
			}
		}

		/// @brief Returns the current sample rate
		///
		/// @return - The sample rate, in Hertz
		[[nodiscard]] inline auto getSampleRate() const noexcept -> Hertz {
			return mSampleRate;
		}

		/// @brief Sets whether the dynamics processor associated with this
		/// has an auto release function
		///
		/// @param hasAutoRelease - Whether this has an auto release function
		inline auto setHasAutoRelease(bool hasAutoRelease) noexcept -> void {
			mHasAutoRelease = hasAutoRelease;
			if(!mHasAutoRelease) {
				mAutoReleaseEnabled = false;
				for(const auto& callback : mAutoReleaseCallbacks) {
					callback(mAutoReleaseEnabled);
				}
			}
		}

		/// @brief Returns whether the dynamics processor associated with this
		/// has an auto release function
		///
		/// @return - Whether this has an auto release function
		[[nodiscard]] inline auto getHasAutoRelease() const noexcept -> bool {
			return mHasAutoRelease;
		}

		/// @brief Sets if auto release is enabled
		///
		/// @param enabled - Whether auto release is enabled
		inline auto setAutoReleaseEnabled(bool enabled) noexcept -> void {
			if(mHasAutoRelease) {
				mAutoReleaseEnabled = enabled;
				for(const auto& callback : mAutoReleaseCallbacks) {
					callback(mAutoReleaseEnabled);
				}
			}
		}

		/// @brief Returns whether auto release is enabled
		///
		/// @return - Whether auto release is enabled
		[[nodiscard]] inline auto getAutoReleaseEnabled() const noexcept -> bool {
			return mAutoReleaseEnabled;
		}

		/// @brief Sets the first attack coefficient to the given value
		///
		/// @param attack - The new first attack coefficient
		inline auto setAttackCoefficient1(FloatType attack) noexcept -> void {
			mAttackCoefficient1 = attack;
		}

		/// @brief Sets the second attack coefficient to the given value
		///
		/// @param attack - The new second attack coefficient
		inline auto setAttackCoefficient2(FloatType attack) noexcept -> void {
			mAttackCoefficient2 = attack;
		}

		/// @brief Sets the attack coefficients to the given value
		///
		/// @param attack1 - The new first attack coefficient
		/// @param attack2 - The new second attack coefficient
		inline auto setAttackCoefficients(FloatType attack1, FloatType attack2) noexcept -> void {
			mAttackCoefficient1 = attack1;
			mAttackCoefficient2 = attack2;
		}

		/// @brief Returns the current first attack coefficient
		///
		/// @return - The first attack coefficient
		[[nodiscard]] inline auto getAttackCoefficient1() const noexcept -> FloatType {
			return mAttackCoefficient1;
		}

		/// @brief Returns the current second attack coefficient
		///
		/// @return - The second attack coefficient
		[[nodiscard]] inline auto getAttackCoefficient2() const noexcept -> FloatType {
			return mAttackCoefficient2;
		}

		/// @brief Returns the current attack coefficients
		///
		/// @return - The first and second attack coefficients, as a std::tuple
		[[nodiscard]] inline auto
		getAttackCoefficients() const noexcept -> std::tuple<FloatType, FloatType> {
			return {mAttackCoefficient1, mAttackCoefficient2};
		}

		/// @brief Sets the first release coefficient to the given value
		///
		/// @param release - The new first release coefficient
		inline auto setReleaseCoefficient1(FloatType release) noexcept -> void {
			mReleaseCoefficient1 = release;
		}

		/// @brief Sets the second release coefficient to the given value
		///
		/// @param release - The new second release coefficient
		inline auto setReleaseCoefficient2(FloatType release) noexcept -> void {
			mReleaseCoefficient2 = release;
		}

		/// @brief Sets the release coefficients to the given values
		///
		/// @param release1 - The new first release coefficient
		/// @param release2 - The new seconds release coefficient
		inline auto
		setReleaseCoefficients(FloatType release1, FloatType release2) noexcept -> void {
			mReleaseCoefficient1 = release1;
			mReleaseCoefficient2 = release2;
		}

		/// @brief Returns the current first release coefficient
		///
		/// @return - The first release coefficient
		[[nodiscard]] inline auto getReleaseCoefficient1() const noexcept -> FloatType {
			return mReleaseCoefficient1;
		}

		/// @brief Returns the current second release coefficient
		///
		/// @return - The second release coefficient
		[[nodiscard]] inline auto getReleaseCoefficient2() const noexcept -> FloatType {
			return mReleaseCoefficient2;
		}

		/// @brief Returns the current release coefficients
		///
		/// @return  - The first and second release coefficients, as a std::tuple
		[[nodiscard]] inline auto
		getReleaseCoefficients() const noexcept -> std::tuple<FloatType, FloatType> {
			return {mReleaseCoefficient1, mReleaseCoefficient2};
		}

		/// @brief Registers the given callback to be called on changes to the given field.
		/// The callback is called immediately to allow for synchronization with the current state.
		///
		/// @tparam F - The type associated with the given field indicated by `field`,
		/// eg: for `Ratio`, this could be `float` or `double` and for `Attack` this could be
		/// `float`, `double`, or an associated `enum`, depending on what was specified at
		/// construction time. The type associated with the field and this type MUST match (failure
		/// to do this will cause a compile-time error).
		/// @tparam field - The field to register the callback on
		///
		/// @param callback - The callback to register
		template<typename F,
				 DynamicsField Field,
				 std::enable_if_t<isRegisterFuncEnabled<F, Field>(), bool> = true>
		auto registerCallback(const std::function<void(F)>& callback) noexcept -> void;

		/// @brief Registers the given callback to be called on changes to the given field.
		/// The callback is called immediately to allow for synchronization with the current state.
		template<>
		inline auto
		registerCallback<AttackKind, DynamicsField::Attack>(const AttackCallback& callback) noexcept
			-> void {
			callback(mAttack);
			mAttackCallbacks.push_back(callback);
		}

		/// @brief Registers the given callback to be called on changes to the given field.
		/// The callback is called immediately to allow for synchronization with the current state.
		template<>
		inline auto registerCallback<ReleaseKind, DynamicsField::Release>(
			const ReleaseCallback& callback) noexcept -> void {
			callback(mRelease);
			mReleaseCallbacks.push_back(callback);
		}

		/// @brief Registers the given callback to be called on changes to the given field.
		/// The callback is called immediately to allow for synchronization with the current state.
		template<>
		inline auto
		registerCallback<FloatType, DynamicsField::Ratio>(const RatioCallback& callback) noexcept
			-> void {
			callback(mRatio);
			mRatioCallbacks.push_back(callback);
		}

		/// @brief Registers the given callback to be called on changes to the given field.
		/// The callback is called immediately to allow for synchronization with the current state.
		template<>
		inline auto registerCallback<Decibels, DynamicsField::Threshold>(
			const ThresholdCallback& callback) noexcept -> void {
			callback(mThreshold);
			mThresholdCallbacks.push_back(callback);
		}

		/// @brief Registers the given callback to be called on changes to the given field.
		/// The callback is called immediately to allow for synchronization with the current state.
		template<>
		inline auto registerCallback<Decibels, DynamicsField::KneeWidth>(
			const KneeWidthCallback& callback) noexcept -> void {
			callback(mKneeWidth);
			mKneeWidthCallbacks.push_back(callback);
		}

		/// @brief Registers the given callback to be called on changes to the given field.
		/// The callback is called immediately to allow for synchronization with the current state.
		template<>
		inline auto registerCallback<Hertz, DynamicsField::SampleRate>(
			const SampleRateCallback& callback) noexcept -> void {
			callback(mSampleRate);
			mSampleRateCallbacks.push_back(callback);
		}

		/// @brief Registers the given callback to be called on changes to the given field.
		/// The callback is called immediately to allow for synchronization with the current state.
		template<>
		inline auto registerCallback<bool, DynamicsField::AutoRelease>(
			const AutoReleaseEnableCallback& callback) noexcept -> void {
			callback(mAutoReleaseEnabled);
			mAutoReleaseCallbacks.push_back(callback);
		}

		auto operator=(const DynamicsState& state) noexcept -> DynamicsState& = default;
		auto operator=(DynamicsState&& state) noexcept -> DynamicsState& = default;

	  private:
		/// State variables

		/// The attack, can be `float`, `double`, or an associated `enum`
		AttackKind mAttack = static_cast<AttackKind>(0);
		/// The release, can be `float, `double`, or an associated `enum`
		ReleaseKind mRelease = static_cast<ReleaseKind>(0);
		/// The ratio, `float` or `double`
		FloatType mRatio = narrow_cast<FloatType>(1.1);
		/// The threshold in Decibels, `float` or `double`
		Decibels mThreshold = -12.0_dB;
		/// The knee width in Decibels, `float` or `double`
		Decibels mKneeWidth = 6.0_dB;
		/// The first attack coefficient, `float` or `double`
		FloatType mAttackCoefficient1 = narrow_cast<FloatType>(0.0);
		/// The second attack coefficient, `float` or `double`
		FloatType mAttackCoefficient2 = narrow_cast<FloatType>(0.0);
		/// The first release coefficient, `float` or `double`
		FloatType mReleaseCoefficient1 = narrow_cast<FloatType>(0.0);
		/// The second release coefficient, `float` or `double`
		FloatType mReleaseCoefficient2 = narrow_cast<FloatType>(0.0);
		/// The sample rate in Hertz
		Hertz mSampleRate = 44.1_kHz;
		bool mHasAutoRelease = false;
		bool mAutoReleaseEnabled = false;

		/// Callback containers

		/// Attack callbacks
		std::vector<AttackCallback> mAttackCallbacks;
		/// Release callbacks
		std::vector<ReleaseCallback> mReleaseCallbacks;
		/// Ratio callbacks
		std::vector<RatioCallback> mRatioCallbacks;
		/// Threshold callbacks
		std::vector<ThresholdCallback> mThresholdCallbacks;
		/// Knee width callbacks
		std::vector<KneeWidthCallback> mKneeWidthCallbacks;
		/// Sample rate callbacks
		std::vector<SampleRateCallback> mSampleRateCallbacks;
		/// AutoRelease callbacks
		std::vector<AutoReleaseEnableCallback> mAutoReleaseCallbacks;

		template<typename F, DynamicsField Field>
		inline static constexpr auto isRegisterFuncEnabled() noexcept -> bool {
			if constexpr(Field == DynamicsField::Attack) {
				return std::is_same_v<F, AttackKind>;
			}
			else if constexpr(Field == DynamicsField::Release) {
				return std::is_same_v<F, ReleaseKind>;
			}
			else if constexpr(Field == DynamicsField::Ratio) {
				return std::is_same_v<F, FloatType>;
			}
			else if constexpr(Field == DynamicsField::Threshold
							  || Field == DynamicsField::KneeWidth) {
				return std::is_same_v<F, Decibels>;
			}
			else if constexpr(Field == DynamicsField::SampleRate) {
				return std::is_same_v<F, Hertz>;
			}
			else {
				return std::is_same_v<F, bool>;
			}
		}
	};
} // namespace apex::dsp

#endif // DYNAMICS_STATE
