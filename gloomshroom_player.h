#pragma once
#include "player.h"
#include "bubbles_bullet.h"
#include "bubbles_bullet_ex.h"

extern Atlas atlas_gloomshroom_idle_left;			//开朗菇朝向左的默认动画图集
extern Atlas atlas_gloomshroom_idle_right;			//开朗菇朝向右的默认动画图集
extern Atlas atlas_gloomshroom_run_left;			//开朗菇朝向左的奔跑动画图集
extern Atlas atlas_gloomshroom_run_right;			//开朗菇朝向右的奔跑动画图集
extern Atlas atlas_gloomshroom_attack_ex_left;		//开朗菇朝向左的特殊攻击动画图集
extern Atlas atlas_gloomshroom_attack_ex_right;		//开朗菇朝向右的特殊攻击动画图集
extern Atlas atlas_gloomshroom_die_left;			//开朗菇朝向左的死亡动画图集
extern Atlas atlas_gloomshroom_die_right;			//开朗菇朝向右的死亡动画图集

class GloomShroomPlayer : public Player {
public:
	GloomShroomPlayer(bool facing_right = true) : Player(facing_right) {
		animation_idle_left.set_atlas(&atlas_gloomshroom_idle_left);
		animation_idle_right.set_atlas(&atlas_gloomshroom_idle_right);
		animation_run_left.set_atlas(&atlas_gloomshroom_run_left);
		animation_run_right.set_atlas(&atlas_gloomshroom_run_right);
		animation_attack_ex_left.set_atlas(&atlas_gloomshroom_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_gloomshroom_attack_ex_right);
		animation_die_left.set_atlas(&atlas_gloomshroom_die_left);
		animation_die_right.set_atlas(&atlas_gloomshroom_die_right);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);

		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);

		size.x = 96, size.y = 96;
		attack_cd = 1000;

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]()
			{
				can_attack = true;
			});

		timer_attack_ex.set_wait_time(attack_ex_duration);
		timer_attack_ex.set_one_shot(true);
		timer_attack_ex.set_callback([&]()
			{
				is_attacking_ex = false;
				can_attack = true;
			});

		timer_spwan_bubbles_ex.set_wait_time(400);
		timer_spwan_bubbles_ex.set_callback([&]()
			{
				spawn_bubbles_bullet(true);
			});

		attack_cd = 1000;
	}
	~GloomShroomPlayer() = default;

	void on_input(const ExMessage& msg) {
		Player::on_input(msg);
	}

	void on_update(int delta) {
		Player::on_update(delta);

		if (is_attacking_ex) {
			timer_attack_ex.on_update(delta);
			timer_spwan_bubbles_ex.on_update(delta);
		}
	}

	void on_draw(const Camera& camera) {
		Player::on_draw(camera);
	}

	void on_attack() {
		spawn_bubbles_bullet(false);

		mciSendString(_T("play bubbles_shot from 0"), NULL, 0, NULL);
	}

	void on_attack_ex() {
		is_attacking_ex = true;
		can_attack = false;
		timer_attack_ex.restart();
		timer_spwan_bubbles_ex.restart();

		spawn_bubbles_bullet(true);

		animation_attack_ex_right.reset();
		animation_attack_ex_left.reset();

		mciSendString(_T("play bubbles_shot_ex from 0"), NULL, 0, NULL);
	}

private:
	void spawn_bubbles_bullet(bool is_bullet_ex) {
		Bullet* bullet = nullptr;
		if (is_bullet_ex)
			bullet = new BubblesBulletEx();
		else
			bullet = new BubblesBullet();

		Vector2 bullet_position;
		const Vector2& bullet_size = bullet->get_size();
		bullet_position.x = position.x + (size.x - bullet_size.x) / 2;
		bullet_position.y = position.y + (size.y - bullet_size.y) / 2;

		bullet->set_is_attacking_ex_spawn(is_bullet_ex);
		bullet->set_is_center_collision(false);

		bullet->set_position(bullet_position.x, bullet_position.y);

		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);

		bullet->set_callback([&]() {
				mp += 20;					// 每次普通命中增加1/5能量 
			});

		bullet_list.push_back(bullet);

	}

private:
	const int attack_ex_duration = 1600;		// 特殊攻击状态持续时长

private:
	Timer timer_attack_ex;						// 特殊攻击状态定时器
	Timer timer_spwan_bubbles_ex;				// 泡沫子弹发射定时器



};