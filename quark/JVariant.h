#pragma once
/*
 * To be used as the main delegate type between all systems. There should be no differentiation 
 * on the systems that are using these. If need be, we can make more items off of these, but for now,
 * this should do
*/
enum JType {
	NUMBER = 1,
	BOOLEAN = 2,
	STRING = 3,
	ARRAY = 4,
	FUNCTION = 5,
	BYTE = 6
};

template <typename T>

class JVariant {
	private T el;
	private JType type;
	JVariant(JType type, T el) : type, el{};

	protected void run(ArrayList args = new ArrayList<JVariant>()) {
		if (type == = JType.FUNCTION) {
			el.run(args);
		}
	}
};