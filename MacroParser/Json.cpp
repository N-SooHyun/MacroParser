#include "Main.h"

//JsonModel class 부분

using namespace JSON;

void JNode::Set_Type(JType nodeType) {
	if (ptype) {
		delete ptype; // 기존에 할당된 메모리 해제
	}
	type = nodeType;
	switch (type) {
	case JType::OBJECT:
		ptype = new JObj();
		break;
	case JType::ARRAY:
		ptype = new JArr();
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
	case JType::INVALID:
		ptype = nullptr; // null 타입은 포인터를 사용하지 않음
		break;
	default:
		ptype = nullptr; // 기본적으로 nullptr로 초기화
		break;
	}
}



//JObj class 부분
void JObj::Set_Key(const char* k) {
	key.Set_Str(k); // 키 값 설정
}

void JObj::Set_Value(JNode::JType nodeType) {
	value.Set_Type(nodeType); // 값 타입 설정
}

//JArr class 부분
void JArr::Set_Value(JNode::JType nodeType) {
	value.Set_Type(nodeType); // 배열 요소의 타입 설정
}
void JArr::Set_Next(JArr* nextNode) {
	next = nextNode; // 다음 배열 요소 설정
}






