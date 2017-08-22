#ifndef __POSSIBLECHANGESTATUS_HPP__
#define __POSSIBLECHANGESTATUS_HPP__
#ifdef _MINWINDEF_ // Windows.h等がこのヘッダーより先にincludeされると、NOMINMAXが定義されてても効果がないためここでundefする
#undef max
#undef min
#endif
#include <type_traits>
#include <algorithm>

inline float ChangeTwoDigits(const double Data) {
	return static_cast<float>(static_cast<int>((Data + 0.005) * 100)) / 100;
}

//C++17
template<typename T> const T& clamp(const T& v, const T& lo, const T& hi) {
	return assert(!comp(hi, lo)),
		(v < lo) ? lo : (hi < v) ? hi : v;
}

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr> class PossibleChangeStatus {
private:
	T MinStatus;
public:
	//public member variable
	T CurrentStatus, MaxStatus;

	PossibleChangeStatus() = default;
	PossibleChangeStatus(const PossibleChangeStatus&) = default;
	PossibleChangeStatus& operator=(const PossibleChangeStatus&) = default;
	PossibleChangeStatus(const T Max) : PossibleChangeStatus(Max, Max) {}
	PossibleChangeStatus(const T Current, const T Max, const T Min = 0)
		: MinStatus(Min), CurrentStatus(Current), MaxStatus(Max) {}


	//public member function
	PossibleChangeStatus operator+(const T Num) {
		return{
			clamp(this->CurrentStatus + Num, this->MinStatus, this->MaxStatus),
			this->MaxStatus, this->MinStatus
		};
	}
	PossibleChangeStatus operator-(const T Num) {
		return{
			clamp(this->CurrentStatus - Num, this->MinStatus, this->MaxStatus),
			this->MaxStatus, this->MinStatus
		};
	}
	PossibleChangeStatus operator*(const T Num) {
		return{
			clamp(this->CurrentStatus * Num, this->MinStatus, this->MaxStatus),
			this->MaxStatus, this->MinStatus
		};
	}
	PossibleChangeStatus operator/(const T Num) {
		return{
			clamp(this->CurrentStatus / Num, this->MinStatus, this->MaxStatus),
			this->MaxStatus, this->MinStatus
		};
	}
	PossibleChangeStatus& operator += (const T Num) {
		this->CurrentStatus = clamp(this->CurrentStatus + Num, this->MinStatus, this->MaxStatus);
		return *this;
	}
	PossibleChangeStatus& operator -= (const T Num) {
		this->CurrentStatus = clamp(this->CurrentStatus - Num, this->MinStatus, this->MaxStatus);
		return *this;
	}
	PossibleChangeStatus& operator *= (const T Num) {
		this->CurrentStatus = clamp(this->CurrentStatus * Num, this->MinStatus, this->MaxStatus);
		return *this;
	}
	PossibleChangeStatus& operator /= (const T Num) {
		this->CurrentStatus = clamp(this->CurrentStatus / Num, this->MinStatus, this->MaxStatus);
		return *this;
	}
	PossibleChangeStatus& operator++() {
		this->CurrentStatus = std::min(this->CurrentStatus + 1, this->MaxStatus);
		return *this;
	}
	PossibleChangeStatus operator++(int) {
		const auto re = *this;
		++(*this);
		return re;
	}
	PossibleChangeStatus& operator--() {
		this->CurrentStatus = std::max(this->CurrentStatus - 1, this->MinStatus);
		return *this;
	}
	PossibleChangeStatus operator--(int) {
		const auto re = *this;
		--(*this);
		return re;
	}
	T& operator * () { return this->CurrentStatus; }
	const T& operator * () const { return this->CurrentStatus; }
	T& get() { return this->CurrentStatus; }
	const T& get() const { return this->CurrentStatus; }
	bool operator <  (const T CompareNum) const { return this->CurrentStatus < CompareNum; }
	bool operator <= (const T CompareNum) const { return this->CurrentStatus <= CompareNum; }
	bool operator >  (const T CompareNum) const { return this->CurrentStatus > CompareNum; }
	bool operator >= (const T CompareNum) const { return this->CurrentStatus >= CompareNum; }
	bool IsMin() const { return this->CurrentStatus == this->MinStatus; }
	bool IsMax() const { return this->CurrentStatus == this->MaxStatus; }
	float GetProportion() const { 
		const float Prportion = ChangeTwoDigits(
			static_cast<double>(this->CurrentStatus) * 100 / static_cast<double>(this->MaxStatus - this->MinStatus)
		);
		return (Proportion == 0.00f && !this->IsMin())
			? 0.01 
			: (Proportion == 100.00f && !this->IsMax()
				? 99.99 : Proportion;
	}
	void FullCharge() { this->CurrentStatus = this->MaxStatus; }
	void AddToMax(const T AddPoint) {
		this->MaxStatus += AddPoint;
		if (this->CurrentStatus > this->MaxStatus) this->CurrentStatus = this->MaxStatus; // AddPointに負の値が渡された時を想定しての処理
	}
	void AddToMin(const T AddPoint) { 
		this->MinStatus += AddPoint; 
		if (this->CurrentStatus < this->MinStatus) this->CurrentStatus = this->MinStatus;
	}
};

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
inline bool operator==(T l, const PossibleChangeStatus<T>& r) { return l == r.CurrentStatus; }

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
inline bool operator==(const PossibleChangeStatus<T>& l, T r) { return r == l; }

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
inline bool operator!=(T l, const PossibleChangeStatus<T>& r) { return !(l == r); }

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
inline bool operator!=(const PossibleChangeStatus<T>& l, T r) { return r != l; }
#endif
