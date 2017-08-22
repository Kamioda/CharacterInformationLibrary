#ifndef __SKILL_HPP__
#define __SKILL_HPP__
#include "Element.hpp"

struct Skill {
	std::string Name;			// 名前
	int UseMP;					// 消費MP
	int BasePower;				// 基本攻撃力
	std::string Description;	// 説明
	ElementInfo SkillElement;	// 属性(enum class値)
};
#endif
