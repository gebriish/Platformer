#pragma once 

struct Color {
	union {
		struct { float r, g, b, a; };
		float rgba[4];
	};
};

Color color_from_hexcode(const char hexcode[6]);
Color color_mix(const Color& a, const Color& b, float t);
