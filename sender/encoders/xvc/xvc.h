#pragma once

#include <vector>

#include "encoder.h"
#include "xvc_enc_lib/xvcenc.h"

class XvcEncoder : public Encoder
{
public:
	XvcEncoder();
	~XvcEncoder();

	virtual EncodeResult* Init(EncoderConfig& InConfig) override;
	virtual EncodeResult* Encode(std::istream* InStream) override;

private:
	bool ReadNextPicture(std::istream* InStream, std::vector<uint8_t>& OutPictureBytes);
	void PrintNalInfo(xvc_enc_nal_unit NalUnit);

private:
	const xvc_encoder_api*	Api;
	xvc_encoder_parameters* Params;
	xvc_encoder*			Encoder;
	std::vector<uint8_t>	PictureBytes;
};