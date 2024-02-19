#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>

const float DEFAULT_SPEED = 90.0f;
const float FRIGHTENED_SPEED = 65.0f;
const float DEAD_SPEED = 125.0f;

const float CLYDE_SCARE = 150.0f;
const float PINKY_OFFSET = 80.0f;
const float INKY_OFFSET = 40.0f;

class GhostAI : public Component
{
public:
	// Used to track the four different GhostAI states
	enum State
	{
		Scatter,
		Chase,
		Frightened,
		Dead
	};

	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;

	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);

	// Get the current state
	State GetState() const { return mState; }

	// Called when the ghost should switch to the "Frightened" state
	void Frighten();

	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current goal, prev, and next
	void DebugDrawPath(struct SDL_Renderer* render);

private:
	// Member data for pathfinding

	// TargetNode is our current goal node
	class PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	class PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying to get to
	class PathNode* mNextNode = nullptr;

	Vector2 mDirection;

	// Current state of the Ghost AI
	State mState = Scatter;

	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;

	//changes how the ghost moves
	void ChangeState(PathNode* node);

	//set a new target for the ghost
	void SetNewTarget();

	//finds next node for pathing
	void FindNextNode(bool useGhostNodes);

	//picks a random node for frightened state
	void PickRandomTarget(int resets);

	//dictates how Blinky moves
	void BlinkyMove();

	//dictates how Pinky moves
	void PinkyMove();

	//dictates how Inky moves
	void InkyMove();

	//dictates how Cylde moves
	void ClydeMove();

	//changes the ghosts look
	void SetTexture();

	//finds closest node to given location
	PathNode* ClosestNode(Vector2 position);

	//how long a ghost have been in a certain state
	float mTime = 0.0f;

	float mStateSpeed = DEFAULT_SPEED;
};
