#include "MsgLoginProofA.h"

MsgLoginProofA::MsgLoginProofA(const std::string& P, const std::string& G, const std::string& A)
    : NetMsg(sizeof(Info) + P.length() + G.length() + A.length() - 3 * sizeof(uint8_t))	// the 3 P, G & A placeholders
{
    Info* info = (Info*)_buffer.get();
	srand(0);
	for (auto i{ 0 }; i < MsgLoginProofA::PADDING_LEN; i++)
		info->padding[i] = rand();
	info->junk_size= MsgLoginProofA::JUNK_LEN;
	for (auto i{ 0 }; i < MsgLoginProofA::JUNK_LEN; i++)
		info->junk[i] = rand();

	info->size = (int32_t)(_size - MsgLoginProofA::PADDING_LEN + 8);
	info->en_iv_size= 8;
	memset(info->en_iv, 0x00, 8);
	info->de_iv_size = 8;
	memset(info->de_iv, 0x00, 8);

	uint8_t* ptr = (uint8_t*)&info->p_size;

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
