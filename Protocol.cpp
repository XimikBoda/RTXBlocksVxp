#include "Protocol.h"
#include "Sock.h"

// | packet len 2 byte | packet id 1 byte | data (n byte) | xor |

namespace Protocol
{
	void init(){}
	void update(){
		Sock::update();
	}
};
