//Basic mbient light shader with no texture support
float4x4 World;
float4x4 View;
float4x4 Projection;
float4 RainbowColor;
float AmbientIntensity = 1.0;
texture Texture;

struct MyVertexStruct
{
	float4 position : POSITION0;
	float4 texcoord : TEXCOORD0;
};

MyVertexStruct VertexShaderFunction(MyVertexStruct input_param)
{
	MyVertexStruct output = (MyVertexStruct)0;

	//combine world + view + projection matrices
	float4x4 WorldViewProj = mul(World,mul(View,Projection));

	//translate the current vertex
	output.position = mul(input_param.position, WorldViewProj);

	//set texture coordinates
	output.texcoord = input_param.texcoord;

	return output;
}

sampler MySampler = sampler_state
{
	Texture = <Texture>;
};

float4 PixelShaderFunction(float2 texcoord : TEXCOORD0) : COLOR
{
	return ( tex2D( MySampler,texcoord ) * RainbowColor);
}

technique AmbientTextured
{
	Pass P0
	{
		VertexShader = compile vs_2_0 VertexShaderFunction();
		PixelShader = compile ps_2_0 PixelShaderFunction();
	}
}
