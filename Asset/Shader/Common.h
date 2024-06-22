
SamplerState g_mainSampler:register(s0);

// Global
cbuffer Param : register(b0) {
  //float    s_scalars[4];
  float4   s_colors[1];
  float4x4 s_matrices[1];
};

// Local
cbuffer Param : register(b1) { 
  float    g_scalars[8];
  float4   g_colors[8];
  float4x4 g_matrices[8];
};