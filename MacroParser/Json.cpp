#include "Main.h"

//JsonModel class 부분

using namespace JSON;

void JNode::Set_Type(JType nodeType) {
	type = nodeType;
	switch (type) {
		case JType::OBJECT:
		{
			ptype = new JObj();
			ObjCnt = -1;
			break;
		}
		case JType::ARRAY:
		{
			ptype = new JArr();
			ArrCnt = -1;
			break;
		}
		case JType::CHAR:
		{
			ptype = new char();
			char *init_char = static_cast<char*>(ptype);
			init_char[0] = '0';
			break;
		}
		case JType::STRING:
		{
			ptype = new Dynamic::DynamicStr(128);
			Dynamic::DynamicStr* init_str = static_cast<Dynamic::DynamicStr*>(ptype);
			init_str->Set_Str("0\0");
			break;
		}
		case JType::NUMBER:
		{
			ptype = new int(); // 기본적으로 int로 초기화
			int* init_num = static_cast<int*>(ptype);
			*init_num = 0;
			break;
		}
		case JType::BOOL:
		{
			ptype = new bool();
			bool* init_bool = static_cast<bool*>(ptype);
			*init_bool = false;
			break;
		}
		case JType::DOUBLE:
		{
			ptype = new double();
			double* init_dnum = static_cast<double*>(ptype);
			*init_dnum = 0.0;
			break;
		}
		case JType::NULL_TYPE:
		{
			ptype = nullptr; // null 타입은 포인터를 사용하지 않음
			ObjCnt = -2;
			ArrCnt = -2;
			break;
		}
		default:
		{
			ptype = nullptr; // 기본적으로 nullptr로 초기화
			break;
		}
	}
}

void* JNode::Get_Ptype() {
	return ptype;
}


//JObj class 부분
void JObj::Set_Key(const char* k) {
	key.Set_Str(k); // 키 값 설정
}

void JObj::Set_Value(JNode::JType nodeType) {
	value.Set_Type(nodeType); // 값 타입 설정
}


JNode* JObj::Get_Value() {
	return &value;
}

//JArr class 부분
void JArr::Set_Value(JNode::JType nodeType) {
	value.Set_Type(nodeType); // 배열 요소의 타입 설정
}
void JArr::Set_Next(JArr* nextNode) {
	next = nextNode; // 다음 배열 요소 설정
}








