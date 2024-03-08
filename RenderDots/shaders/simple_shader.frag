#version 450

layout(push_constant) uniform Pushdata
{
    vec3 color;
} pushdata;

layout(location = 0) out vec4 outColor;

void main()
{
    // r g b a
    outColor = vec4(pushdata.color, 1.0);
}

