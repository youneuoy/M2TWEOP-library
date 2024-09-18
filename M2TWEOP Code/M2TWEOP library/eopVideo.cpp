#include "pch.h"
#include "eopVideo.h"

#include "imgui/imgui.h"

eopVideo::eopVideo(char* path, IDirect3DDevice9* d3dDevice)
{
	if (!loadVideo(path))
	{
		IsBadState = true;
		return;
	}

	if (FAILED(d3dDevice->CreateTexture(videoWidth, videoHeight, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &videoTexture, nullptr)))
	{
		ErrorOnLoading = "Failed to create DirectX texture for video:" + std::string(path);;
		IsBadState = true;
		return;
	}
	ImageX = videoWidth;
	ImageY = videoHeight;

}

eopVideo::~eopVideo()
{
	av_frame_free(&frame);
	av_packet_free(&packet);
	avcodec_free_context(&codecContext);
	avformat_close_input(&formatContext);
	sws_freeContext(swsContext);
}

void eopVideo::PlayFrame()
{
	auto now = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime).count();

	if (elapsedTime >= frameDelay)
	{
		if (decodeFrame())
		{
			uploadFrameToDirectXTexture();
		}
		else
		{
			IsPlayEnded = true;
		}
		lastFrameTime = now;
	}


	ImGui::Image((void*)videoTexture, ImVec2(ImageX, ImageY));
}

void eopVideo::restartVideo()
{
	IsPlayEnded = false;

	resetContext();

	if (av_seek_frame(formatContext, videoStreamIndex, 0, AVSEEK_FLAG_BACKWARD) < 0) {
		//error
	}
	else {
		avcodec_flush_buffers(codecContext);
	}
}

int eopVideo::GetXSize()
{
	return ImageX;
}

void eopVideo::SetXSize(int size)
{
	ImageX = size;
}

int eopVideo::GetYSize()
{
	return ImageY;
}

void eopVideo::SetYSize(int size)
{
	ImageY = size;
}

bool eopVideo::IsOk()
{
	return !IsBadState;
}

void eopVideo::resetContext()
{
	avcodec_flush_buffers(codecContext);


	lastFrameTime = std::chrono::steady_clock::now();
}

bool eopVideo::loadVideo(const char* path)
{
	if (avformat_open_input(&formatContext, path, nullptr, nullptr) != 0) {
		ErrorOnLoading = "Failed to open video file: " + std::string(path);
		return false;
	}
	if (avformat_find_stream_info(formatContext, nullptr) < 0) {
		ErrorOnLoading = "Failed to retrieve stream info from video file.";
		return false;
	}

	int  videoStreamIndex = -1;
	for (unsigned int i = 0; i < formatContext->nb_streams; ++i) {
		if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStreamIndex = i;
			break;
		}
	}

	if (videoStreamIndex == -1) {
		ErrorOnLoading = "Failed to find video stream in file.";

		return false;
	}

	AVCodecParameters* codecParams = formatContext->streams[videoStreamIndex]->codecpar;
	const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
	if (!codec) {
		ErrorOnLoading = "Failed to find video codec.";

		return false;
	}

	codecContext = avcodec_alloc_context3(codec);
	if (!codecContext) {
		ErrorOnLoading = "Failed to allocate video codec context.";

		return false;
	}

	if (avcodec_parameters_to_context(codecContext, codecParams) < 0) {
		ErrorOnLoading = "Failed to copy video codec parameters to codec context.";

		return false;
	}

	if (avcodec_open2(codecContext, codec, nullptr) < 0) {
		ErrorOnLoading = "Failed to open video codec.";

		return false;
	}

	videoWidth = codecContext->width;
	videoHeight = codecContext->height;

	// Frame delay based on video frame rate
	AVRational frameRate = formatContext->streams[videoStreamIndex]->r_frame_rate;
	frameDelay = static_cast<int>(1000 * frameRate.den / frameRate.num);

	frame = av_frame_alloc();
	if (!frame) {
		ErrorOnLoading = "Failed to allocate frame.";
		return false;
	}
	packet = av_packet_alloc();
	if (!packet) {
		ErrorOnLoading = "Failed to allocate packet.";
		return false;
	}

	swsContext = sws_getContext(videoWidth, videoHeight, codecContext->pix_fmt, videoWidth, videoHeight, AV_PIX_FMT_RGBA, SWS_BILINEAR, nullptr, nullptr, nullptr);
	if (!swsContext) {
		ErrorOnLoading = "Failed to initialize SwsContext for frame scaling.";
		return false;
	}

	return true;
}

bool eopVideo::decodeFrame()
{
	while (av_read_frame(formatContext, packet) >= 0) {
		if (avcodec_send_packet(codecContext, packet) == 0) {
			if (avcodec_receive_frame(codecContext, frame) == 0) {
				av_packet_unref(packet);
				return true;
			}
		}
		av_packet_unref(packet);
	}
	return false;
}

void eopVideo::uploadFrameToDirectXTexture()
{
	if (!frame || !videoTexture) return;

	uint8_t* data[1] = { nullptr };
	int linesize[1] = { 0 };

	std::vector<uint8_t> rgbData(videoWidth * videoHeight * 3);
	data[0] = rgbData.data();
	linesize[0] = videoWidth * 3;

	sws_scale(swsContext, frame->data, frame->linesize, 0, videoHeight, data, linesize);

	D3DLOCKED_RECT lockedRect;
	if (SUCCEEDED(videoTexture->LockRect(0, &lockedRect, nullptr, D3DLOCK_DISCARD))) {
		uint8_t* dest = static_cast<uint8_t*>(lockedRect.pBits);
		for (int y = 0; y < videoHeight; ++y) {
			memcpy(dest + y * lockedRect.Pitch, rgbData.data() + y * linesize[0], linesize[0]);
		}
		videoTexture->UnlockRect(0);
	}
}



