#ifndef __SPEEDMANAGER_HPP__
#define __SPEEDMANAGER_HPP__
#include "UseDamageCalculationParameter.hpp"
#include <random>

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
class SpeedManager : public UseDamageCalculationParameter<T> {
public:
	SpeedManager() : UseDamageCalculationParameter<T>() {}
	SpeedManager(const T DefaultNum) : UseDamageCalculationParameter<T>(DefaultNum) {}
	SpeedManager(const T DefaultNum, const T Max, const T Min) : UseDamageCalculationParameter<T>(DefaultNum, Max, Min) {}
	T GetParameterToCreateAttackTurn(std::mt19937& RandEngine, const T MaxAddPoint, const T MinAddPoint) const {
		std::uniform_int_distribution<T> rand(MinAddPoint, MaxAddPoint);
		return this->Get() + rand(RandEngine);
	}
};
#endif
