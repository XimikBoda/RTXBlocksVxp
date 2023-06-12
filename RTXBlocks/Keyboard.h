#pragma once
#ifndef MRE
#include <SFML/Window/Event.hpp>

/* Key events supported by MRE. */

#define VM_KEY_EVENT_UP       (1)       /* Key up event */
#define VM_KEY_EVENT_DOWN        (2)    /* Key down event */
#define VM_KEY_EVENT_LONG_PRESS  (3)    /* Key long press event */
#define VM_KEY_EVENT_REPEAT      (4)    /* Key repeat event */

/* Key codes supported by MRE. */

#define VM_KEY_UP             (-1)      /* Arrow key up */
#define VM_KEY_DOWN           (-2)      /* Arrow key down */
#define VM_KEY_LEFT              (-3)   /* Arrow key left */
#define VM_KEY_RIGHT          (-4)      /* Arrow key right */
#define VM_KEY_LEFT_SOFTKEY      (-6)   /* Left soft key */
#define VM_KEY_RIGHT_SOFTKEY     (-7)   /* Right soft key */
#define VM_KEY_OK             (-5)      /* OK key */
#define VM_KEY_CLEAR          (-8)      /* Clear key */
#define VM_KEY_BACK              (-9)   /* Back key */
#define VM_KEY_NUM0           (48)      /* Number key 0 */
#define VM_KEY_NUM1           (49)      /* Number key 1 */
#define VM_KEY_NUM2           (50)      /* Number key 2 */
#define VM_KEY_NUM3           (51)      /* Number key 3 */
#define VM_KEY_NUM4           (52)      /* Number key 4 */
#define VM_KEY_NUM5           (53)      /* Number key 5 */
#define VM_KEY_NUM6           (54)      /* Number key 6 */
#define VM_KEY_NUM7           (55)      /* Number key 7 */
#define VM_KEY_NUM8           (56)      /* Number key 8 */
#define VM_KEY_NUM9           (57)      /* Number key 9 */
#define VM_KEY_VOL_UP            (58)   /* Volume key up */
#define VM_KEY_VOL_DOWN       (59)      /* Volume key down */
#define VM_KEY_POUND          (35)      /* Key # */
#define VM_KEY_STAR              (42)   /* Key * */
#endif // MRE


namespace Keyboard
{
	void init();

	bool pres(int a);
	bool click(int a);

	bool at_least_one_key();

	void update();

	void keyboard_event(int k, int ev);
#ifndef MRE
	void keyMaping(sf::Event ev);
#endif // MRE
};

