#pragma once
struct UUID
{
	unsigned int a1, a2, a3, a4;
	const char* get_string();

	bool operator== (const UUID& b) {
		return (this->a1 == b.a1) & (this->a2 == b.a2) &
			(this->a3 == b.a3) & (this->a4 == b.a4);
	}
	UUID operator= (const UUID& b) {
		this->a1 = b.a1; 
		this->a2 = b.a2;
		this->a3 = b.a3; 
		this->a4 = b.a4;
		return *this;
	}
};

