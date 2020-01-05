
#include "Keys.hpp"

namespace yare {

	//https://stackoverflow.com/questions/55573238/how-do-i-do-a-proper-input-class-in-glfw-for-a-game-engine
	enum class Event 
	{
		Release = 0, Press, Repeat 
	};

	struct MouseEvent
	{
		int x, y;
		Event event;
	};
	
	struct KeyEvent
	{
		Key key;
		Event event;
	};

	class  Input
	{
	public:
		static Input * Create();

		void onMouseEvent(const MouseEvent & mouse);
		void onKeyEvent(const KeyEvent & key);


	};

}