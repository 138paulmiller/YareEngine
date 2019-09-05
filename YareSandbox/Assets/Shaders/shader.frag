#version 330

in vec4 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;

in vec4 light_pos;

uniform sampler2D tex0;

// output
out vec4 color;

void main(void)
{       
    vec3 o =-normalize(frag_position.xyz);
    vec3 n = normalize(frag_normal);
    vec3 r = reflect(o,n);
    vec3 l = normalize(light_pos.xyz-frag_position.xyz);

    float ambient = 0.1;
    float diffus = 0.7*max(0.0,dot(n,l));
    float specular = 0.6*pow(max(0.0,-dot(r,l)),4.0);

    color = texture(tex0, frag_uv) * ( ambient + diffus + specular );

}
