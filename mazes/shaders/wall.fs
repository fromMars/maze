#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 Normal;
    vec2 TexCoord;
    vec3 frag_pos;
    vec4 FragPosLightSpace;
} vs_in;

uniform sampler2D texture1;
uniform sampler2D depth_map;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    vec3 proj_coord = fragPosLightSpace.xyz/fragPosLightSpace.w;
    proj_coord = proj_coord * .5 + .5;
    float closest_depth = texture(depth_map, proj_coord.xy).r;
    float current_depth = proj_coord.z;
    float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;
    if (proj_coord.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    // ambient
    float ambientstrength = 0.2;
    vec3 ambient = light_color * ambientstrength;
    // diffuse
    vec3 norm = normalize(vs_in.Normal);
    vec3 light_dir = normalize(light_pos - vs_in.frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light_color * diff;
    // specular
    vec3 view_dir = normalize(view_pos - vs_in.frag_pos);
    //vec3 reflect_dir = reflect(-light_dir, norm);
    //float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
    vec3 hf_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(norm, hf_dir), 0.0), 32);
    vec3 specular = light_color * spec;
    // shadow
    float bias = max(0.01*(1.0-dot(norm, light_dir)), 0.005);
    bias = 0.0f;
    float shadow = ShadowCalculation(vs_in.FragPosLightSpace, bias);
    // result
    //vec3 result = ambient + diffuse + specular;
    vec3 result = (ambient + (1.0-shadow) * (diffuse+specular)) * light_color;

    FragColor = vec4(result * texture(texture1, vs_in.TexCoord).rgb, 1.0);
    //FragColor = vec4(result * texture(texture1, TexCoord).rgb, 1.0);
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
