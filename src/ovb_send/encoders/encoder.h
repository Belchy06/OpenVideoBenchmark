#pragma once

#include <vector>

#include "ovb_common/common.h"
#include "encoder_callback.h"
#include "encoder_config.h"
#include "encoder_result.h"

// Encoder wrapper that wraps all of the third_party codecs
class Encoder
{
public:
	void RegisterEncodeCompleteCallback(IEncodeCompleteCallback* InEncoderCompleteCallback);

	virtual EncodeResult* Init(EncoderConfig& InConfig);
	virtual EncodeResult* Encode(std::vector<uint8_t>& InPictureBytes, bool bInLastPicture);

protected:
	int ScaleX(int InX, EChromaFormat InFormat);
	int ScaleY(int InY, EChromaFormat InFormat);

protected:
	EncoderConfig			 Config;
	IEncodeCompleteCallback* OnEncodedImageCallback;
};