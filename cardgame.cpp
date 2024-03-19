#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<variant>

namespace Effect {
	enum Skilleffect {
		range = 0,
		attack = 1,
		defend = 2,
		shield = 3,
		health = 4,
		speed = 5
	};
}
#define RANGE Effect::range
#define ATTACK Effect::attack
#define DEFEND Effect::defend
#define SHIELD Effect::shield
#define HEALTH Effect::health
#define SPEED Effect::speed

class Role
{
private:
	std::string position;
	int attack;
	int defend;
	int health;
	int speed;
public:
	Role(const std::string& Position, int Attack, int Defend, int Health, int Speed) :
		position(Position), attack(Attack), defend(Defend), health(Health), speed(Speed) {}
	~Role() {}

	virtual inline std::string Position() { return position; }
	virtual inline int Attack() { return attack; }
	virtual inline int Defend() { return defend; }
	virtual inline int Health() { return health; }
	virtual inline int Speed() { return speed; }

	virtual inline void Attack(int Attack) { attack = Attack; }
	virtual inline void Defend(int Defend) { defend = Defend; }
	virtual inline void Health(int Health) { health = Health; }
	virtual inline void Speed(int Speed) { speed = Speed; }

	virtual inline void showrole() {
		std::cout << "职业: " << position << std::endl;
		std::cout << "攻击: " << attack << std::endl;
		std::cout << "防御力: " << defend << std::endl;
		std::cout << "生命值: " << health << std::endl;
		std::cout << "速度: " << speed << std::endl;
	}

	virtual inline void show() = 0;

};

class Skill
{
private:
	std::string skillname;
	std::string description;
	std::vector<std::pair<Effect::Skilleffect, int>> skilleffect;

public:
	template<typename... Args>
	Skill(const std::string& Skillname, const std::string& Description, Args&&... args) :
		skillname(Skillname), description(Description) {
		(void)std::initializer_list<int>{
			((void)skilleffect.emplace_back(std::forward<Args>(args).first, std::forward<Args>(args).second), 0)...
		};
	}
	~Skill() {}

	virtual inline std::string Skillname() { return skillname; }
	virtual inline std::string Description() { return description; }
	virtual inline void showeffect() {
		for (const auto& [effect, value] : skilleffect) {
			std::cout << effect << ": " << value << std::endl;
		}
	}
	virtual inline void showskill() {
		std::cout << std::endl;
		std::cout << skillname << ": " << std::endl;
		std::cout << "详细描述: " << description << std::endl;
		std::cout << "技能效果: " << std::endl;
		showeffect();
	}
};

class Mage :private Role
{
private:
	Skill normal_attack;
	Skill combat_skill;
	Skill finishing_skill;

public:
	Mage() :Role("法师", 8, 2, 25, 10),
		normal_attack("法球攻击", "对1个目标造成8点伤害", std::make_pair(ATTACK, 8), std::make_pair(RANGE, 1)),
		combat_skill("法力崩坏", "对3个目标造成6点伤害", std::make_pair(ATTACK, 6), std::make_pair(RANGE, 3)),
		finishing_skill("致命火花", "对一个目标造成15点伤害", std::make_pair(ATTACK, 15), std::make_pair(RANGE, 1)) {}
	void show() override {
		showrole();
		normal_attack.showskill();
		combat_skill.showskill();
		finishing_skill.showskill();
	}
	~Mage() {}

};

class Knight :private Role
{
private:
	Skill normal_attack;
	Skill combat_skill;
	Skill finishing_skill;

public:
	Knight() :Role("骑士", 5, 6, 40, 5),
		normal_attack("骑士剑术", "对1个目标造成5点伤害", std::make_pair(ATTACK, 5), std::make_pair(RANGE, 1)),
		combat_skill("绝对守护", "对1个目标增加8点护盾（护盾不可叠加）", std::make_pair(SHIELD, 8), std::make_pair(RANGE, 1)),
		finishing_skill("皇家意志", "对友军全体防御提高12点", std::make_pair(ATTACK, 15), std::make_pair(RANGE, 1)) {}
	void show() override{
		showrole();
		normal_attack.showskill();
		combat_skill.showskill();
		finishing_skill.showskill();
	}
	~Knight() {}
};

using AllRole = std::variant<std::unique_ptr<Knight>, std::unique_ptr<Mage>>;
class Battle
{
	std::vector<AllRole> FriendSide;
	std::vector<AllRole> EnemySide;
public:
	Battle() {};
	~Battle() {}

	template<typename T, typename... Args>
	void AddFriend(Args&&... args);

	template<typename T, typename... Args>
	void AddEnemy(Args&&... args);

	void showFriend();
	void showEnemy();

};

template<typename T, typename... Args>
void Battle::AddFriend(Args&&... args) {
	(void)std::initializer_list<int>{
		(FriendSide.emplace_back(std::make_unique<T>(std::forward<Args>(args))), 0)...
	};
}

template<typename T, typename... Args>
void Battle::AddEnemy(Args&&... args) {
	(void)std::initializer_list<int>{
		(EnemySide.emplace_back(std::make_unique<T>(std::forward<Args>(args))), 0)...
	};
}

void Battle::showFriend()
{
	for (const auto& role : FriendSide) {
		std::visit([](const auto& ptr) {
			if (ptr) { // 检查指针是否非空  
				ptr->show();
			}
			}, role);
	}
}

void Battle::showEnemy()
{
	for (const auto& role : EnemySide) {
		std::visit([](const auto& ptr) {
			if (ptr) {
				ptr->show();
			}
		}, role);
	}
}

int main() {
	using namespace std;
	Mage mage;
	Knight knight;
	//mage.show();
	cout << endl;
	//knight.show();
	Battle battle;
	battle.AddEnemy<Mage>(mage);
	battle.AddEnemy<Knight>(knight);
	battle.AddFriend<Knight>(knight);
	battle.AddFriend<Mage>(mage);
	battle.showFriend();
	battle.showEnemy();
	return 0;
}
