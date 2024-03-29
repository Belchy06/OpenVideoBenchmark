#pragma once

#include "ovb_recv/decoders/decoder_result.h"
#include "libde265/de265.h"

class HevcResult : public DecodeResult
{
public:
	HevcResult(de265_error InHevcReturn)
		: HevcReturn(InHevcReturn) {}

	virtual bool		IsSuccess() override { return HevcReturn == DE265_OK; }
	virtual std::string Error() override
	{
		switch (HevcReturn)
		{
			case DE265_OK:
				return "DE265_OK";
			case DE265_ERROR_NO_SUCH_FILE:
				return "DE265_ERROR_NO_SUCH_FILE";
			case DE265_ERROR_COEFFICIENT_OUT_OF_IMAGE_BOUNDS:
				return "DE265_ERROR_COEFFICIENT_OUT_OF_IMAGE_BOUNDS";
			case DE265_ERROR_CHECKSUM_MISMATCH:
				return "DE265_ERROR_CHECKSUM_MISMATCH";
			case DE265_ERROR_CTB_OUTSIDE_IMAGE_AREA:
				return "DE265_ERROR_CTB_OUTSIDE_IMAGE_AREA";
			case DE265_ERROR_OUT_OF_MEMORY:
				return "DE265_ERROR_OUT_OF_MEMORY";
			case DE265_ERROR_CODED_PARAMETER_OUT_OF_RANGE:
				return "DE265_ERROR_CODED_PARAMETER_OUT_OF_RANGE";
			case DE265_ERROR_IMAGE_BUFFER_FULL:
				return "DE265_ERROR_IMAGE_BUFFER_FULL";
			case DE265_ERROR_CANNOT_START_THREADPOOL:
				return "DE265_ERROR_CANNOT_START_THREADPOOL";
			case DE265_ERROR_LIBRARY_INITIALIZATION_FAILED:
				return "DE265_ERROR_LIBRARY_INITIALIZATION_FAILED";
			case DE265_ERROR_LIBRARY_NOT_INITIALIZED:
				return "DE265_ERROR_LIBRARY_NOT_INITIALIZED";
			case DE265_ERROR_WAITING_FOR_INPUT_DATA:
				return "DE265_ERROR_WAITING_FOR_INPUT_DATA";
			case DE265_ERROR_CANNOT_PROCESS_SEI:
				return "DE265_ERROR_CANNOT_PROCESS_SEI";
			case DE265_ERROR_PARAMETER_PARSING:
				return "DE265_ERROR_PARAMETER_PARSING";
			case DE265_ERROR_NO_INITIAL_SLICE_HEADER:
				return "DE265_ERROR_NO_INITIAL_SLICE_HEADER";
			case DE265_ERROR_PREMATURE_END_OF_SLICE:
				return "DE265_ERROR_PREMATURE_END_OF_SLICE";
			case DE265_ERROR_UNSPECIFIED_DECODING_ERROR:
				return "DE265_ERROR_UNSPECIFIED_DECODING_ERROR";
			case DE265_ERROR_NOT_IMPLEMENTED_YET:
				return "DE265_ERROR_NOT_IMPLEMENTED_YET";
			case DE265_WARNING_NO_WPP_CANNOT_USE_MULTITHREADING:
				return "DE265_WARNING_NO_WPP_CANNOT_USE_MULTITHREADING";
			case DE265_WARNING_WARNING_BUFFER_FULL:
				return "DE265_WARNING_WARNING_BUFFER_FULL";
			case DE265_WARNING_PREMATURE_END_OF_SLICE_SEGMENT:
				return "DE265_WARNING_PREMATURE_END_OF_SLICE_SEGMENT";
			case DE265_WARNING_INCORRECT_ENTRY_POINT_OFFSET:
				return "DE265_WARNING_INCORRECT_ENTRY_POINT_OFFSET";
			case DE265_WARNING_CTB_OUTSIDE_IMAGE_AREA:
				return "DE265_WARNING_CTB_OUTSIDE_IMAGE_AREA";
			case DE265_WARNING_SPS_HEADER_INVALID:
				return "DE265_WARNING_SPS_HEADER_INVALID";
			case DE265_WARNING_PPS_HEADER_INVALID:
				return "DE265_WARNING_PPS_HEADER_INVALID";
			case DE265_WARNING_SLICEHEADER_INVALID:
				return "DE265_WARNING_SLICEHEADER_INVALID";
			case DE265_WARNING_INCORRECT_MOTION_VECTOR_SCALING:
				return "DE265_WARNING_INCORRECT_MOTION_VECTOR_SCALING";
			case DE265_WARNING_NONEXISTING_PPS_REFERENCED:
				return "DE265_WARNING_NONEXISTING_PPS_REFERENCED";
			case DE265_WARNING_NONEXISTING_SPS_REFERENCED:
				return "DE265_WARNING_NONEXISTING_SPS_REFERENCED";
			case DE265_WARNING_BOTH_PREDFLAGS_ZERO:
				return "DE265_WARNING_BOTH_PREDFLAGS_ZERO";
			case DE265_WARNING_NONEXISTING_REFERENCE_PICTURE_ACCESSED:
				return "DE265_WARNING_NONEXISTING_REFERENCE_PICTURE_ACCESSED";
			case DE265_WARNING_NUMMVP_NOT_EQUAL_TO_NUMMVQ:
				return "DE265_WARNING_NUMMVP_NOT_EQUAL_TO_NUMMVQ";
			case DE265_WARNING_NUMBER_OF_SHORT_TERM_REF_PIC_SETS_OUT_OF_RANGE:
				return "DE265_WARNING_NUMBER_OF_SHORT_TERM_REF_PIC_SETS_OUT_OF_RANGE";
			case DE265_WARNING_SHORT_TERM_REF_PIC_SET_OUT_OF_RANGE:
				return "DE265_WARNING_SHORT_TERM_REF_PIC_SET_OUT_OF_RANGE";
			case DE265_WARNING_FAULTY_REFERENCE_PICTURE_LIST:
				return "DE265_WARNING_FAULTY_REFERENCE_PICTURE_LIST";
			case DE265_WARNING_EOSS_BIT_NOT_SET:
				return "DE265_WARNING_EOSS_BIT_NOT_SET";
			case DE265_WARNING_MAX_NUM_REF_PICS_EXCEEDED:
				return "DE265_WARNING_MAX_NUM_REF_PICS_EXCEEDED";
			case DE265_WARNING_INVALID_CHROMA_FORMAT:
				return "DE265_WARNING_INVALID_CHROMA_FORMAT";
			case DE265_WARNING_SLICE_SEGMENT_ADDRESS_INVALID:
				return "DE265_WARNING_SLICE_SEGMENT_ADDRESS_INVALID";
			case DE265_WARNING_DEPENDENT_SLICE_WITH_ADDRESS_ZERO:
				return "DE265_WARNING_DEPENDENT_SLICE_WITH_ADDRESS_ZERO";
			case DE265_WARNING_NUMBER_OF_THREADS_LIMITED_TO_MAXIMUM:
				return "DE265_WARNING_NUMBER_OF_THREADS_LIMITED_TO_MAXIMUM";
			case DE265_NON_EXISTING_LT_REFERENCE_CANDIDATE_IN_SLICE_HEADER:
				return "DE265_NON_EXISTING_LT_REFERENCE_CANDIDATE_IN_SLICE_HEADER";
			case DE265_WARNING_CANNOT_APPLY_SAO_OUT_OF_MEMORY:
				return "DE265_WARNING_CANNOT_APPLY_SAO_OUT_OF_MEMORY";
			case DE265_WARNING_SPS_MISSING_CANNOT_DECODE_SEI:
				return "DE265_WARNING_SPS_MISSING_CANNOT_DECODE_SEI";
			case DE265_WARNING_COLLOCATED_MOTION_VECTOR_OUTSIDE_IMAGE_AREA:
				return "DE265_WARNING_COLLOCATED_MOTION_VECTOR_OUTSIDE_IMAGE_AREA";
			case DE265_WARNING_PCM_BITDEPTH_TOO_LARGE:
				return "DE265_WARNING_PCM_BITDEPTH_TOO_LARGE";
			case DE265_WARNING_REFERENCE_IMAGE_BIT_DEPTH_DOES_NOT_MATCH:
				return "DE265_WARNING_REFERENCE_IMAGE_BIT_DEPTH_DOES_NOT_MATCH";
			case DE265_WARNING_REFERENCE_IMAGE_SIZE_DOES_NOT_MATCH_SPS:
				return "DE265_WARNING_REFERENCE_IMAGE_SIZE_DOES_NOT_MATCH_SPS";
			case DE265_WARNING_CHROMA_OF_CURRENT_IMAGE_DOES_NOT_MATCH_SPS:
				return "DE265_WARNING_CHROMA_OF_CURRENT_IMAGE_DOES_NOT_MATCH_SPS";
			case DE265_WARNING_BIT_DEPTH_OF_CURRENT_IMAGE_DOES_NOT_MATCH_SPS:
				return "DE265_WARNING_BIT_DEPTH_OF_CURRENT_IMAGE_DOES_NOT_MATCH_SPS";
			case DE265_WARNING_REFERENCE_IMAGE_CHROMA_FORMAT_DOES_NOT_MATCH:
				return "DE265_WARNING_REFERENCE_IMAGE_CHROMA_FORMAT_DOES_NOT_MATCH";
			case DE265_WARNING_INVALID_SLICE_HEADER_INDEX_ACCESS:
				return "DE265_WARNING_INVALID_SLICE_HEADER_INDEX_ACCESS";
			default:
				return "";
		}
	}

private:
	de265_error HevcReturn;
};