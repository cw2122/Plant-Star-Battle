#pragma once
#include "bullet.h"
#include "animation.h"
#include "player.h"

extern Atlas atlas_bubbles;				//ÆÕÍ¨¹¥»÷ÅÝÄ­¶¯»­Í¼¼¯

extern Player* player_1;
extern Player* player_2;

class BubblesBullet : public Bullet {
public:
	BubblesBullet() {
		size.x = 150, size.y = 150;

		damage = 20;

		animation_attack.set_atlas(&atlas_bubbles);
		animation_attack.set_interval(75);
		animation_attack.set_loop(false);
		animation_attack.set_callback([&]() {can_remove = true; });

		
		IMAGE* frame_attack = animation_attack.get_frame();
		attack_render_offset.x = (frame_attack->getwidth() - size.x) / 2.0f;
		attack_render_offset.y = (frame_attack->getheight() - size.y) / 2.0f;
	}
	~BubblesBullet() = default;

	void on_collide() {
		Bullet::on_collide();
	}

	void on_update(int delta) {

		const Vector2& position_player_1 = player_1->get_position();
		const Vector2& position_player_2 = player_2->get_position();
		const Vector2& size_player_1 = player_1->get_size();
		const Vector2& size_player_2 = player_2->get_size();

		switch (target_id)
		{
		case PlayerID::P1:
			position.x = position_player_2.x + (size_player_2.x - size.x) / 2;
			position.y = position_player_2.y + (size_player_2.y - size.y) / 2;
			break;
		case PlayerID::P2:
			position.x = position_player_1.x + (size_player_1.x - size.x) / 2;
			position.y = position_player_1.y + (size_player_1.y - size.y) / 2;
			break;
		default:
			break;
		}

		animation_attack.on_update(delta);
	}

	void on_draw(const Camera& camera) const {

		animation_attack.on_draw(camera, (int)(position.x - attack_render_offset.x), (int)(position.y - attack_render_offset.y));

		Bullet::on_draw(camera);
	}

private:
	Animation animation_attack;			//ÅÝÄ­×Óµ¯·¢Éä¶¯»­
	Vector2 attack_render_offset;		//ÅÝÄ­×Óµ¯äÖÈ¾Æ«ÒÆ

};