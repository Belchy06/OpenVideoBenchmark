#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "common.h"
#include "bvc.h"
#include "bvc_common/format.h"
#include "bvc_result.h"

BvcEncoder::BvcEncoder()
	: Params(nullptr), Encoder(nullptr)
{
}

BvcEncoder::~BvcEncoder()
{
	if (Encoder)
	{
		Encoder = nullptr;
	}

	if (Params)
	{
		Params = nullptr;
	}
}

EncodeResult* BvcEncoder::Init(EncoderConfig& InConfig)
{
	Config = InConfig;

	if (Config.Width)
	{
		Params->width = Config.Width;
	}

	if (Config.Height)
	{
		Params->height = Config.Height;
	}

	if (Config.Format != EChromaFormat::CHROMA_FORMAT_UNDEFINED)
	{
		// clang-format off
        if(Config.Format == EChromaFormat::CHROMA_FORMAT_MONOCHROME) {
            Params->format = bvc_chroma_format::BVC_CHROMA_FORMAT_MONOCHROME;
        } else if(Config.Format == EChromaFormat::CHROMA_FORMAT_420) {
            Params->format = bvc_chroma_format::BVC_CHROMA_FORMAT_420;
        } else if(Config.Format == EChromaFormat::CHROMA_FORMAT_422) {
            Params->format = bvc_chroma_format::BVC_CHROMA_FORMAT_422;
        } else if(Config.Format == EChromaFormat::CHROMA_FORMAT_444) {
            Params->format = bvc_chroma_format::BVC_CHROMA_FORMAT_444;
        }
		// clang-format on
	}

	Encoder = new bvc_encoder();
	if (!Encoder)
	{
		std::cerr << "Error: Failed to allocate encoder" << std::endl;
		std::exit(-1);
	}

	return new BvcResult(Encoder->init(Params));
}

EncodeResult* BvcEncoder::Encode(std::vector<uint8_t>& InPictureBytes, bool bInLastPicture)
{
	bvc_enc_result Result;
	bvc_enc_nal*   NalUnits;
	int			   NumNalUnits;

	if (bInLastPicture)
	{
		// Flush the encoder for remaining NalUnits and reconstructed pictures.
		// Result = Api->encoder_flush(Encoder, &NalUnits, &NumNalUnits, nullptr);
		// Continue will remain true as long as there are buffered pictures
		// that should be reconstructed.
	}
	else
	{
		// Encode one picture and get 0 or 1 reconstructed picture back.
		// Also get back 0 or more NalUnits depending on if pictures are being
		// buffered in order to encode a full Sub Gop.
		Result = Encoder->encode(&InPictureBytes[0], &NalUnits, &NumNalUnits);
	}

	// Loop through all Nal Units that were received and write to file
	// the Nal Unit length followed by the actual Nal Unit.
	for (int i = 0; i < NumNalUnits; i++)
	{
		if (OnEncodedImageCallback != nullptr)
		{
			OnEncodedImageCallback->OnEncodeComplete(NalUnits[i].bytes, NalUnits[i].size);
		}
	}

	return new BvcResult(Result);
}