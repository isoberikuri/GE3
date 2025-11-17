#pragma once


class WinApp
{
public:
    // 初期化処理
    void Initialize();

    // 更新処理
    void Update();

    // namespace省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    bool PushKey(BYTE keyNumber);
    bool TriggerKey(BYTE keyNumber);

private://メンバ変数
    //キーボードのデバイス
    ComPtr<IDirectInputDevice8> keyboard;
    // DirectInputのインスタンス生成
    ComPtr<IDirectInput8> directInput;

    BYTE key[256] = {};
    BYTE keyPre[256] = {};
