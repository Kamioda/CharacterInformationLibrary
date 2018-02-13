#ifndef __USEDAMAGECALCULATIONPARAMETER_HPP__
#define __USEDAMAGECALCULATIONPARAMETER_HPP__
#include "Number.hpp"

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
class UseDamageCalculationParameter : public standard::number<T> {
private:
	T DefaultParameter;
public:
	UseDamageCalculationParameter() : standard::number<T>() {}
	constexpr UseDamageCalculationParameter(const T DefaultNum)
		: standard::number<T>(DefaultNum), DefaultParameter(this->cmp(DefaultNum)) {}
	constexpr UseDamageCalculationParameter(const T DefaultNum, const T Max, const T Min)
		: standard::number<T>(DefaultNum, Max, Min), DefaultParameter(this->cmp(DefaultNum)) {}
	// 上下したパラメーターを元に戻す
	void Reset() { this->ChangeCurrentNumToReserevedNum(this->DefaultParameter); }
	// パラメーターの上昇
	// 引　数 : パラメーターの加算値
	// 戻り値 : 実際に上がった値
	T PowerUp(const T AddNum) {
		const T Before = this->Get();
		this->operator+=(AddNum); 
		return this->Get() - Before;
	}
	// パラメーターの下降
	// 引　数 : パラメーターの加算値
	// 戻り値 : 実際に下がった値
	T PowerDown(const T SubtractNum) {
		const T Before = this->Get();
		this->operator-=(SubtractNum);
		return Before - this->Get();
	}
	// 現在値を取得する
	T operator * () const noexcept { return this->Get(); }
};
#endif
