#include "Main.h"

//JsonModel class 부분

using namespace JSON;

void JNode::Set_Type(JType nodeType) {
	type = nodeType;
	switch (type) {
	case JType::OBJECT:
		ptype = new JObj();
		ObjCnt = -1;
		break;
	case JType::ARRAY:
		ptype = new JArr();
		ArrCnt = -1;
		break;
	case JType::STRING:
		ptype = new Dynamic::DynamicStr(128);
		break;
	case JType::NUMBER:
		ptype = new int(); // 기본적으로 int로 초기화
		break;
	case JType::BOOL:
		ptype = new bool();
		break;
	case JType::DOUBLE:
		ptype = new double();
		break;
	case JType::NULL_TYPE:
		ptype = nullptr; // null 타입은 포인터를 사용하지 않음
		ObjCnt = -2;
		ArrCnt = -2;
		break;
	default:
		ptype = nullptr; // 기본적으로 nullptr로 초기화
		break;
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








