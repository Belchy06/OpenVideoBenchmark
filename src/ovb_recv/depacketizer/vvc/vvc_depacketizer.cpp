#include "ovb_common/common.h"
#include "ovb_recv/depacketizer/vvc/vvc_depacketizer.h"
#include "vvdec/vvdec.h"

#define LogVvcDepacketizer "LogVvcDepacketizer"

void VvcDepacketizer::HandlePacket(RTPPacket InPacket)
{
	// TODO (belchy06): Reordering?

	if (InPacket.GetSequenceNumber() < PrevSequenceNumber)
	{
		// A packet has come in late, discard it
		return;
	}

	if (InPacket.GetSequenceNumber() > PrevSequenceNumber + 1)
	{
		// A packet has come in with a sequence number higher than expected, notify users of the number of packets missed
		LOG(LogVvcDepacketizer, LOG_SEVERITY_WARNING, "Missed packet(s): {} -> {}", PrevSequenceNumber + 1, InPacket.GetSequenceNumber() - 1);
	}

	// if (InPacket.GetTimeStamp() > prevTimestamp)
	// {
	// 	// A packet has come in with a newer timestamp (indicating a new NAL), but we haven't finished receiving the previous NAL
	// 	Packets.clear();
	// }

	PrevTimestamp = InPacket.GetTimeStamp();
	PrevSequenceNumber = InPacket.GetSequenceNumber();

	uint8_t* PacketData = InPacket.GetPayload();
	size_t	 PacketSize = InPacket.GetPayloadSize();
	// assert(PacketSize > 2);

	// clang-format off
	uint8_t ForbiddenZeroBit =   (PacketData[0] & 0b10000000) >> 7;
	uint8_t NuhReservedZeroBit = (PacketData[0] & 0b01000000) >> 6;
	uint8_t NuhLayerId =         (PacketData[0] & 0b00111111) >> 0;
	uint8_t NalUnitType =        (PacketData[1] & 0b11111000) >> 3;
	uint8_t NuhTemporalIdPlus1 = (PacketData[1] & 0b00000111) >> 0;
	// clang-format on
	// assert(ForbiddenZeroBit == 0);
	// assert(NuhReservedZeroBit == 0);
	LOG(LogVvcDepacketizer, LOG_SEVERITY_DETAILS, "Depacketizing NAL. Type: {}; Size: {}", +NalUnitType, PacketSize);

	if (NalUnitType != 29 && NalUnitType != 28)
	{
		// Single nal
		if (Fragments.size() > 0)
		{
			// We've received a new single nal without finishing off the last fragmented unit. Warn and continue
			LOG(LogVvcDepacketizer, LOG_SEVERITY_WARNING, "Received new single nal without finishing previously fragmented nal. Dropping {} fragments", Fragments.size());
			Fragments.clear();
		}

		std::vector<uint8_t> ReconstructedNalBytes;
		// Start sequence
		// clang-format off
		if (NalUnitType == VVC_NAL_UNIT_DCI || 
            NalUnitType == VVC_NAL_UNIT_VPS || 
            NalUnitType == VVC_NAL_UNIT_SPS || 
            NalUnitType == VVC_NAL_UNIT_PPS || 
            NalUnitType == VVC_NAL_UNIT_PREFIX_APS || 
            NalUnitType == VVC_NAL_UNIT_SUFFIX_APS)
		{
			ReconstructedNalBytes.push_back(0x0);
		}
		// clang-format on
		ReconstructedNalBytes.push_back(0x0);
		ReconstructedNalBytes.push_back(0x0);
		ReconstructedNalBytes.push_back(0x1);

		for (size_t i = 0; i < PacketSize; i++)
		{
			ReconstructedNalBytes.push_back(PacketData[i]);
		}

		DepacketizerListener->OnNALReceived(ReconstructedNalBytes.data(), ReconstructedNalBytes.size());
	}
	else if (NalUnitType == 28)
	{
		// Aggregated Packet
		if (Fragments.size() > 0)
		{
			// We've received a new AP without finishing off the last fragmented unit. Warn and continue
			LOG(LogVvcDepacketizer, LOG_SEVERITY_WARNING, "Received new aggregated packet without finishing previously fragmented nal. Dropping {} fragments", Fragments.size());
			Fragments.clear();
		}

		// Skip PayloadHdr
		PacketData += 2;
		PacketSize -= 2;

		while (PacketSize > 0)
		{
			size_t	 ReadIdx = 0;
			uint16_t NALSize = 0;
			NALSize |= PacketData[ReadIdx++] << 8;
			NALSize |= PacketData[ReadIdx++] << 0;

			ForbiddenZeroBit = (PacketData[ReadIdx] & 0b10000000) >> 7;
			NuhReservedZeroBit = (PacketData[ReadIdx] & 0b01000000) >> 6;
			NuhLayerId = (PacketData[ReadIdx] & 0b00111111) >> 0;
			ReadIdx++;
			NalUnitType = (PacketData[ReadIdx] & 0b11111000) >> 3;
			NuhTemporalIdPlus1 = (PacketData[ReadIdx] & 0b00000111) >> 0;
			ReadIdx++;

			std::vector<uint8_t> ReconstructedNalBytes;
			// Start sequence
			// clang-format off
		    if (NalUnitType == VVC_NAL_UNIT_DCI || 
                NalUnitType == VVC_NAL_UNIT_VPS || 
                NalUnitType == VVC_NAL_UNIT_SPS || 
                NalUnitType == VVC_NAL_UNIT_PPS || 
                NalUnitType == VVC_NAL_UNIT_PREFIX_APS || 
                NalUnitType == VVC_NAL_UNIT_SUFFIX_APS)
		    {
			    ReconstructedNalBytes.push_back(0x0);
		    }
			// clang-format on
			ReconstructedNalBytes.push_back(0x0);
			ReconstructedNalBytes.push_back(0x0);
			ReconstructedNalBytes.push_back(0x1);

			uint8_t NalHeaderByte;
			// clang-format off
			NalHeaderByte = 0;
            NalHeaderByte |= (ForbiddenZeroBit   << 7) & 0b10000000;
            NalHeaderByte |= (NuhReservedZeroBit << 6) & 0b01000000;
            NalHeaderByte |= (NuhLayerId         << 0) & 0b00111111;
            ReconstructedNalBytes.push_back(NalHeaderByte);
			// clang-format on

			// clang-format off
			NalHeaderByte = 0;
            NalHeaderByte |= (NalUnitType        << 3) & 0b11111000;
            NalHeaderByte |= (NuhTemporalIdPlus1 << 0) & 0b00000111;
            ReconstructedNalBytes.push_back(NalHeaderByte);
			// clang-format on

			for (size_t i = ReadIdx; i < ReadIdx + NALSize; i++)
			{
				ReconstructedNalBytes.push_back(PacketData[i]);
			}

			DepacketizerListener->OnNALReceived(ReconstructedNalBytes.data(), ReconstructedNalBytes.size());

			PacketData += (NALSize + 2);
			PacketSize -= (NALSize + 2);
		}
	}
	else if (NalUnitType == 29)
	{
		// Fragmentaion packet
		uint8_t FUHeaderByte = PacketData[2];
		// clang-format off
		uint8_t bIsFirst = (FUHeaderByte & 0b10000000) >> 7;
		uint8_t bIsLast  = (FUHeaderByte & 0b01000000) >> 6;
		uint8_t FUType =   (FUHeaderByte & 0b00011111) >> 0;
		// clang-format on

		if (bIsFirst && Fragments.size() > 0)
		{
			// We've received a new fragmented nal without finishing off the last fragmented unit. Warn and continue
			LOG(LogVvcDepacketizer, LOG_SEVERITY_WARNING, "Received new fragmented nal without finishing previously fragmented nal. Dropping {} fragments", Fragments.size());
			Fragments.clear();
		}

		Fragments.push_back(InPacket);

		if (bIsLast)
		{
			std::vector<uint8_t> ReconstructedNalBytes;
			// Start sequence
			ReconstructedNalBytes.push_back(0x0);
			ReconstructedNalBytes.push_back(0x0);
			ReconstructedNalBytes.push_back(0x1);

			std::vector<uint8_t> NalHeader;
			uint8_t				 NalHeaderByte;
			// clang-format off
			NalHeaderByte = 0;
            NalHeaderByte |= (ForbiddenZeroBit   << 7) & 0b10000000;
            NalHeaderByte |= (NuhReservedZeroBit << 6) & 0b01000000;
            NalHeaderByte |= (NuhLayerId         << 0) & 0b00111111;
            NalHeader.push_back(NalHeaderByte);
			// clang-format on

			// clang-format off
			NalHeaderByte = 0;
            NalHeaderByte |= (FUType             << 3) & 0b11111000;
            NalHeaderByte |= (NuhTemporalIdPlus1 << 0) & 0b00000111;
            NalHeader.push_back(NalHeaderByte);
			// clang-format on

			ReconstructedNalBytes.reserve(NalHeader.size() + ReconstructedNalBytes.size());
			ReconstructedNalBytes.insert(ReconstructedNalBytes.end(), NalHeader.begin(), NalHeader.end());
			for (RTPPacket Fragment : Fragments)
			{
				uint8_t* FragmentData = Fragment.GetPayload();
				for (size_t i = 3; i < Fragment.GetPayloadSize(); i++)
				{
					ReconstructedNalBytes.push_back(FragmentData[i]);
				}
			}

			DepacketizerListener->OnNALReceived(ReconstructedNalBytes.data(), ReconstructedNalBytes.size());

			Fragments.clear();
		}
	}
}

#undef LogVvcDepacketizer