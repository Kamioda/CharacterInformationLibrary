# CharacterInformationLibrary
対戦ゲームやRPG等でパラメーターや魔法、特技等、キャラクターに関する情報の管理を行うライブラリです。

## どんなクラス・構造体があるの？
- PossibleChangeStatus(PossibleChangeStatus.hpp)

ＨＰやＭＰ等のパラメーターの演算管理を行うクラス

- LevelManager(LevelManager.hpp)

経験値及びレベルの演算管理を行うクラス

- Element(Element.hpp)

属性を管理するクラス。ダメージ倍率の演算も行える

- ElementInfo(Element.hpp)

属性(enum class)

- AdvantageInfo(Element.hpp)

属性相性の判定を行うクラス。ダメージ倍率の演算も行えるが、属性の管理は行えない

- Skill(Skill.hpp)

魔法、特技の情報を管理する構造体
