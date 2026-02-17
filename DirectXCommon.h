#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "Logger.h"
#include "StringUtility.h"
#include "WinApp.h"
#include <Windows.h>
#include <format>
#include <array>
#include <dxcapi.h>
#include <cassert>
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include <thread>
#include "externals/DirectXTex/DirectXTex.h"


class DirectXCommon
{
public:
	// 初期化処理
	void Initialize(WinApp* winApp);

	//=========================================//
	//          デバイス(ID3D12Device)         //
	//=========================================//
	void CreateDevice();
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	//=========================================//
	//              コマンド関連               //
	//=========================================//
	void CreateCommandQueue();
	//=========================================//
	//            スワップチェーン             //
	//=========================================//
	void CreateSwapChain();
	IDXGISwapChain4* GetSwapChain() const { return swapChain.Get(); }
	//=========================================//
	//              深度バッファ               //
	//=========================================//
	void CreateDepthBuffer();
	//=========================================//
	//         各種デスクリプタヒープ          //
	//=========================================//
	void CreateDescriptorHeapRTVDSV();
	//=========================================//
	//         レンダーターゲットビュー        //
	//=========================================//
	void CreateRenderTargetViews();
	//SRVの指定指定番号のCPUデスクリプタハンドルを取得する
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);
	//SRVの指定指定番号のGPUデスクリプタハンドルを取得する
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);
	//=========================================//
	//           深度ステンシルビュー          //
	//=========================================//
	void CreateDepthStencilView();
	//=========================================//
	//                 フェンス                //
	//=========================================//
	void CreateFence();
	//=========================================//
	//               ビューポート              //
	//=========================================//
	void InitializeViewport();
	//=========================================//
	//              シザリング矩形             //
	//=========================================//
	void InitializeScissorRect();
	//=========================================//
	//              DXCコンパイラ              //
	//=========================================//
	void CreateDXCCompiler();
	//=========================================//
	//                  ImGui                  //
	//=========================================//
	void InitializeImGui();
	// ★ これを追加
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
	//フェンス値
	UINT64 fenceVal = 0;
	//DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	//ゲッター getter
	ID3D12Device* GetDevice() const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }

	//=========================================//
	//          シェーダーコンパイル           //
	//=========================================//
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader
	(
		const std::wstring& filePath,
		const wchar_t* profile
	);

	//=========================================//
	//              リソースの生成             //
	//=========================================//
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);
	void UploadTextureData(const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const DirectX::ScratchImage& mipImages);
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

private://メンバ変数

	//=========================================//
	//          デバイス(ID3D12Device)         //
	//=========================================//

	//=========================================//
	//            スワップチェーン             //
	//=========================================//
	//スワップチェーンリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources[2];
	//=========================================//
	//         各種デスクリプタヒープ          //
	//=========================================//
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap>CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	//=========================================//
	//         レンダーターゲットビュー        //
	//=========================================//
	//指定番号のCPUデスクリプタハンドルを取得する
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//指定番号のGPUデスクリプタハンドルを取得する
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);

	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	// RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;
	// SRV用デスクリプタサイズ
	UINT descriptorSizeSRV = 0;

	//=========================================//
	//           深度ステンシルビュー          //
	//=========================================//
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;
	//=========================================//
	//                 フェンス                //
	//=========================================//
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	HANDLE fenceEvent = nullptr;
	//=========================================//
	//               ビューポート              //
	//=========================================//
	D3D12_VIEWPORT viewport{};
	//=========================================//
	//              シザリング矩形             //
	//=========================================//
	D3D12_RECT scissorRect{};

	//DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	//WindowsAPI
	WinApp* winApp = nullptr;
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 2> rtvHandles{};

	//================================//
    //              60fps             //
    //================================//

    //FPS固定初期化
	void InitializeFixFPS();

	//FPS固定更新
	void UpdateFixFPS();

	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	//=========================================//
    //          シェーダーコンパイル           //
    //=========================================//
	// DXC
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;


};