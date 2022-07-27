#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;

out VS_OUT {
    vec3 Normal;
    vec2 TexCoord;
    vec3 frag_pos;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 transpose_inverse;
uniform mat4 lightSpaceMatrix;

void main()
{
    vs_out.Normal = mat3(transpose_inverse) * aNormal;
    vs_out.TexCoord = aTexCoord;
    vs_out.frag_pos = vec3(model * vec4(aPos, 1.0));
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.frag_pos, 1.0);
    gl_Position = projection * view * vec4(vs_out.frag_pos, 1.0);

}
