#pragma once

#include "ovb_recv/decoders/decoder_result.h"
#include "xvc_dec_lib/xvcdec.h"

class OvcResult : public DecodeResult
{
public:
	OvcResult(ovc_dec_result InOvcReturn)
		: OvcReturn(InOvcReturn) {}

	virtual bool		IsSuccess() override { return OvcReturn == OVC_DEC_OK; }
	virtual std::string Error() override
	{
		switch (OvcReturn)
		{
			case OVC_DEC_OK:
				return "OVC_DEC_OK";
			case OVC_DEC_UNINITIALISED:
				return "OVC_DEC_UNINITIALISED";
			case OVC_DEC_NO_PICTURE:
				return "OVC_DEC_NO_PICTURE";
			case OVC_DEC_MALFORMED_NAL_HEADER:
				return "OVC_DEC_MALFORMED_NAL_HEADER";
			case OVC_DEC_MALFORMED_NAL_BODY:
				return "OVC_DEC_MALFORMED_NAL_BODY";
			case OVC_DEC_MISSING_VPS:
				return "OVC_DEC_MISSING_VPS";
			case OVC_DEC_MISSING_PPS:
				return "OVC_DEC_MISSING_PPS";
			default:
				return "";
		}
	}

private:
	ovc_dec_result OvcReturn;
};