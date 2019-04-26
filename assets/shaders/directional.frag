#version 330 core
uniform vec3 u_topHemisphereDirection;
uniform vec3 u_skyColor;
uniform vec3 u_groundColor;

in Interpolators {
    vec2 uv;
    vec3 view;
    vec3 normal;
} fs_in;

struct Material {
    sampler2D ambient_occlusion;
    sampler2D emissive;
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    vec3 emissive_tint;
    vec3 albedo_tint;
    vec3 specular_tint;
    float roughness_scale;
    //TODO: Complete The Material Struct. Find the variable names in the scene code. 
};
uniform Material material;

struct DirectionalLight {
    vec3 color;
    vec3 direction;
};
uniform DirectionalLight light;

uniform vec3 ambient;

out vec4 color;

float lambert(vec3 n, vec3 l){
    return max(0, dot(n,l));
}

float phong(vec3 n, vec3 l, vec3 v, vec3 h, float shininess){
    //TODO: Change Model from Phong to Blinn Phong
    return pow(max(0, dot(n,h)), shininess);
}

void main()
{

    float ao = texture(material.ambient_occlusion, fs_in.uv).r;
    vec3 emissive = material.emissive_tint * texture(material.emissive, fs_in.uv).rgb;
    vec3 albedo = material.albedo_tint * texture(material.albedo, fs_in.uv).rgb;
    vec3 specular = material.specular_tint * texture(material.specular, fs_in.uv).rgb;
    float roughness = material.roughness_scale * texture(material.roughness,fs_in.uv).r;
    vec3 n = normalize(fs_in.normal);
    vec3 v = normalize(fs_in.view);
    vec3 l = -light.direction;
    vec3 h = normalize(-light.direction + v);
    float shininess = 2/pow(max(0.01f,roughness), 2) - 2;
    float NdotL	= dot(n, u_topHemisphereDirection);
   // from [-1, 1] to [0, 1] range
   float lightInfluence	= NdotL * 0.5 + 0.5;
   // interpolate colors from upper and lower hemispheres
    vec3 colorhemi = mix(u_groundColor, u_skyColor, lightInfluence);
    color = vec4(
        emissive +
        colorhemi +
        albedo*ao*ambient + 
        albedo*light.color*lambert(n, l) + 
        specular*light.color*phong(n, l, v, h, shininess),
        1.0f 
    );
}