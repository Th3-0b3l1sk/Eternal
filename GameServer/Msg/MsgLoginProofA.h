#pragma once
#include "C:\Dev\Eternal\EternalCore\include\Msg\NetMsg.h"

class MsgLoginProofA : public Eternal::Msg::NetMsg
{
    static const size_t PADDING_LEN = 11;
    static const size_t JUNK_LEN = 12;

private:
#pragma pack(push, 1)
    struct Info
    {
		uint8_t Pading[MsgLoginProofA::PADDING_LEN];
		int32_t Size;
		int32_t JunkSize;
		uint8_t Junk[MsgLoginProofA::JUNK_LEN];
		int32_t EnIVSize;
		uint8_t EnIV[8];
		int32_t DeIVSize;
		uint8_t DeIV[8];
		int32_t PSize;
		char P[1];
		int32_t GSize;
		char G[1];
		int32_t ASize;
		char A[1];
    };
#pragma pack(pop)

public:
	MsgLoginProofA(const std::string& P, const std::string& G, const std::string& A);
	MsgLoginProofA();
};

