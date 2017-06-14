
#include <Shlwapi.h>
#include <atlbase.h>

// bitmap scaling

class BitmapScaler
{
private:
    IWICImagingFactory* mpIWICFactory;

public:
    ~BitmapScaler()
    {
        if (mpIWICFactory) {
            mpIWICFactory->Release();
            mpIWICFactory = NULL;
        }
    }

    BOOL Init()
    {
        if (!mpIWICFactory) {
            HRESULT hr;
            hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                                  IID_PPV_ARGS(&mpIWICFactory));
            if (FAILED(hr)) {
                DebugMsg(TEXT("Failed to create imaging factory: %x"), hr);
                return FALSE;
            }
        }
        return TRUE;
    }

    BOOL Scale(RGBFrameData& src, RGBFrameData& out)
    {
        ASSERT(mpIWICFactory);

        HRESULT hr;
        IWICBitmap* pBitmapSrc;
        int iSize = src.iHeight * src.iWidth * sizeof(RGBTRIPLE);
        hr = mpIWICFactory->CreateBitmapFromMemory(src.iWidth, src.iHeight, GUID_WICPixelFormat24bppBGR,
                                                   src.iWidth * sizeof(RGBTRIPLE), iSize,
                                                   reinterpret_cast<BYTE*>(src.pData), &pBitmapSrc);
        if (FAILED(hr)) {
            DebugMsg(TEXT("Cannot create bitmap: %x"), hr);
            return FALSE;
        }

        // Create scaler and scale the image
        IWICBitmapScaler* pIScaler;
        hr = mpIWICFactory->CreateBitmapScaler(&pIScaler);
        hr = pIScaler->Initialize(pBitmapSrc, out.iWidth, out.iHeight, WICBitmapInterpolationModeLinear);
        if (FAILED(hr)) {
            DebugMsg(TEXT("Cannot scale bitmap: %x"), hr);
            pBitmapSrc->Release();
            return FALSE;
        }
        pBitmapSrc->Release();

        // Copy scaled image pixels to out frame
        int iOutSize = out.iHeight * out.iWidth * sizeof(RGBTRIPLE);
        hr = pIScaler->CopyPixels(NULL, out.iWidth * sizeof(RGBTRIPLE), iOutSize,
                                  reinterpret_cast<BYTE*>(out.pData));
        pIScaler->Release();
        if (FAILED(hr)) {
            DebugMsg(TEXT("Cannot copy scaled bitmap: %x"), hr);
            return FALSE;
        }
        return TRUE;
    }

    BOOL Write(const RGBFrameData& out)
    {
        // Bitmap writing
        CComPtr<IWICBitmapFrameEncode> piBitmapFrame;
        CComPtr<IPropertyBag2> pPropertybag;
        CComPtr<IWICBitmapEncoder> piEncoder;
        CComPtr<IStream> pStream = (SHCreateMemStream(NULL, 0));

        hr = mpIWICFactory->CreateEncoder(GUID_ContainerFormatJpeg, NULL, &piEncoder);
        hr = piEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
        hr = piEncoder->CreateNewFrame(&piBitmapFrame, &pPropertybag);

        hr = piBitmapFrame->Initialize(pPropertybag);
        hr = piBitmapFrame->SetSize(out.iWidth, out.iHeight);
        WICPixelFormatGUID formatGUID = GUID_WICPixelFormat24bppBGR;
        hr = piBitmapFrame->SetPixelFormat(&formatGUID);

        hr = piBitmapFrame->WritePixels(out.iHeight, out.iWidth * sizeof(RGBTRIPLE), iOutSize,
                                        reinterpret_cast<BYTE*>(out.pData));
        hr = piBitmapFrame->Commit();
        hr = piEncoder->Commit();

        STATSTG ssStremData{};
        hr = pStream->Stat(&ssStremData, STATFLAG_NONAME);

        LARGE_INTEGER pos{};
        hr = pStream->Seek(pos, STREAM_SEEK_SET, NULL);
        ULONG cbRead;
        BYTE* p = new BYTE[ssStremData.cbSize.LowPart];
        pStream->Read(p, ssStremData.cbSize.LowPart, &cbRead);

        HANDLE f = CreateFile(L"D:\\VR\\screen.jpg", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, NULL);
        WriteFile(f, p, cbRead, NULL, NULL);
        CloseHandle(f);

        delete p;
        return TRUE;
    }
};
