#include "Main.h"

//JsonModel class �κ�

//Json.h�� ���� ����� .cpp����
using namespace JSON;

//<JNode Ŭ����>---------------------------------------------------------------------------------------------------------
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
			ptype = new int(); // �⺻������ int�� �ʱ�ȭ
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
			ptype = nullptr; // null Ÿ���� �����͸� ������� ����
			ObjCnt = -2;
			ArrCnt = -2;
			break;
		}
		default:
		{
			ptype = nullptr; // �⺻������ nullptr�� �ʱ�ȭ
			break;
		}
	}
}

void* JNode::Get_Ptype() {
	return ptype;
}



//<JObj Ŭ����>----------------------------------------------------------------------------------------------------------
//JObj class �κ�
void JObj::Set_Key(const char* k) {
	key.Set_Str(k); // Ű �� ����
}

void JObj::Set_Value(JNode::JType nodeType) {
	value->Set_Type(nodeType);
}


JNode* JObj::Get_Value() {
	return value;
}


//<JArr Ŭ����>---------------------------------------------------------------------------------------------------------
//JArr class �κ�
void JArr::Set_Value(JNode::JType nodeType) {
	value->Set_Type(nodeType); // �迭 ����� Ÿ�� ����
}
void JArr::Set_Next(JArr* nextNode) {
	next = nextNode; // ���� �迭 ��� ����
}



//<JsonData Ŭ����>-----------------------------------------------------------------------------------------------------

//<��ȯ>
void JsonData::operator=(int num){
	//Object Ÿ���� �ƴҶ�
	NO_NODE_CK()

	if (isAssignable){ //Ű���� ������ ����������
		JsonCtrl* Value = new JsonCtrl(focus_obj->value);
			
		*Value = num;

	}
	else{//���ԺҰ����ϴٸ�? �̷����� ������?
		return;
	}
}

void JsonData::operator=(int* num){
	//Object Ÿ���� �ƴҶ�
	NO_NODE_CK()

		if (isAssignable){ //���԰����ϴٸ� ������
		JsonCtrl* Value = new JsonCtrl(focus_obj->value);

		*Value = *num;
		}
		else{//���ԺҰ����ϴٸ�? �̷����� ������?
			return;
		}
}

void JsonData::operator=(double dnum){
	//Object Ÿ���� �ƴҶ�
	NO_NODE_CK()

		if (isAssignable){ //���԰����ϴٸ� ������
		JsonCtrl* Value = new JsonCtrl(focus_obj->value);

		*Value = dnum;
		}
		else{//���ԺҰ����ϴٸ�? �̷����� ������?
			return;
		}
}

void JsonData::operator=(double* dnum){
	//Object Ÿ���� �ƴҶ�
	NO_NODE_CK()

	if (isAssignable){ //���԰����ϴٸ� ������
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = *dnum;
	}
	else{//���ԺҰ����ϴٸ�? �̷����� ������?
		return;
	}
}

void JsonData::operator=(bool boolean){
	//Object Ÿ���� �ƴҶ�
	NO_NODE_CK()

	if (isAssignable){ //���԰����ϴٸ� ������
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = boolean;
	}
	else{//���ԺҰ����ϴٸ�? �̷����� ������?
		return;
	}
}

void JsonData::operator=(bool* boolean){
	//Object Ÿ���� �ƴҶ�
	NO_NODE_CK()

	if (isAssignable){ //���԰����ϴٸ� ������
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = *boolean;
	}
	else{//���ԺҰ����ϴٸ�? �̷����� ������?
		return;
	}
}

void JsonData::operator=(const char* str){
	//Object Ÿ���� �ƴҶ�
	NO_NODE_CK()

	if (isAssignable){ //���԰����ϴٸ� ������
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = str;
	}
	else{//���ԺҰ����ϴٸ�? �̷����� ������?
		return;
	}
}

void JsonData::operator=(const char c){
	//Object Ÿ���� �ƴҶ�
	NO_NODE_CK()

	if (isAssignable){ //���԰����ϴٸ� ������
	JsonCtrl* Value = new JsonCtrl(focus_obj->value);

	*Value = c;
	}
	else{//���ԺҰ����ϴٸ�? �̷����� ������?
		return;
	}
}

//<����>
JsonData::operator int(){
	NO_NODE_CK(-1)

	if (isReturnable){//Ű���� ������ Type�� �´��� �ش� �������� ���������
		if (focus_obj->value->type == JNode::JType::NUMBER){//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� ����
			int* num = static_cast<int*>(focus_obj->value->ptype);
			return *num;
		}
		else{//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� �ٸ�
			printf("��ȯ Ÿ���� �ٸ��ϴ�.\n");
			return -1;
		}
	}
	else{//Ű�� ��ü�� �������� ���� ���� ��ȯ�������
		printf("Ű���� �����ϴ�.\n");
		return -1;
	}
}

JsonData::operator int*(){
	NO_NODE_CK(nullptr)
	if (isReturnable){//Ű���� ������ Type�� �´��� �ش� �������� ���������
		if (focus_obj->value->type == JNode::JType::NUMBER){//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� ����
			int* num = static_cast<int*>(focus_obj->value->ptype);
			return num;
		}
		else{//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� �ٸ�
			printf("��ȯ Ÿ���� �ٸ��ϴ�.\n");
			return nullptr;
		}
	}
	else{//Ű�� ��ü�� �������� ���� ���� ��ȯ�������
		printf("Ű���� �����ϴ�.\n");
		return nullptr;
	}
}

JsonData::operator double(){
	NO_NODE_CK(-1.0)

	if (isReturnable){//Ű���� ������ Type�� �´��� �ش� �������� ���������
		if (focus_obj->value->type == JNode::JType::DOUBLE){//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� ����
			double* dnum = static_cast<double*>(focus_obj->value->ptype);
			return *dnum;
		}
		else{//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� �ٸ�
			printf("��ȯ Ÿ���� �ٸ��ϴ�.\n");
			return -1.0;
		}
	}
	else{//Ű�� ��ü�� �������� ���� ���� ��ȯ�������
		printf("Ű���� �����ϴ�.\n");
		return -1.0;
	}
}

JsonData::operator double*(){
	NO_NODE_CK(nullptr)

	if (isReturnable){//Ű���� ������ Type�� �´��� �ش� �������� ���������
		if (focus_obj->value->type == JNode::JType::DOUBLE){//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� ����
			double* dnum = static_cast<double*>(focus_obj->value->ptype);
			return dnum;
		}
		else{//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� �ٸ�
			printf("��ȯ Ÿ���� �ٸ��ϴ�.\n");
			return nullptr;
		}
	}
	else{//Ű�� ��ü�� �������� ���� ���� ��ȯ�������
		printf("Ű���� �����ϴ�.\n");
		return nullptr;
	}
}

JsonData::operator bool(){
	NO_NODE_CK(false)

	if (isReturnable){//Ű���� ������ Type�� �´��� �ش� �������� ���������
		if (focus_obj->value->type == JNode::JType::BOOL){//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� ����
			bool* bl = static_cast<bool*>(focus_obj->value->ptype);
			return *bl;
		}
		else{//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� �ٸ�
			printf("��ȯ Ÿ���� �ٸ��ϴ�.\n");
			return false;
		}
	}
	else{//Ű�� ��ü�� �������� ���� ���� ��ȯ�������
		printf("Ű���� �����ϴ�.\n");
		return false;
	}
}

JsonData::operator bool*(){
	NO_NODE_CK(nullptr)

	if (isReturnable){//Ű���� ������ Type�� �´��� �ش� �������� ���������
		if (focus_obj->value->type == JNode::JType::BOOL){//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� ����
			bool* bl = static_cast<bool*>(focus_obj->value->ptype);
			return bl;
		}
		else{//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� �ٸ�
			printf("��ȯ Ÿ���� �ٸ��ϴ�.\n");
			return nullptr;
		}
	}
	else{//Ű�� ��ü�� �������� ���� ���� ��ȯ�������
		printf("Ű���� �����ϴ�.\n");
		return nullptr;
	}
}

JsonData::operator char(){
	NO_NODE_CK('\0')

	if (isReturnable){//Ű���� ������ Type�� �´��� �ش� �������� ���������
		if (focus_obj->value->type == JNode::JType::CHAR){//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� ����
			char* c = static_cast<char*>(focus_obj->value->ptype);
			return *c;
		}
		else{//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� �ٸ�
			printf("��ȯ Ÿ���� �ٸ��ϴ�.\n");
			return '\0';
		}
	}
	else{//Ű�� ��ü�� �������� ���� ���� ��ȯ�������
		printf("Ű���� �����ϴ�.\n");
		return '\0';
	}
}

JsonData::operator char*(){
	NO_NODE_CK(nullptr)
	if (isReturnable){//Ű���� ������ Type�� �´��� �ش� �������� ���������
		if (focus_obj->value->type == JNode::JType::STRING){//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� ����
			Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(focus_obj->value->ptype);
			return str->Get_Str();
		}
		else{//��ȯ�ϰ��� �ϴ� lValue�� Ÿ���� �ٸ�
			printf("��ȯ Ÿ���� �ٸ��ϴ�.\n");
			return nullptr;
		}
	}
	else{//Ű�� ��ü�� �������� ���� ���� ��ȯ�������
		printf("Ű���� �����ϴ�.\n");
		return nullptr;
	}
}




//<JsonCtrl Ŭ����>-----------------------------------------------------------------------------------------------------




