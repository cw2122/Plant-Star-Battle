#pragma once
#include "vector2.h"
#include "player_id.h"
#include "camera.h"

#include <functional>
#include <graphics.h>

extern bool is_debug;

class Bullet {
public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int val) {
		damage = val;
	}

	int get_damage() {
		return damage;
	}

	void set_position(float x, float y) {
		position.x = x, position.y = y;
	}

	const Vector2& get_position() const {
		return position;
	}

	const Vector2& get_size() const {
		return size;
	}

	void set_velocity(float x, float y) {
		velocity.x = x, velocity.y = y;
	}

	void set_collide_target(PlayerID target) {
		target_id = target;
	}

	PlayerID get_collide_target() const {
		return target_id;
	}

	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}

	void set_valid(bool flag) {
		valid = flag;
	}

	bool get_valid() const {
		return valid;
	}

	void set_is_attacking_ex_spawn(bool flag) {
		is_attacking_ex_spawn = flag;
	}

	void set_is_center_collision(bool flag) {
		is_center_collision = flag;
	}
	
	bool check_can_remove() const {
		return can_remove;
	}

	virtual void on_collide() {
		if (!is_attacking_ex_spawn && callback)
			callback();
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size, bool is_center_collision) {
		if (is_center_collision) {
			return this->position.x + this->size.x / 2 >= position.x
				&& this->position.x + this->size.x / 2 <= position.x + size.x
				&& this->position.y + this->size.y / 2 >= position.y
				&& this->position.y + this->size.y / 2 <= position.y + size.y;
		}
		else {
			return (max(this->position.x + this->size.x, position.x + size.x) -
				min(this->position.x, position.x) < this->size.x + size.x) &&
				(max(this->position.y + this->size.y, position.y + size.y) -
				min(this->position.y, position.y) < this->size.y + size.y);
		}
 	}

	virtual void on_update(int delta) {

	}

	virtual void on_draw(const Camera& camera) const {
		if (is_debug) {
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			rectangle((int)position.x, (int)position.y,
				(int)(position.x + size.x), (int)(position.y + size.y));
			solidcircle((int)(position.x + size.x / 2), (int)(position.y + size.y / 2), 5);
		}
	}

public:
	bool is_center_collision;				// �ӵ��Ƿ���������ײ

protected:
	Vector2 size;							// �ӵ��ߴ�
	Vector2 position;						// �ӵ�λ��
	Vector2 velocity;						// �ӵ��ٶ�
	int damage = 10;						// �ӵ��˺�
	bool valid = true;						// �ӵ��Ƿ���Ч
	bool can_remove = false;				// �ӵ��Ƿ���Ա��Ƴ�
	std::function<void()> callback;			// �ӵ���ײ�ص�����
	PlayerID target_id = PlayerID::P1;		// �ӵ���ײĿ�����ID
	bool is_attacking_ex_spawn = false;		// �ӵ��Ƿ������⹥��������

protected:
	bool check_if_exceeds_screen() {
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.y <= 0 || position.y >= getheight());
	}

};