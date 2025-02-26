#include "../color.hpp"
#include <cstdint>
#include <cstdio>


Color color_from_hexcode(const char hexcode[6]) {
	uint32_t r, g, b;
	sscanf(hexcode, "%02x%02x%02x", &r, &g, &b);
	Color c {r/255.0f,g/255.0f, b/255.0f, 1.0f};
	return c;
}

Color color_mix(const Color& a, const Color& b, float t) {
	Color result;
	for (size_t i=0; i < 4; i++) 
		result.rgba[i] = a.rgba[i] + ( b.rgba[i] - a.rgba[i]) * t;
	return result;
}
