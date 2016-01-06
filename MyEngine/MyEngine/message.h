#ifndef MESSAGE
#define MESSAGE

enum Message_Type {
	M_COLLISION,
	M_STATE,
	M_DEBUGDRAW,
	M_RENDERTEXT
};

class message {
public:
	message(Message_Type MT) :MType(MT) {};
	void broadcast();
	virtual ~message() {};

	Message_Type MType;
};

#endif