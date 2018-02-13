#ifndef __NUMBER_HPP__
#define __NUMBER_HPP__
#include <string>
#include <type_traits>
#include <limits>
#include <algorithm>
#include <cassert>

namespace standard {
	template<typename T, class Compare> constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) {
		return  assert(!comp(hi, lo)),
			comp(v, lo) ? lo : comp(hi, v) ? hi : v;
	}
	template<class T> constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
#ifdef _MSC_VER
		return clamp(v, lo, hi, std::less<>());
#else
		return std::clamp(v, lo, hi);
#endif
	}

	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	class number {
	private:
		T n, maximum, minimum;
	protected:
		T cmp(const T num) {
			return clamp(num, this->minimum, this->maximum);
		}
	public:
		number() = default;
		constexpr number(const T num, const T max, const T min) : n(clamp(num, min, max)), maximum(max), minimum(min) {}
		constexpr number(const T num) : number(num, std::numeric_limits<T>::max(), std::numeric_limits<T>::min()) {}
		number& operator + (const number& num) const { return number(clamp(this->n + num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator - (const number& num) const { return number(clamp(this->n - num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator * (const number& num) const { return number(clamp(this->n * num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator / (const number& num) const { return number(clamp(this->n / num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator & (const number& num) const { return number(clamp(this->n & num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator % (const number& num) const { return number(clamp(this->n % num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator | (const number& num) const { return number(clamp(this->n | num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator ^ (const number& num) const { return number(clamp(this->n ^ num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator << (const number& num) const { return number(clamp(this->n << num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator >> (const number& num) const { return number(clamp(this->n >> num.n, this->minimum, this->maximum), this->maximum, this->minimum); }
		number& operator += (const number& num) { this->n = this->cmp(this->n + num.n); return *this; }
		number& operator ++ () { this->n = this->cmp(this->n + 1); return *this; }
		number& operator -= (const number& num) { this->n = this->cmp(this->n - num.n); return *this; }
		number& operator -- () { this->n = this->cmp(this->n - 1); return *this; }
		number& operator *= (const number& num) { this->n = this->cmp(this->n = num.n); return *this; }
		number& operator /= (const number& num) { this->n = this->cmp(this->n = num.n); return *this; }
		number& operator &= (const number& num) { this->n = this->cmp(this->n & num.n); return *this; }
		number& operator %= (const number& num) { this->n = this->cmp(this->n % num.n); return *this; }
		number& operator |= (const number& num) { this->n = this->cmp(this->n | num.n); return *this; }
		number& operator <<= (const number& num) { this->n = this->cmp(this->n << num.n); return *this; }
		number& operator >>= (const number& num) { this->n = this->cmp(this->n >> num.n); return *this; }
		bool operator == (const number& num) const { return this->n == num.n; }
		bool operator != (const number& num) const { return this->n != num.n; }
		bool operator <  (const number& num) const { return this->n < num.n; }
		bool operator <= (const number& num) const { return this->n <= num.n; }
		bool operator >  (const number& num) const { return this->n > num.n; }
		bool operator >= (const number& num) const { return this->n >= num.n; }
		template<typename U, std::enable_if_t<std::is_arithmetic<U>::value, std::nullptr_t> = nullptr>
		operator number<U>() {
			return number<U>(
				this->n,
				this->maximum >= std::numeric_limits<U>::max() ? std::numeric_limits<U>::max() : this->maximum,
				this->minimum <= std::numeric_limits<U>::min() ? std::numeric_limits<U>::min() : this->minimum
				);
		}
		// 現在値を取得する
		T Get() const noexcept { return this->n; }
		// 設定されている現在の最大値を取得する
		T GetMax() const noexcept { return this->maximum; }
		// 設定されている現在の最大値を取得する
		T GetMin() const noexcept { return this->minimum; }
		// 現在値を指定された値に変更する
		void ChangeCurrentNumToReserevedNum(const T num) { this->n = num; }
		// 最大値を指定された値に変更する
		// 例外 : 引数に指定された値が現在の最小値より小さい場合、std::runtime_errorが投げられる
		void ChangeMaximumToReservedNum(const T num) {
			if (num < this->minimum) throw std::runtime_error("maximum must be larger than minimum.");
			this->maximum = num;
			this->n = this->cmp(this->n);
		}
		// 最小値を指定された値に変更する
		// 例外 : 引数に指定された値が現在の最大値より大きい場合、std::runtime_errorが投げられる
		void ChangeMinimumToReservedNum(const T num) {
			if (num > this->maximum) throw std::runtime_error("minimum must be smaller than maximum.");
			this->minimum = num;
			this->n = this->cmp(this->n);
		}
		// 最大値に指定された値を加算する
		// 例外 : 引数に指定された値が負の場合、計算することによって最大値が最小値を下回る場合、std::runtime_errorが投げられる
		void AddToMax(const T num) { this->ChangeMaximumToReservedNum(this->GetMax() + num); }
		// 最大値に指定された値を加算する
		// 例外 : 引数に指定された値が正の場合、計算することによって最小値が最大値を上回る場合、std::runtime_errorが投げられる
		void AddToMin(const T num) { this->ChangeMinimumToReservedNum(this->GetMin() + num); }
	};
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& clamp(const number<T>& v, const number<T>& lo, const number<T>& hi) { return number<T>(clamp<T>(v.Get(), lo.Get(), hi.Get())); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& clamp(const T& v, const number<T>& lo, const number<T>& hi) { return number<T>(clamp<T>(v.Get(), lo.Get(), hi.Get())); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& clamp(const number<T>& v, const T& lo, const number<T>& hi) { return number<T>(clamp<T>(v.Get(), lo.Get(), hi.Get())); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& clamp(const number<T>& v, const number<T>& lo, const T& hi) { return number<T>(clamp<T>(v.Get(), lo.Get(), hi)); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& clamp(const T& v, const T& lo, const number<T>& hi) { return number<T>(clamp<T>(v.Get(), lo.Get(), hi.Get())); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& clamp(const T& v, const number<T>& lo, const T& hi) { return number<T>(clamp<T>(v.Get(), lo.Get(), hi.Get())); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& clamp(const number<T>& v, const T& lo, const T& hi) { return number<T>(clamp<T>(v.Get(), lo.Get(), hi.Get())); }

	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator + (const T& n, const number<T>& num) { return number<T>(clamp(n + num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator - (const T& n, const number<T>& num) { return number<T>(clamp(n - num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator * (const T& n, const number<T>& num) { return number<T>(clamp(n * num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator / (const T& n, const number<T>& num) { return number<T>(clamp(n / num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator & (const T& n, const number<T>& num) { return number<T>(clamp(n & num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator % (const T& n, const number<T>& num) { return number<T>(clamp(n % num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator | (const T& n, const number<T>& num) { return number<T>(clamp(n | num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator ^ (const T& n, const number<T>& num) { return number<T>(clamp(n ^ num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator << (const T& n, const number<T>& num) { return number<T>(clamp(n << num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline number<T>& operator >> (const T& n, const number<T>& num) { return number<T>(clamp(n >> num.Get(), num.GetMin(), num.GetMax()), num.GetMax(), num.GetMin()); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline bool operator == (const T& n, const number<T>& num) { return n == num.Get(); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline bool operator != (const T& n, const number<T>& num) { return n != num.Get(); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline bool operator <  (const T& n, const number<T>& num) { return n < num.Get(); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline bool operator <= (const T& n, const number<T>& num) { return n <= num.Get(); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline bool operator >  (const T& n, const number<T>& num) { return n > num.Get(); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	inline bool operator >= (const T& n, const number<T>& num) { return n >= num.Get(); }

	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& max(const number<T>& Left, const number<T>& Right) { return number<T>(std::max(Left.Get(), Right.Get())); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& max(const number<T>& Left, const T& Right) { return number<T>(std::max(Left.Get(), Right)); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& max(const T& Left, const number<T>& Right) { return number<T>(std::max(Left, Right.Get())); }

	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& min(const number<T>& Left, const number<T>& Right) { return number<T>(std::min(Left.Get(), Right.Get())); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& min(const number<T>& Left, const T& Right) { return number<T>(std::min(Left.Get(), Right)); }
	template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, std::nullptr_t> = nullptr>
	constexpr const number<T>& min(const T& Left, const number<T>& Right) { return number<T>(std::min(Left, Right.Get())); }

	inline number<long double> abs(const number<long double> n) { return number<long double>(std::abs(n.Get())); }
	inline number<double> abs(const number<double> n) { return number<double>(std::abs(n.Get())); }
	inline number<float> abs(const number<float> n) { return number<float>(std::abs(n.Get())); }
	inline number<long long> abs(const number<long long> n) { return number<long long>(std::abs(n.Get())); }
	inline number<long> abs(const number<long> n) { return number<long>(std::abs(n.Get())); }
	inline number<int> abs(const number<int> n) { return number<int>(std::abs(n.Get())); }

	namespace {
		template<typename T, std::enable_if_t<std::is_signed<T>::value, std::nullptr_t> = nullptr>
		number<T> string_to_signed_integer(const std::string& s, size_t* Index = 0, const int Base = 10) { return number<T>(static_cast<T>(std::stoll(s, Index, Base))); }
		template<typename T, std::enable_if_t<std::is_signed<T>::value, std::nullptr_t> = nullptr>
		number<T> wstring_to_signed_integer(const std::wstring& s, size_t* Index = 0, const int Base = 10) { return number<T>(static_cast<T>(std::stoll(s, Index, Base))); }
		template<typename T, std::enable_if_t<std::is_unsigned<T>::value, std::nullptr_t> = nullptr>
		number<T> string_to_unsigned_integer(const std::string& s, size_t* Index = 0, const int Base = 10) { return number<T>(static_cast<T>(std::stoull(s, Index, Base))); }
		template<typename T, std::enable_if_t<std::is_unsigned<T>::value, std::nullptr_t> = nullptr>
		number<T> wstring_to_unsigned_integer(const std::wstring& s, size_t* Index = 0, const int Base = 10) { return number<T>(static_cast<T>(std::stoull(s, Index, Base))); }
		template<typename T, std::enable_if_t<std::is_floating_point<T>::value, std::nullptr_t> = nullptr>
		number<T> string_to_float(const std::string s, size_t* Index = 0) { return number<T>(static_cast<T>(std::stold(s, Index))); }
		template<typename T, std::enable_if_t<std::is_floating_point<T>::value, std::nullptr_t> = nullptr>
		number<T> wstring_to_float(const std::wstring s, size_t* Index = 0) { return number<T>(static_cast<T>(std::stold(s, Index))); }
	}
	inline number<int> stoi(const std::string& s, size_t* Index = 0, const int Base = 10) { return string_to_signed_integer<int>(s, Index, Base); }
	inline number<int> stoi(const std::wstring& s, size_t* Index = 0, const int Base = 10) { return wstring_to_signed_integer<int>(s, Index, Base); }
	inline number<long> stol(const std::string& s, size_t* Index = 0, const int Base = 10) { return string_to_signed_integer<long>(s, Index, Base); }
	inline number<long> stol(const std::wstring& s, size_t* Index = 0, const int Base = 10) { return wstring_to_signed_integer<long>(s, Index, Base); }
	inline number<__int64> stoll(const std::string& s, size_t* Index = 0, const int Base = 10) { return string_to_signed_integer<__int64>(s, Index, Base); }
	inline number<__int64> stoll(const std::wstring& s, size_t* Index = 0, const int Base = 10) { return wstring_to_signed_integer<__int64>(s, Index, Base); }
	inline number<unsigned int> stoui(const std::string& s, size_t* Index = 0, const int Base = 10) { return string_to_unsigned_integer<unsigned int>(s, Index, Base); }
	inline number<unsigned int> stoui(const std::wstring& s, size_t* Index = 0, const int Base = 10) { return wstring_to_unsigned_integer<unsigned int>(s, Index, Base); }
	inline number<unsigned long> stoul(const std::string& s, size_t* Index = 0, const int Base = 10) { return string_to_unsigned_integer<unsigned long>(s, Index, Base); }
	inline number<unsigned long> stoul(const std::wstring& s, size_t* Index = 0, const int Base = 10) { return wstring_to_unsigned_integer<unsigned long>(s, Index, Base); }
	inline number<unsigned __int64> stoull(const std::string& s, size_t* Index = 0, const int Base = 10) { return string_to_unsigned_integer<unsigned __int64>(s, Index, Base); }
	inline number<unsigned __int64> stoull(const std::wstring& s, size_t* Index = 0, const int Base = 10) { return wstring_to_unsigned_integer<unsigned __int64>(s, Index, Base); }
	inline number<float> stof(const std::string& s, size_t* Index = 0) { return string_to_float<float>(s, Index); }
	inline number<float> stof(const std::wstring& s, size_t* Index = 0) { return wstring_to_float<float>(s, Index); }
	inline number<double> stod(const std::string& s, size_t* Index = 0) { return string_to_float<double>(s, Index); }
	inline number<double> stod(const std::wstring& s, size_t* Index = 0) { return wstring_to_float<double>(s, Index); }
	inline number<long double> stold(const std::string& s, size_t* Index = 0) { return string_to_float<long double>(s, Index); }
	inline number<long double> stold(const std::wstring& s, size_t* Index = 0) { return wstring_to_float<long double>(s, Index); }
}
#endif
