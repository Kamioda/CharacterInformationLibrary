#ifndef __LEVELMANAGER_HPP__
#define __LEVELMANAGER_HPP__
#include "PossibleChangeStatus.hpp"
#include <vector>

class LevelManager {
private:
	std::vector<size_t> LevelUpBorderPointList;
	PossibleChangeStatus<size_t> Exp;
	PossibleChangeStatus<unsigned int> Level;
public:
	LevelManager() = default;
	/*
	第１引数 : レベル２に上がるのに必要な経験値から始まる、各レベルに上がるために必要な合計経験値のリスト
	第２引数 : 現在の経験値
	*/
	LevelManager(const std::vector<size_t> LevelUpBorderPointList, const size_t CurrentExp = 0)
		: LevelUpBorderPointList(LevelUpBorderPointList), Exp({ CurrentExp, LevelUpBorderPointList.back() }),
		Level({ 1, static_cast<unsigned int>(LevelUpBorderPointList.size() + 1), 1 }) {
		for (auto& i : this->LevelUpBorderPointList) {
			if (this->Exp < i) break;
			this->Level++;
		}
	}
	// 現在の経験値を取得する
	const size_t GetCurrentExp() const { return *this->Exp; }
	// 現在のレベルを取得する
	const unsigned int GetCurrentLevel() const { return *this->Level; }
	// 次のレベルに上がるために必要な経験値を取得する
	const size_t GetExpPointNeededToRaiseNextLevel() const { return this->LevelUpBorderPointList[*this->Level - 2] - *this->Exp; }
	// 経験値を加算する。
	// 戻り値 : 上がったレベル
	unsigned int AddExp(const size_t AddExpPoint) {
		this->Exp += AddExpPoint;
		const PossibleChangeStatus<unsigned int> Before = this->Level;
		if (this->Exp >= this->LevelUpBorderPointList[*this->Level - 2]) this->Level++;
		return *this->Level - *Before;
	}
};
#endif
