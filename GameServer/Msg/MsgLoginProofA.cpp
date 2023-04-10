#include "MsgLoginProofA.h"

MsgLoginProofA::MsgLoginProofA(const std::string& P, const std::string& G, const std::string& A)
    : NetMsg(sizeof(Info) + P.length() + G.length() + A.length() - 3 * sizeof(uint8_t))	// the 3 P, G & A placeholders
{
    Info* info = (Info*)_buffer.get();
	srand(0);

	for (auto i{ 0 }; i < MsgLoginProofA::PADDING_LEN; i++)
		info->Pading[i] = rand();

	info->JunkSize = MsgLoginProofA::JUNK_LEN;
	for (auto i{ 0 }; i < MsgLoginProofA::JUNK_LEN; i++)
		info->Junk[i] = rand();

	info->Size = (int32_t)(_size - MsgLoginProofA::PADDING_LEN + 8);
	info->EnIVSize = 8;
	memset(info->EnIV, 0x00, 8);
	info->DeIVSize = 8;
	memset(info->DeIV, 0x00, 8);

	uint8_t* ptr = (uint8_t*)&info->PSize;

	*((int32_t*)(ptr)) = (int32_t)P.length();
	ptr += sizeof(int32_t);
	memcpy(ptr, P.c_str(), P.length());
	ptr += P.length();

	*((int32_t*)(ptr)) = (int32_t)G.length();
	ptr += sizeof(int32_t);
	memcpy(ptr, G.c_str(), G.length());
	ptr += G.length();

	*((int32_t*)(ptr)) = (int32_t)A.length();
	ptr += sizeof(int32_t);
	memcpy(ptr, A.c_str(), A.length());
	ptr += A.length();
}
