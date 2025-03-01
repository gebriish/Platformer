#pragma once

static const char* SPRITE_SHADER_SOURCE_V = R"(
#version 410 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 instancePos;
layout (location = 2) in vec2 instanceSize;
layout (location = 3) in vec2 instanceUvMin;
layout (location = 4) in vec2 instanceUvMax;
layout (location = 5) in vec4 instanceColor;
layout (location = 6) in float instanceTexId;
uniform mat4 uProj, uView;
out vec4 fVertexColor;
out vec2 fTexCoords;
flat out vec2 fSpriteSize;
flat out float fTexId;
void main() {
  vec2 worldPos = instancePos + (aPos * instanceSize);
	fSpriteSize = instanceSize;
  fVertexColor = instanceColor;
  fTexCoords = instanceUvMin + aPos * (instanceUvMax - instanceUvMin);
  fTexId = instanceTexId;
  gl_Position = uProj * uView * vec4(worldPos, 0.0f, 1.0f);
}
)";


static const char* SPRITE_SHADER_SOURCE_F = R"(
#version 410 core
in vec4 fVertexColor;
in vec2 fTexCoords;
flat in float fTexId;
flat in vec2 fSpriteSize;
out vec4 fragColor;
uniform sampler2D uTextures[32];
void main() {
	int texIndex = int(fTexId);
	vec2 res = textureSize(uTextures[texIndex], 0);
	vec2 texel = 1/res;
	vec2 pxPerTex = fSpriteSize * texel;
	vec2 tx = fTexCoords * res;
	vec2 txOffset = clamp(fract(tx) * pxPerTex, 0, 0.5) - clamp((1 - fract(tx)) * pxPerTex, 0, 0.5);
	vec2 uv = (floor(tx) + 0.5 + txOffset) * texel;
	vec4 texture_color = texture(uTextures[texIndex], uv);
	fragColor = texture_color * fVertexColor;
}
)";
