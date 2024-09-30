float4x4 gWorldViewProj;
float4x3 gBoneMatrices[50];
Texture2D gBaseTexture;

sampler sBaseTexture = sampler_state
{
   texture = <gBaseTexture>;
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
   AddressU = WRAP;
   AddressV = WRAP;
};

struct VertexInSkinned
{
    float3 PositionL : POSITION0;
    float4 BoneWeights : BLENDWEIGHT;
    float4 BoneIndices : BLENDINDICES;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct VertexOut
{
   float4 PosH      : POSITION0;
   float2 Tex       : TEXCOORD0;
};

VertexOut VSSkinnedMinimal(
   VertexInSkinned vin)
{
   VertexOut vout = (VertexOut)0;
   float boneWeights[4] = (float[4])vin.BoneWeights;
   int boneIndices[4] = (int[4])vin.BoneIndices;
   float4 inPos = float4(vin.PositionL, 1.0f);
   float3 blendedPosition = 0.0f;

   for (int indice = 0; indice < 4; ++indice)
   {
      float normalizedBoneWeight = boneWeights[indice];
      float4x3 boneMatrix = gBoneMatrices[boneIndices[indice]];
      blendedPosition += normalizedBoneWeight * mul(inPos, boneMatrix);
   }

   vout.PosH = mul(float4(blendedPosition, 1.0f), gWorldViewProj);
   //vout.PosH = mul(inPos, gWorldViewProj);
   vout.Tex = vin.Tex;

   return vout;
}

float4 PS(VertexOut In) : SV_Target
{
   return tex2D(sBaseTexture, In.Tex);
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique minimalSkinnedTechnique
{
   pass P0
   {
      VertexShader = compile vs_2_0 VSSkinnedMinimal();
      PixelShader = compile ps_2_0 PS();
   }
}
