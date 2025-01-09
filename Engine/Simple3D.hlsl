//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

Texture2D g_toon_texture : register(t1);

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
    float4 color : COLOR; //色（明るさ)
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

    normal = mul(normal, matNormal);
    //float4 light = float4(0, 1, -1, 0);//光源ベクトルの逆ベクトル
    //float4 light = float4(1, 0, 0, 0);
    float4 light = lightPosition;
    light = normalize(light); //単位ベクトル化
    outData.color = saturate(dot(normal, light));
    float4 posw = mul(pos, matW);
    
    
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
    
    float NE = dot(inData.normal.xyz, normalize(inData.eyev.xyz)); //法線と視線のcos
    
    float4 NL = saturate(dot(inData.normal, normalize(lightPosition)));
    float4 reflection = reflect(normalize(-lightPosition), inData.normal);
    float4 specular = pow(saturate(dot(reflection, normalize(inData.eyev))), shininess) * specularColor;
    float2 uv;
    uv.x = NL;
    uv.y = 0.5;
    float tI = g_toon_texture.Sample(g_sampler, uv);
    
    //float stI = g_toon_texture.Sample(g_sampler, float2(specular.x, 0));
    
    float4 ambentSource = float4(0.5, 0.5, 0.5, 1.0);
    float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 diffuse;
    float4 ambient;
    if (isTextured == false)
    {
        //diffuse = diffuseColor * inData.color * factor.x;
        //ambient = diffuseColor * ambentSource * factor.x;
        diffuse = lightSource * diffuseColor * tI;
        ambient = lightSource * diffuseColor * ambientColor;
    }
    else
    {
        diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * tI;
        ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
    }
    float4 ret = diffuse + ambient;
    if (NE > -0.1 && NE < 0.1)
    {
        ret = float4(0, 0, 0, 1);
    }
    return ret;
    //return diffuse + ambient;
}