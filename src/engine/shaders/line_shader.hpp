#pragma once

const char* LINE2D_SHADER_SOURCE_V = R"(
#version 410 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
uniform mat4 uProj, uView;
out vec4 fVertexColor;
void main() {
	fVertexColor = aColor;
	gl_Position = uProj * uView * vec4(aPos, 0.0, 1.0f);
}
)";

const char* LINE2D_SHADER_SOURCE_F = R"(
#version 410 core
in vec4 fVertexColor;
out vec4 fragColor;
void main() {
	fragColor = fVertexColor;
}
)";
