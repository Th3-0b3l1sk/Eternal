#pragma once
#include "Msg\NetMsg.h"

class MsgLoginProofA : public Eternal::Msg::NetMsg
{
    static const size_t PADDING_LEN = 11;
    static const size_t JUNK_LEN	= 12;

private:
#pragma pack(push, 1)
    struct Info
    {
		uint8_t padding[MsgLoginProofA::PADDING_LEN];
		int32_t size;
		int32_t junk_size;
		uint8_t junk[MsgLoginProofA::JUNK_LEN];
		int32_t en_iv_size;
		uint8_t en_iv[8];
		int32_t de_iv_size;
		uint8_t de_iv[8];
		int32_t p_size;
		char P[1];
		int32_t g_size;
		char G[1];
		int32_t a_size;
		char A[1];
    };
#pragma pack(pop)

public:
	MsgLoginProofA(const std::string& P, const std::string& G, const std::string& A);
	MsgLoginProofA();
};

