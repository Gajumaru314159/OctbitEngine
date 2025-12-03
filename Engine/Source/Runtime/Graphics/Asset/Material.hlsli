struct GlobalProps {
	float4x4 MatrixTest;
	float Time;
};

struct SceneProps {
	// LightRenderFeature
	float4 LightDir;
};

struct ViewProps {
	// CameraRenderFeature
	float4x4 MatrixV;
	float4x4 MatrixP;
	float4x4 MatrixVP;
	float4x4 MatrixInvV;
	float4x4 MatrixInvP;
	float4x4 MatrixInvVP;
	float4 CameraPos;
	float4 ScreenSize;
	float4 CameraUp;
	float4 CameraRight;
	float4 CameraFront;
};