#include "Main.h"

//JsonModel class 부분

//Json.h에 대한 선언부 .cpp파일
using namespace JSON;

//<JNode 클래스>---------------------------------------------------------------------------------------------------------
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



//<JObj 클래스>----------------------------------------------------------------------------------------------------------
//JObj class 부분
void JObj::Set_Key(const char* k) {
	key.Set_Str(k); // 키 값 설정
}

void JObj::Set_Value(JNode::JType nodeType) {
	value->Set_Type(nodeType);
}


JNode* JObj::Get_Value() {
	return value;
}


//<JArr 클래스>---------------------------------------------------------------------------------------------------------
//JArr class 부분
void JArr::Set_Value(JNode::JType nodeType) {
	value->Set_Type(nodeType); // 배열 요소의 타입 설정
}
void JArr::Set_Next(JArr* nextNode) {
	next = nextNode; // 다음 배열 요소 설정
}



//<JsonData 클래스>-----------------------------------------------------------------------------------------------------

//<반환>
void JsonData::operator=(int num){
	//Object 타입이 아닐때
	NO_NODE_CK()

	if (isAssignable){ //키값이 존재함 덮어씌워줘야함
		JsonCtrl* Value = new JsonCtrl(focus_obj->value);
			
		*Value = num;

	}
	else{//대입불가능하다면? 이런경우는 없도록?
		return;
	}
}

void JsonData::operator=(int* num){
	//Object 타입이 아닐때
	NO_NODE_CK()

		if (isAssignable){ //대입가능하다면 덮어씌우기
		JsonCtrl* Value = new JsonCtrl(focus_obj->value);

		*Value = *num;
		}
		else{//대입불가능하다면? 이런경우는 없도록?
			return;
		}
}

void JsonData::operator=(double dnum){
	//Object 타입이 아닐때
	NO_NODE_CK()

		if (isAssignable){ //대입가능하다면 덮어씌우기
		JsonCtrl* Value = new JsonCtrl(focus_obj->value);

		*Value = dnum;
		}
		else{//대입불가능하다면? 이런경우는 없도록?
			return;
		}
}

void JsonData::operator=(double* dnum){
	//Object 타입이 아닐때
	NO_NODE_CK()

	if (isAssignable){ //대입가능하다면 덮어씌우기
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = *dnum;
	}
	else{//대입불가능하다면? 이런경우는 없도록?
		return;
	}
}

void JsonData::operator=(bool boolean){
	//Object 타입이 아닐때
	NO_NODE_CK()

	if (isAssignable){ //대입가능하다면 덮어씌우기
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = boolean;
	}
	else{//대입불가능하다면? 이런경우는 없도록?
		return;
	}
}

void JsonData::operator=(bool* boolean){
	//Object 타입이 아닐때
	NO_NODE_CK()

	if (isAssignable){ //대입가능하다면 덮어씌우기
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = *boolean;
	}
	else{//대입불가능하다면? 이런경우는 없도록?
		return;
	}
}

void JsonData::operator=(const char* str){
	//Object 타입이 아닐때
	NO_NODE_CK()

	if (isAssignable){ //대입가능하다면 덮어씌우기
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = str;
	}
	else{//대입불가능하다면? 이런경우는 없도록?
		return;
	}
}

void JsonData::operator=(const char c){
	//Object 타입이 아닐때
	NO_NODE_CK()

	if (isAssignable){ //대입가능하다면 덮어씌우기
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = c;
	}
	else{//대입불가능하다면? 이런경우는 없도록?
		return;
	}
}

//<대입>
JsonData::operator int(){
	NO_NODE_CK(-1)

	if (isReturnable){//키값이 존재함 Type이 맞는지 해당 영역에서 비교해줘야함
		if (focus_obj->value->type == JNode::JType::NUMBER){//반환하고자 하는 lValue와 타입이 같음
			int* num = static_cast<int*>(focus_obj->value->ptype);
			return *num;
		}
		else{//반환하고자 하는 lValue와 타입이 다름
			printf("반환 타입이 다릅니다.\n");
			return -1;
		}
	}
	else{//키값 자체도 존재하지 않음 실패 반환해줘야함
		printf("키값이 없습니다.\n");
		return -1;
	}
}

JsonData::operator int*(){
	NO_NODE_CK(nullptr)
	if (isReturnable){//키값이 존재함 Type이 맞는지 해당 영역에서 비교해줘야함
		if (focus_obj->value->type == JNode::JType::NUMBER){//반환하고자 하는 lValue와 타입이 같음
			int* num = static_cast<int*>(focus_obj->value->ptype);
			return num;
		}
		else{//반환하고자 하는 lValue와 타입이 다름
			printf("반환 타입이 다릅니다.\n");
			return nullptr;
		}
	}
	else{//키값 자체도 존재하지 않음 실패 반환해줘야함
		printf("키값이 없습니다.\n");
		return nullptr;
	}
}

JsonData::operator double(){
	NO_NODE_CK(-1.0)

	if (isReturnable){//키값이 존재함 Type이 맞는지 해당 영역에서 비교해줘야함
		if (focus_obj->value->type == JNode::JType::DOUBLE){//반환하고자 하는 lValue와 타입이 같음
			double* dnum = static_cast<double*>(focus_obj->value->ptype);
			return *dnum;
		}
		else{//반환하고자 하는 lValue와 타입이 다름
			printf("반환 타입이 다릅니다.\n");
			return -1.0;
		}
	}
	else{//키값 자체도 존재하지 않음 실패 반환해줘야함
		printf("키값이 없습니다.\n");
		return -1.0;
	}
}

JsonData::operator double*(){
	NO_NODE_CK(nullptr)

	if (isReturnable){//키값이 존재함 Type이 맞는지 해당 영역에서 비교해줘야함
		if (focus_obj->value->type == JNode::JType::DOUBLE){//반환하고자 하는 lValue와 타입이 같음
			double* dnum = static_cast<double*>(focus_obj->value->ptype);
			return dnum;
		}
		else{//반환하고자 하는 lValue와 타입이 다름
			printf("반환 타입이 다릅니다.\n");
			return nullptr;
		}
	}
	else{//키값 자체도 존재하지 않음 실패 반환해줘야함
		printf("키값이 없습니다.\n");
		return nullptr;
	}
}

JsonData::operator bool(){
	NO_NODE_CK(false)

	if (isReturnable){//키값이 존재함 Type이 맞는지 해당 영역에서 비교해줘야함
		if (focus_obj->value->type == JNode::JType::BOOL){//반환하고자 하는 lValue와 타입이 같음
			bool* bl = static_cast<bool*>(focus_obj->value->ptype);
			return *bl;
		}
		else{//반환하고자 하는 lValue와 타입이 다름
			printf("반환 타입이 다릅니다.\n");
			return false;
		}
	}
	else{//키값 자체도 존재하지 않음 실패 반환해줘야함
		printf("키값이 없습니다.\n");
		return false;
	}
}

JsonData::operator bool*(){
	NO_NODE_CK(nullptr)

	if (isReturnable){//키값이 존재함 Type이 맞는지 해당 영역에서 비교해줘야함
		if (focus_obj->value->type == JNode::JType::BOOL){//반환하고자 하는 lValue와 타입이 같음
			bool* bl = static_cast<bool*>(focus_obj->value->ptype);
			return bl;
		}
		else{//반환하고자 하는 lValue와 타입이 다름
			printf("반환 타입이 다릅니다.\n");
			return nullptr;
		}
	}
	else{//키값 자체도 존재하지 않음 실패 반환해줘야함
		printf("키값이 없습니다.\n");
		return nullptr;
	}
}

JsonData::operator char(){
	NO_NODE_CK('\0')

	if (isReturnable){//키값이 존재함 Type이 맞는지 해당 영역에서 비교해줘야함
		if (focus_obj->value->type == JNode::JType::CHAR){//반환하고자 하는 lValue와 타입이 같음
			char* c = static_cast<char*>(focus_obj->value->ptype);
			return *c;
		}
		else{//반환하고자 하는 lValue와 타입이 다름
			printf("반환 타입이 다릅니다.\n");
			return '\0';
		}
	}
	else{//키값 자체도 존재하지 않음 실패 반환해줘야함
		printf("키값이 없습니다.\n");
		return '\0';
	}
}

JsonData::operator char*(){
	NO_NODE_CK(nullptr)
	if (isReturnable){//키값이 존재함 Type이 맞는지 해당 영역에서 비교해줘야함
		if (focus_obj->value->type == JNode::JType::STRING){//반환하고자 하는 lValue와 타입이 같음
			Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(focus_obj->value->ptype);
			return str->Get_Str();
		}
		else{//반환하고자 하는 lValue와 타입이 다름
			printf("반환 타입이 다릅니다.\n");
			return nullptr;
		}
	}
	else{//키값 자체도 존재하지 않음 실패 반환해줘야함
		printf("키값이 없습니다.\n");
		return nullptr;
	}
}




//<JsonCtrl 클래스>-----------------------------------------------------------------------------------------------------




