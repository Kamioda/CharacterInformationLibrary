# CharacterInformationLibrary
対戦ゲームやRPG等でパラメーターや魔法、特技等、キャラクターに関する情報を管理を行うライブラリです。

## どんなクラス・構造体があるの？
- PossibleChangeStatus(PossibleChangeStatus.hpp)

ＨＰやＭＰ等の演算管理を行うクラス

- LevelManager(LevelManager.hpp)

経験値及びレベルの演算管理を行うクラス

- Element(Element.hpp)

属性を管理するクラス。ダメージ倍率の演算も行える

- ElementInfo(Element.hpp)

属性(enum class)

- Skill(Skill.hpp)

魔法、特技の情報を管理する構造体
