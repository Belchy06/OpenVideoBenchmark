#pragma once

#include <ios>

#include "ovb_common/common.h"

#include "ovb_send/encoders/ovc/ovc_config.h"
#include "ovb_send/encoders/vvc/vvc_config.h"
#include "ovb_send/encoders/xvc/xvc_config.h"

class EncoderConfig : public OvcEncoderConfig, public VvcEncoderConfig, public XvcEncoderConfig
{
public:
	int			  Width;
	int			  Height;
	int			  BitDepth;
	double		  Framerate;
	EChromaFormat Format;

	ELogSeverity LogLevel;
};