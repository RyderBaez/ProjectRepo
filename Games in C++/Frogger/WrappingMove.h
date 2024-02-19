#include "MoveComponent.h"
#include <SDL2/SDL_stdinc.h>
#include "Math.h"
class WrappingMove : public MoveComponent
{
public:
	WrappingMove(class Actor* owner);
	void Update(float deltaTime) override;
	void SetDirection(Vector2 direction);
	Vector2 GetDirection() const;

private:
	Vector2 mDirection;
};