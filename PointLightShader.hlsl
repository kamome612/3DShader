//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    //変換行列、視点、光源
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW; //ワールド変換マトリクス
    float4x4 matNormal; //法線をワールド座標に対応させる行列＝回転＊スケール
    float4 diffuseColor; // ディフューズカラー（マテリアルの色）
    float4 lightPosition;
    float4 factor; //ディフューズファクター(diffuseFactor)
    bool isTextured; // テクスチャ貼ってあるかどうか
    
    //float4 Attenuation;
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 wpos : POSITION;
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 normal : NORMAL;
    
    //float4 norw : NORMALO;
    //float4 cos_alpha : COLOR; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    
    float4 spos = mul(pos, matWVP);
    float4 wpos = mul(pos, matW);
    float4 wnormal = mul(normal, matNormal);
    
    outData.pos = spos;
    outData.wpos = wpos;
    outData.uv = uv.xy;
    outData.normal = wnormal;
    
    //float dir = normalize(lightPosition - wpos);
    //outData.color = clamp(dot(normalize(wnormal), dir), 0, 1);

    //normal = mul(normal, matNormal);
    //normal.w = 0;
    //normal = normalize(normal);
    
    ////outData.norw = mul(normal, matWVP);
    ////lightを点光源の方向ベクトルに置き換える
    //float4 lightvec = lightPosition - outData.pos;
    //float4 light = normalize(lightvec); //単位ベクトル化
    //outData.color = clamp(dot(normal, light), 0, 1);
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 diffuse;
    float4 ambient;
    float4 ambentSource = { 0.2, 0.2, 0.2, 1.0 };
    //float4 ambentSource = { 0.5, 0.5, 0.5, 1.0 };
    float3 dir = normalize(lightPosition.xyz - inData.wpos.xyz);//ピクセル位置の織ゴンの３次元座標 = wpos
    //inData.normal.z = 0;
    float color = saturate(dot(normalize(inData.normal.xyz), dir));
    float3 k = { 0.2f, 0.2f, 1.0f };
    float len = length(lightPosition.xyz - inData.wpos.xyz);
    float dTerm = 1.0 / (k.x + k.y * len + k.z * len * len);
    
    if (isTextured == false)
    {
        diffuse = diffuseColor * color * dTerm * factor.x;
        ambient = diffuseColor * ambentSource;
    }
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv)* color * dTerm * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambentSource;
    }
    return diffuse + ambient;
    
    //float3 dir;
    //float len;
    //float colD;
    //float colA;
    //float col;
    
    ////点光源の方向
    //dir = lightVec;
    
    ////点光源の距離
    //len = length(dir);
    
    ////点光源の方向をnormalize
    //dir = dir / len;
    
    ////拡散
    //colD = saturate(dot(normalize(inData.norw.xyz), dir));
    ////減衰
    //colA = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));
    
    //col = colD * colA;
    //return float4(col, col, col, 1.0f);
}
