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
    bool isNormalMapped; //法線マップが貼られているかどうか
};

cbuffer gStage : register(b1)
{
    float4 lightPosition;
    float4 eyePosition;
    float4 pLightposition;
    float4 pointLightColor[5];
    float4 spotLightColor;
    float4 direction;
    float4 kTerm[5];
    float4 sptParam;
    int4 pointListSW[5];
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    //float4 wpos : POSITION0;
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 eyev : POSITION;//ワールド座標に変換された視線っベクトル
    float4 Neyev : POSITION1;//ノーマルマップ用の接空間に変換された視線ベクトル
    float4 normal : NORMAL;//法線ベクトル
    float4 light : POSITION2;//ライトを接空間に変換したベクトル
    float4 color : COLOR; //色（明るさ）
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
    float4 light = lightPosition[0];
    light = normalize(light); //単位ベクトル化
    outData.color = clamp(dot(normal, light), 0, 1);
    
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 ambentSource = float4(0.0, 0.0, 0.0, 1.0);
    float4 diffuse;
    float4 ambient;
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
	//return g_texture.Sample(g_sampler, inData.uv);// (diffuse + ambient);]
	//float4 diffuse = lightSource * inData.color;
	//float4 ambient = lightSource * ambentSource;
    return diffuse + ambient;
}