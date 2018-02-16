#ifndef __POSSIBLECHANGESTATUS_HPP__
#define __POSSIBLECHANGESTATUS_HPP__
#include "Number.hpp"

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
class PossibleChangeStatus : public standard::number<T> {
public:
	PossibleChangeStatus() : standard::number<T>() {}
	constexpr PossibleChangeStatus(const T Max)
		: PossibleChangeStatus(Max, Max) {}
	constexpr PossibleChangeStatus(const T Current, const T MaxStatus, const T MinStatus = 0)
		: standard::number<T>(Current, MaxStatus, MinStatus) {}
	// 最小値であるかを判定する
	bool IsMin() const noexcept { return this->GetMin() == this->Get(); }
	// 最大値であるかを判定する
	bool IsMax() const noexcept { return this->GetMax() == this->Get(); }
	// 現在値を取得する
	T operator * () const noexcept { return this->Get(); }
	// 割合を取得する
	float GetRatio() const noexcept { return static_cast<float>(static_cast<double>(this->Get()) / static_cast<double>((this->GetMax() - this->GetMin()))); }
};
#endif
