#pragma once
#include "player.h"

extern Atlas atlas_nut_idle_left;					//奸果朝向左的默认动画图集
extern Atlas atlas_nut_idle_right;					//奸果朝向右的默认动画图集
extern Atlas atlas_nut_run_left;					//奸果朝向左的奔跑动画图集
extern Atlas atlas_nut_run_right;					//奸果朝向右的奔跑动画图集
extern Atlas atlas_nut_attack_ex_left;				//奸果朝向左的特殊攻击动画图集
extern Atlas atlas_nut_attack_ex_right;				//奸果朝向右的特殊攻击动画图集
extern Atlas atlas_nut_die_left;					//奸果朝向左的死亡动画图集
extern Atlas atlas_nut_die_right;					//奸果朝向右的死亡动画图集
extern Atlas atlas_nut_explode;						//奸果爆炸动画图集

class NutPlayer : public Player {
public:
	NutPlayer(bool facing_right = true) : Player(facing_right) {
		animation_idle_left.set_atlas(&atlas_nut_idle_left);
		animation_idle_right.set_atlas(&atlas_nut_idle_right);
		animation_run_left.set_atlas(&atlas_nut_run_left);
		animation_run_right.set_atlas(&atlas_nut_run_right);
		animation_attack_ex_left.set_atlas(&atlas_nut_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_nut_attack_ex_right);
		animation_explode.set_atlas(&atlas_nut_explode);
		animation_die_left.set_atlas(&atlas_nut_die_left);
		animation_die_right.set_atlas(&atlas_nut_die_right);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(50);
		animation_attack_ex_right.set_interval(50);
		animation_explode.set_interval(50);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);

		animation_attack_ex_left.set_loop(false);
		animation_attack_ex_right.set_loop(false);
		animation_explode.set_loop(false);
		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);

		size.x = 96, size.y = 96;
		attack_cd = 1500;

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]()
			{
				can_attack = true;
			});

		animation_attack_ex_left.set_callback([&]()
			{
				is_explode_visible = true;
				mciSendString(_T("play nut_explode from 0"), NULL, 0, NULL);
			});

		animation_attack_ex_right.set_callback([&]()
			{
				is_explode_visible = true;
				//mciSendString(_T("play nut_explode from 0"), NULL, 0, NULL);
			});

		animation_explode.set_callback([&]()
			{
				is_attacking_ex = false;
				is_explode_visible = false;
			});
	}
	~NutPlayer() = default;

	void on_input(const ExMessage& msg) {
		Player::on_input(msg);
	}

	void on_update(int delta) {
		Player::on_update(delta);

		if (is_explode_visible)
			animation_explode.on_update(delta);
	}

	void on_draw(const Camera& camera) {
		Player::on_draw(camera);

		if (is_explode_visible) {
			Vector2 explode_position;
			IMAGE* frame = animation_explode.get_frame();
			explode_position.x = position.x + (size.x - frame->getwidth()) / 2;
			explode_position.y = position.y + (size.y - frame->getheight()) / 2;
			animation_explode.on_draw(camera, (int)explode_position.x, (int)explode_position.y);
		}
	}

	void on_attack() {

	}

	void on_attack_ex() {
		is_attacking_ex = true;
		
		animation_explode.reset();
		animation_attack_ex_left.reset();
		animation_attack_ex_right.reset();

	}

private:
	Animation animation_explode;			// 奸果爆炸动画
	bool is_explode_visible = false;		// 是否显示爆炸动画

};