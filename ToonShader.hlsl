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
    float4 factor; //ディフューズファクター(diffuseFactor)
    float4 ambientColor;
    float4 specularColor;
    float4 shininess;
    bool isTextured; // テクスチャ貼ってあるかどうか
};

cbuffer gStage : register(b1)
{
    float4 lightPosition;
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色（明るさ）
    float4 normal : NORMAL;
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
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;

    outData.normal = mul(normal, matNormal);
    //float4 light = float4(0, 1, -1, 0);//光源ベクトルの逆ベクトル
    //float4 light = float4(1, 0, 0, 0);
    float4 light = lightPosition;
    light = normalize(light); //単位ベクトル化
    outData.color = clamp(dot(normal, light), 0, 1);
    
    //float OutColor;
    //if (outData.color.x < 1 / 4)
    //{
    //    OutColor = float4(0 / 4.0f, 0 / 4.0f, 0 / 4.0f, 1.0);

    //}
    //else if (outData.color.x < 2 / 4)
    //{
    //    OutColor = float4(1 / 4.0f, 1 / 4.0f, 1 / 4.0f, 1.0);
    //}
    //else if (outData.color.x < 3 / 4)
    //{
    //    OutColor = float4(2 / 4.0f, 2 / 4.0f, 2 / 4.0f, 1.0);
    //}
    //else if (outData.color.x < 1)
    //{
    //    OutColor = float4(3 / 4.0f, 3 / 4.0f, 3 / 4.0f, 1.0);
    //}
    //else
    //{
    //    OutColor = float4(4 / 4.0f, 4 / 4.0f, 4 / 4.0f, 1.0);
    //}
    
    //outData.color = OutColor;
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    ////float2 myUv = { 0, 125, 0.25, 0, 0 };
    //float4 Id = { 1.0, 1.0, 1.0, 0.0 };
    //float4 Kd = g_texture.Sample(g_sampler, inData.uv);
    //float4 ambentSource = { 0.5, 0.5, 0.5, 0.0 }; //環境光の強さ
    //if (isTextured == false)
    //{
    //    return Id * diffuseColor * inData.color + Id * diffuseColor * ambentSource;
    //}
    //else
    //{
    //    return Id * Kd * inData.color + Id * Kd * ambentSource;
    //}
    //return g_texture.Sample(g_sampler, inData.uv);
    
    float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 ambentSource = float4(0.5, 0.5, 0.5, 1.0);
    float4 diffuse;
    float4 ambient;
    
    float4 NL = saturate(dot(inData.normal, normalize(lightPosition)));
    float4 n1 = float4(1 / 4.0, 1 / 4.0, 1 / 4.0, 1);
    float4 n2 = float4(2 / 4.0, 2 / 4.0, 2 / 4.0, 1);
    float4 n3 = float4(3 / 4.0, 3 / 4.0, 3 / 4.0, 1);
    float4 n4 = float4(4 / 4.0, 4 / 4.0, 4 / 4.0, 1);
    float tI = 0.1 * step(n1, NL) + 0.2 * step(n2, NL) + 
               0.3 * step(n3, NL) + 0.4 * step(n4, NL);
    
    float OutColor;
    if (NL.x < 1.0f / 4)
    {
        OutColor = float4(0.0f / 4.0f, 0.0f / 4.0f, 0.0f / 4.0f, 1.0f);

    }
    else if (NL.x < 2.0f / 4)
    {
        OutColor = float4(1.0f / 4.0f, 1.0f / 4.0f, 1.0f / 4.0f, 1.0f);
    }
    else if (NL.x < 3.0f / 4)
    {
        OutColor = float4(2.0f / 4.0f, 2.0f / 4.0f, 2.0f / 4.0f, 1.0f);
    }
    else if (NL.x < 1.0f)
    {
        OutColor = float4(3.0f / 4.0f, 3.0f / 4.0f, 3.0f / 4.0f, 1.0f);
    }
    else
    {
        OutColor = float4(4.0f / 4.0f, 4.0f / 4.0f, 4.0f / 4.0f, 1.0f);
    }
    
    if (isTextured == false)
    {
        diffuse = diffuseColor * inData.color * factor.x;
        ambient = diffuseColor * ambentSource * factor.x;
    }
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambentSource * factor.x;
    }
    return diffuse + ambient;
    //return OutColor;
}