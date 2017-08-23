#ifndef __ELEMENT_HPP__
#define __ELEMENT_HPP__
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

enum class ElementInfo : size_t { Normal = 0, Fire = 1, Ice = 2, Thunder = 3, Earth = 4, Wind = 5, Shine = 6, Dark = 7 };

class AdvantageInfo {
private:
	std::vector<ElementInfo> WeakElement;
	std::vector<ElementInfo> BeautyElement;
	AdvantageInfo(const std::vector<ElementInfo> Weak, const std::vector<ElementInfo> Beauty) : WeakElement(Weak), BeautyElement(Beauty) {}
public:
	AdvantageInfo(const ElementInfo Element) {
		switch (Element) {
			case ElementInfo::Fire:
				AdvantageInfo({ ElementInfo::Ice, ElementInfo::Earth }, { ElementInfo::Fire, ElementInfo::Thunder, ElementInfo::Wind });
				break;
			case ElementInfo::Ice:
				AdvantageInfo({ ElementInfo::Thunder, ElementInfo::Wind }, { ElementInfo::Fire, ElementInfo::Ice, ElementInfo::Earth });
				break;
			case ElementInfo::Thunder:
				AdvantageInfo({ ElementInfo::Fire, ElementInfo::Earth }, { ElementInfo::Ice, ElementInfo::Thunder, ElementInfo::Wind });
				break;
			case ElementInfo::Earth:
				AdvantageInfo({ ElementInfo::Ice, ElementInfo::Wind }, { ElementInfo::Fire, ElementInfo::Thunder, ElementInfo::Earth });
				break;
			case ElementInfo::Wind:
				AdvantageInfo({ ElementInfo::Fire, ElementInfo::Thunder }, { ElementInfo::Ice, ElementInfo::Earth, ElementInfo::Wind });
				break;
			case ElementInfo::Shine:
				AdvantageInfo({ ElementInfo::Dark }, { ElementInfo::Shine });
				break;
			case ElementInfo::Dark:
				AdvantageInfo({ ElementInfo::Shine }, { ElementInfo::Dark });
				break;
			default:
				throw std::runtime_error("入力された属性はこのクラスでは相性判定できません。");
		 }
	}
	// 第１引数：攻撃属性
	// 第２引数：強みである属性による攻撃の場合のダメージ倍率
	// 第３引数：弱点である属性による攻撃の場合のダメージ倍率
	// 戻り値　：ダメージ倍率
	float GetAdvantage(const ElementInfo AttackElement, const float DmgMgnfctByBtAttack, const float DmgMgnfctByWkAttack) const {
		return this->IsWeakElement(AttackElement) ? 2.0f : (this->IsBeautyElement(AttackElement) ? 0.5f : 1.0f);
	}
	// 引数：攻撃属性
	bool IsWeakElement(const ElementInfo AttackElement) const {
		return std::any_of(this->WeakElement.begin(), this->WeakElement.end(), [AttackElement](const ElementInfo Elem) { return AttackElement == Elem; });
	}
	// 引数：攻撃属性
	bool IsBeautyElement(const ElementInfo AttackElement) const {
		return std::any_of(this->BeautyElement.begin(), this->BeautyElement.end(), [AttackElement](const ElementInfo Elem) { return AttackElement == Elem; });
	}
};

class Element {
private:
	ElementInfo ElementCast(const std::string Element) noexcept {
		// ユーザーが属性を自由に追加できるようにstd::vectorクラスにしてあるだけ
		static const std::vector<std::string> ElementList = { "normal", "ice", "thunder", "earth", "wind", "shine", "dark" };
		for (size_t i = 0; i < ElementList.size(); i++) if (Element == ElementList[i]) return static_cast<ElementInfo>(i);
		return ElementInfo::Normal;
	}
	ElementInfo ElementCast(const std::wstring Element) noexcept {
		static const std::vector<std::wstring> ElementList = { L"normal", L"ice", L"thunder", L"earth", L"wind", L"shine", L"dark" };
		for (size_t i = 0; i < ElementList.size(); i++) if (Element == ElementList[i]) return static_cast<ElementInfo>(i);
		return ElementInfo::Normal;
	}
public:
	Element() : Element(ElementInfo::Normal) {}
	Element(const ElementInfo Elem) : Elem(Elem) {}
	Element(const std::string Elem) : Element(this->ElementCast(Elem)) {}
	Element(const std::wstring Elem) : Element(this->ElementCast(Elem)) {}
	ElementInfo Elem;
	float Advantage(const ElementInfo AttackElement, connst float DmgMgnfctByBtAttack, const float DmgMgnfctByWkAttack) const {
		return (this->Elem == ElementInfo::Normal || AttackElement == ElementInfo::Normal)
			? 1.0f : AdvantageInfo(this->Elem).GetAdvantage(AttackElement, DmgMgnfctByBtAttack, DmgMgnfctByWkAttack);
	}
};
#endif
