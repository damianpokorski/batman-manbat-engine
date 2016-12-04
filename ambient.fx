float4x4 World;
float4x4 View;
float4x4 Projection;
float4 AmbientColor : AMBIENT = float4(1.0,1.0,1.0,1.0);
float AmbientIntensity = 1.0;
struct MyVertexStruct{
	float4 position : POSITION0;
	float4 color : COLOR0;
};
MyVertexStruct VertexShaderFunction(MyVertexStruct input_param){
	MyVertexStruct output = (MyVertexStruct)0;
	float4x4 WorldViewProj = mul(World, mul(View,Projection));
	output.position = mul(input_param.position, WorldViewProj);
	
	output.color.rgb = AmbientColor * AmbientIntensity;
	output.color.a = AmbientColor.a;
	
	return output;
}
float4 PixelShaderFunction(float4 c:COLOR0) : COLOR{
	return c;
}
technique Ambient{
	Pass P0{
		VertexShader = compile vs_2_0 VertexShaderFunction();
		PixelShader = compile ps_2_0 PixelShaderFunction();
	}
}