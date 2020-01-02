#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
};

//--------------- Inputs ------------------//
in vec4 frag_pos;
in vec3 frag_normal;
in vec2 frag_uv;

//--------------- Outputs ------------------//
//Write to RenderTarget 0 - See Attachment points in Rendertarget
layout(location = 0) out vec3 out_position;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec4 out_diffuse;
layout(location = 3) out vec4 out_specular;
layout(location = 4) out vec4 out_emissive;

uniform Material material;



void main(void)
{       
	out_position = frag_pos.xyz;
	out_normal = normalize(frag_normal);
	//out_depth = frag_pos.zzz;

	out_diffuse = texture(material.diffuse, frag_uv);
	out_specular.rgb = texture(material.specular, frag_uv).rgb;
	out_specular.a = material.shininess;
	out_emissive = texture(material.emission, frag_uv);


}



