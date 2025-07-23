#pragma once

namespace JSON {
	//.Json ������ ������ ���� 2��
	// ������Ÿ�� - {}, []
	// ������Ÿ�� - "str", true, 1, 123.2, null
	//1. Key�� ������ Stirng Ÿ��
	//2. Value : Number (int, float), String, Boolean (true, false), Array ([]), Object ({}), null

	//---------------------------------------------------------------------------------
	//-------------------------------Json�ڷᱸ�� �κ�---------------------------------
	//---------------------------------------------------------------------------------

	class JNodeBase {
	public:
		JNodeBase(){}
		virtual ~JNodeBase() {}
	};

	class JNode {
	public:
		enum class JType {
			//������ Ÿ��
			OBJECT,		//{}
			ARRAY,		//[]
			//������ Ÿ��
			STRING,		//���ڿ�
			CHAR,		//����Ÿ��
			NUMBER,		//����
			BOOL,		//true false
			DOUBLE,		//�Ҽ�
			NULL_TYPE,	//null
		};
		JNode() : type(JType::NULL_TYPE), ptype(nullptr), ObjCnt(-2), ArrCnt(-2){}
		JNode(JType nodeType) : type(nodeType), ptype(nullptr) {
			Set_Type(nodeType); // Ÿ�� ����
		}

		void Set_Type(JType nodeType);
		void* Get_Ptype();
		template<typename T>
		JNode::JType GetJsonType();

		//���������
		JNode(const JNode& other) : type(other.type), ptype(nullptr), ObjCnt(other.ObjCnt), ArrCnt(other.ArrCnt){
			Set_Type(other.type); //ptype ����
		}

		~JNode() {
			//delete ptype;
			if (type == JNode::JType::ARRAY || type == JNode::JType::OBJECT) {
				// JObj�� JArr Ÿ���� ���, ptype�� �ش� Ÿ���� ��ü�� ����Ű�Ƿ� ���� �Ҵ� ����
				delete static_cast<JNodeBase*>(ptype);
			}
			else if (type == JNode::JType::STRING) {
				// DynamicStr Ÿ���� ���, ptype�� ���ڿ��� ����Ű�Ƿ� ���� �Ҵ� ����
				delete static_cast<Dynamic::DynamicStr*>(ptype);
			}
			else if (type == JNode::JType::NUMBER || type == JNode::JType::BOOL || type == JNode::JType::DOUBLE) {
				delete ptype;
			}
			ptype = nullptr;
			type = JNode::JType::NULL_TYPE; // Ÿ���� INVALID�� �����Ͽ� �����ϰ� �ʱ�ȭ
		}

		//private:
		JType type; //��� Ÿ��
		void* ptype; //��� Ÿ�Կ� ���� ������ ������
		int ObjCnt; 
		int ArrCnt;

		friend class JObj;
		friend class JArr;
		friend class JsonCtrl;
		friend class JsonAPI;
	};


	class JObj : public JNodeBase {
	public:
		JObj() : key(128), value(JNode::JType::NULL_TYPE), next(nullptr){}
		~JObj() {
			if (next != nullptr) {
				delete next;
				next = nullptr;
			}
		}

		//��������� ��������
		JObj(const JObj& other) : key(other.key), value(other.value), next(other.next){			
		}

		//�̵�������
		JObj(JObj&& other) : key(other.key), value(other.value), next(other.next){
		}

		void Set_Key(const char* k);  // Ű �� ����
		void Set_Value(JNode::JType nodeType);// �� Ÿ�� ����
		JNode* Get_Value();

	//private:
		Dynamic::DynamicStr key; // Ű ��
		JNode value; // �� (Json_Model Ÿ������ ����)
		JObj* next; // ���� ��ü�� ����Ű�� ������ (���� ����Ʈ ���·� ���� ����)
		friend class JArr;
		friend class JsonAPI;
	};


	class JArr : public JNodeBase{
	public:
		JArr() : next(nullptr), value(JNode::JType::NULL_TYPE){}
		~JArr() {
			if (next != nullptr) {
				delete next;
				next = nullptr;
			}
		}

		//��������� ��������
		JArr(const JArr& other) : value(other.value), next(other.next){}

		//�̵�������
		JArr(const JArr&& other) : value(other.value), next(other.next){}

		void Set_Value(JNode::JType nodeType);
		void Set_Next(JArr* nextNode);

	//private:
		JArr* next; // ���� �迭 ��Ҹ� ����Ű�� ������ (���� ����Ʈ ���·� ���� ����)
		JNode value; // �迭 ����� �� (Json_Model Ÿ������ ����)

		friend class JsonAPI;
	};


	//---------------------------------------------------------------------------------
	//-------------------------------Json����� ȣ��κ�--------------------------------
	//---------------------------------------------------------------------------------

	//JSON ��� ��Ģ
	// {} ��ü�϶�
	// JNode Js = "{"Key" : "Value"}"; 
	// 1. Insert �߰��ϱ�
	// Js.a = "b";  -> {"Key" : "Value", "a" : "b"}
	// Js["a"] = "b";  -> {"Key" : "Value", "a" : "b"}
	// char* var = "a";    Js[var] = "b";  -> {"Key" : "Value", "a" : "b"}

	// 2. Get �� �������� �� ã��?
	// Js.get("a");  -> "b"
	// Js["a"];  -> "b"
	// char* var = "a";    Js[var];  -> "b"

	// 3. Delete �����ϱ�
	// Js.del("a");  -> {"Key" : "Value"}
	// Js["a"].del();  -> {"Key" : "Value"}
	// char* var = "a";    Js[var].del();  -> {"Key" : "Value"}

	// 4. Clear ����
	// Js.clear();  -> {}


	// [] Arr�϶�
	// 1. Insert �߰��ϱ�
	// Js[0] = "a";  -> ["a"]
	// Js[1] = "b";  -> ["a", "b"]
	// int idx = 2;    Js[idx] = "c";  -> ["a", "b", "c"]

	// 2. Get �� �������� �� ã��?
	// Js[0];  -> "a"
	// Js[1];  -> "b"
	// int idx = 2;    Js[idx];  -> "c"

	// 3. Delete �����ϱ�
	// Js[0].del();  -> ["b", "c"]

	// 4. Clear ����
	// Js.clear();  -> []


	// ���� Ÿ���϶�
	// 1. Insert �߰��ϱ� Ȥ�� ��������?
	// Js = "Hello";  -> "Hello"
	// Js = 123;  -> 123

	// 2. Get �� ��������
	// Js.get();  -> "Hello" Ȥ�� 123
	// Js.get<int>();  -> 123

	// 3. Delete �����ϱ�
	// Js.del();  -> null Ȥ�� 0

	// 4. Clear ����
	// Js.clear();  -> null Ȥ�� 0

	//---------------------------------------------------------------------------------
#define DELETE_ROOT \
	delete Root_Node; \
	Root_Node = nullptr; \
	Root_Node = new JNode(JNode::JType::NULL_TYPE); \
	Cur_Node = Root_Node;

#define NO_TYPE_THROW \
	if (Is_Null()) {\
		throw std::invalid_argument("�ȿ� ���� �����ϴ�.");\
					}\
	if (Not_Match_Type(curType)) {\
		throw std::invalid_argument("Ÿ���� ���� �ʽ��ϴ�.");\
					}


#define NO_TYPE_EXCE(rtrn)\
	if(Is_Null()){\
		std::cout << "�ȿ� ���� �����ϴ�." << std::endl;\
		return rtrn;\
		}\
	if (Not_Match_Type(curType)) {\
		std::cout << "Ÿ���� ���� �ʽ��ϴ�." << std::endl;\
		return rtrn;\
	}\

	class JsonData {
	private:
		JNode* node;

	public:
		//�⺻������
		JsonData() : node(nullptr){}
		//������
		JsonData(JNode* n) : node(n){}
		//��ü ������ Ű���� ���� ����� ã�Ƽ� ���� �������
		JsonData(JNode* n, const char* key) : node(n){
			try{
				JNode* Value = nullptr;
				JObj* obj = static_cast<JObj*>(node->ptype);

				for (int i = 0; i<=node->ObjCnt; i++){
					//���ڿ� �� �ؾ���
					if (obj->key.StrCat(key)){
						node = &obj->value;
						return;
					}
					obj = obj->next;
				}
				throw std::invalid_argument("�´� Key���� ���� ���� �ʽ��ϴ�.");
			}
			catch (const std::invalid_argument& e){
				std::cout << "���� �߻�: " << e.what() << std::endl;
				node = nullptr;
			}
		}
		//�迭 ������ index�� ���� �� ã�Ƽ� �����������
		JsonData(JNode* n, int index) : node(n){
			try{
				JNode* Value;
				JArr* arr = static_cast<JArr*>(node->ptype);

				if (node->ArrCnt < index){
					throw std::invalid_argument("�ش� �ε��� ���� �������� �ʽ��ϴ�. �迭�� ũ�⸦ Ȯ�����ּ���");
				}

				for (int i = 0; i<=node->ArrCnt; i++){
					//�ش� index��ġ���� �����ؾ��� ���Ḯ��Ʈ �̰Ŵ� ¿ �� ������
					arr = arr->next;
				}
				node = &arr->value;
			}
			catch (const std::invalid_argument& e){
				std::cout << "���� �߻�: " << e.what() << std::endl;
				node = nullptr;
			}


		}
		
		
		//�б�뵵
        operator JNode&() const {  
            if (node != nullptr)  
                return *node;  
            JNode defaultNode; // Create a static default JNode instance to return as a reference  
            return defaultNode;  
        }
		operator JNode*() const { 
			if (node != nullptr)
				return node; 
			return nullptr;
		}
		//�����뵵
		JsonData& operator=(const JNode& other){
            if (node == nullptr) {
            JsonData defaultData; // Create a static default JsonData instance to return as a reference
            return defaultData;
            }
			
			//���� �����Ѵٸ� Node�� ����⸦ �����ؾ��� �������縦 �����ؾ���
			*node = other;

			
		}
	};


	// ������ JNode�� �����̸� �̰� �������� �����ϵ���

	class JsonCtrl {
	private:
		//��������� �� ������Կ����� ����
		//��������� 
		JsonCtrl(const JsonCtrl& other) : Root_Node(other.Root_Node), Cur_Node(other.Cur_Node), Next_Node(other.Next_Node){}

		//�̵�������
		JsonCtrl(const JsonCtrl&& other) : Root_Node(other.Root_Node), Cur_Node(other.Cur_Node), Next_Node(other.Next_Node){}

		JsonCtrl& operator=(const JsonCtrl& other){}
		JsonCtrl operator=(const JsonCtrl other){}
		JsonCtrl operator=(const JsonCtrl *other){}
	public:
		//�����ڷ� �����ÿ��� ������ Root_Node�� �ʱ�ȣ �Ǿ�� �ϰ� Cur_Node, Next_node�� ����Ű�� Focus�� ���Ҹ� �Ѵ�
		JsonCtrl() : Root_Node(new JNode(JNode::JType::NULL_TYPE)), Cur_Node(Root_Node), Next_Node(nullptr){}
		JsonCtrl(JNode::JType rootType) : Cur_Node(new JNode(JNode::JType::NULL_TYPE)), Next_Node(nullptr){
			Root_Node = new JNode(rootType);
			Cur_Node = Root_Node;
		}
		//�Ҹ���
		~JsonCtrl() {
			if (Root_Node != nullptr) {
				delete Root_Node;
				Root_Node = nullptr;
			}
		}


//<����ó��>

		//JNode�� nullptr�϶�
		bool Is_Null() const {
			return Cur_Node == nullptr;
		}
		bool Overwrite() {
			if (Is_Null()) {
				return false; // root�� nullptr�̸� ��� �� ����
			}
			//DELETE_ROOT
			delete Cur_Node; 
			Cur_Node = nullptr; 
			Cur_Node = new JNode(JNode::JType::NULL_TYPE); 
			Cur_Node = Root_Node;
			return true;
		}
		bool Not_Match_Type(JNode::JType CallType) const {
			if (CallType != Cur_Node->type){
				return true;
			}
			return false;
		}


//<���� ������ �����ε� - Get �κ�>--------------------------------------------------------------------------------------------------------------
//<enum Ÿ�� ����>
		void operator=(JNode::JType rootType) {
			Overwrite();
			Root_Node = new JNode(rootType);
			Cur_Node = Root_Node;
		}

//<��ü Ÿ�� ����>

		//�������� ������ ������ ����Ⱑ ������ �Ǿ�� �ؼ� Root_Node�� �־������
		void operator=(JObj* obj) {
			Overwrite();
			Root_Node->type = JNode::JType::OBJECT;
			Root_Node->ptype = static_cast<void*>(obj);
			Cur_Node = Root_Node;
		}

		//JNode ���ڿ� ��ü �Ľ������ �Ҷ�
		bool Str_Obj_Parse(const char* str) {
			Root_Node->Set_Type(JNode::JType::OBJECT);
			Dynamic::DynamicStr strPtr(512);

			strPtr.Set_Str(str);

			//�Ľ� ���������� ����
			int i = 0;
			int last_focus = strPtr.str_last_focus;

			for (; i < last_focus; i++) {

			}


			return true;
		}

		//class["a"] = ����
		

//<�迭 Ÿ�� ����>

		void operator=(JArr* arr) {
			Overwrite();
			Root_Node->type = JNode::JType::ARRAY;
			Root_Node->ptype = static_cast<void*>(arr);
			Cur_Node = Root_Node;
		}

		//JNode ���ڿ� �迭 �Ľ�������Ҷ�
		bool Str_Arr_Parse(const char* str) {
			Root_Node->Set_Type(JNode::JType::ARRAY);
			Dynamic::DynamicStr* strPtr = new Dynamic::DynamicStr(512);

			strPtr->Set_Str(str);

			//�Ľ� ���������� ����
			std::cout << "Parsing JSON Array: " << str << std::endl;
			std::cout << strPtr->Get_Str() << std::endl;

			return true;
		}

		//class[1] = ����
		

//<���� Ÿ�� ����>

		// 1. ����Ⱑ �����ϵ���
		//String Ÿ���϶�
		void operator=(const char* str) {
			Overwrite();

			Root_Node->Set_Type(JNode::JType::STRING);
			Dynamic::DynamicStr* strPtr = static_cast<Dynamic::DynamicStr*>(Root_Node->Get_Ptype());
			strPtr->Set_Str(str);

			short first_focus = 0;
			short last_focus = strPtr->str_last_focus;

			if (strPtr->Get_Str()[first_focus] == '\"' && strPtr->Get_Str()[last_focus] == '\"') {
				first_focus++;
				last_focus--;
			}

			if (strPtr->Get_Str()[first_focus] == '{' && strPtr->Get_Str()[last_focus] == '}') {
				//��ü ���� �Ǵ�
				DELETE_ROOT
					// JNode �Ľ����ִ� �Լ� ȣ��
					if (Str_Obj_Parse(str)) {
					return;
					}
					else {
						std::cout << "JsonCtrl::Str_Obj_Parse() Error" << std::endl;
					}
			}
			else if (strPtr->Get_Str()[first_focus] == '[' && strPtr->Get_Str()[last_focus] == ']') {
				//�迭 ���� �Ǵ�
				DELETE_ROOT
					if (Str_Arr_Parse(str)) {
					return;
					}
					else {
						std::cout << "JsonCtrl::Str_Arr_Parse() Error" << std::endl;
					}
			}
			Cur_Node = Root_Node;
		}

		//char ���� Ÿ���϶�
		void operator=(const char c){
			Overwrite();
			Root_Node->Set_Type(JNode::JType::CHAR);
			char* charPtr = static_cast<char*>(Root_Node->Get_Ptype());
			*charPtr = c;
			Cur_Node = Root_Node;
		}

		//Number Ÿ���϶�
		void operator=(int number) {
			Overwrite();
			Root_Node->Set_Type(JNode::JType::NUMBER);
			int* numPtr = static_cast<int*>(Root_Node->Get_Ptype());
			*numPtr = number;
			Cur_Node = Root_Node;
		}
		
		void operator=(int* number){
			Overwrite();
			Root_Node->Set_Type(JNode::JType::NUMBER);
			int* numPtr = static_cast<int*>(Root_Node->Get_Ptype());
			*numPtr = *number;
			Cur_Node = Root_Node;
		}

		//Boolean Ÿ���϶�
		void operator=(bool boolean) {
			Overwrite();
			Root_Node->Set_Type(JNode::JType::BOOL);
			bool* boolPtr = static_cast<bool*>(Root_Node->Get_Ptype());
			*boolPtr = boolean;
			Cur_Node = Root_Node;
		}

		void operator=(bool* boolean){
			Overwrite();
			Root_Node->Set_Type(JNode::JType::BOOL);
			bool* boolPtr = static_cast<bool*>(Root_Node->Get_Ptype());
			*boolPtr = *boolean;
			Cur_Node = Root_Node;
		}

		//Double Ÿ���϶�
		void operator=(double number) {
			Overwrite();
			Root_Node->Set_Type(JNode::JType::DOUBLE);
			double* doublePtr = static_cast<double*>(Root_Node->Get_Ptype());
			*doublePtr = number;
			Cur_Node = Root_Node;
		}

		void operator=(double* number){
			Overwrite();
			Root_Node->Set_Type(JNode::JType::DOUBLE);
			double* doublePtr = static_cast<double*>(Root_Node->Get_Ptype());
			*doublePtr = *number;
			Cur_Node = Root_Node;
		}

		//Null Ÿ���϶�
		void operator=(std::nullptr_t) {
			Overwrite();
			Root_Node->Set_Type(JNode::JType::NULL_TYPE);
			Cur_Node = Root_Node;
		}

//JObj, JArr�� �б� ���Ⱑ �����ϵ��� ������ִ� ������ �����ε� ----------------------------------------------------------------------------------

		JsonData operator[](const char* key){
			//Node["Key"] -> Node�� ��ȯ������� return Cur_Node Ȥ�� Root_Node;
		}
		JsonData operator[](int index){
			
		}

		/*JsonData operator[](const char* key){
			try{
				JNode::JType curType = JNode::JType::OBJECT;

				NO_TYPE_THROW

				JsonData pop(Cur_Node, key);

				return pop;
			}
			catch (const std::invalid_argument& e){
				std::cout << "����: " << e.what() << std::endl;
				return JsonData();
			}
		}

		JsonData operator[](int index){
			try{
				JNode::JType curType = JNode::JType::ARRAY;

				NO_TYPE_THROW

				JsonData pop(Cur_Node, index);

				return pop;
			}
			catch (const std::invalid_argument& e){
				std::cout << "����: " << e.what() << std::endl;
				return JsonData();
			}
		}*/
//---------------------------------------------------------------------------------------------------------------------------------------------

//<��ȯ ������ �����ε� - Pop �κ�>--------------------------------------------------------------------------------------------------------------
//<��ü Ÿ�� ��ȯ>

		//JObj�� ��ȯ �� ��ü�� ��ȯ
		operator JObj*() const{
			try{
				JNode::JType curType = JNode::JType::OBJECT;

				NO_TYPE_THROW

				JObj* jobj = static_cast<JObj*>(Cur_Node->ptype);

				return jobj;
			}
			catch (const std::invalid_argument& e){
				std::cout << "����: " << e.what() << std::endl;
				return nullptr;
			}
		}
		operator JObj() const{
			try{
				JNode::JType curType = JNode::JType::OBJECT;

				NO_TYPE_THROW

				JObj* jobj = static_cast<JObj*>(Cur_Node->ptype);

				return *jobj;
			}
			catch (const std::invalid_argument& e){
				std::cout << "����: " << e.what() << std::endl;
				return JObj();	//�⺻������ NULLTYPE ��ȯ
			}
		}



//<�迭 Ÿ�� ��ȯ>
		
		//JNode& operator[](int index) {}
		

		operator JArr*() const{
			try{
				JNode::JType curType = JNode::JType::ARRAY;

				NO_TYPE_THROW

				JArr* jarr = static_cast<JArr*>(Cur_Node->ptype);

				return jarr;
			}
			catch (const std::invalid_argument& e){
				std::cout << "����: " << e.what() << std::endl;
				return nullptr;
			}
		}
		operator JArr() const{
			try{
				JNode::JType curType = JNode::JType::ARRAY;

				NO_TYPE_THROW

					JArr* jarr = static_cast<JArr*>(Cur_Node->ptype);

				return *jarr;
			}
			catch (const std::invalid_argument& e){
				std::cout << "����: " << e.what() << std::endl;
				return JArr();	//�⺻������ NULLTYPE ��ȯ
			}
		}



//<���� Ÿ�� ��ȯ>

		//������ȯ
		operator int() const{
			JNode::JType curType = JNode::JType::NUMBER;

			NO_TYPE_EXCE(-1)

			int *num = static_cast<int*>(Cur_Node->ptype);

			return *num;

		}
		operator int*() const{
			JNode::JType curType = JNode::JType::NUMBER;

			NO_TYPE_EXCE(nullptr)

			int *num = static_cast<int*>(Cur_Node->ptype);

			return num;
		}
		//�Ǽ���ȯ
		operator double() const{
			JNode::JType curType = JNode::JType::DOUBLE;

			NO_TYPE_EXCE(-1.0);

			double* dnum = static_cast<double*>(Cur_Node->ptype);

			return *dnum;
		}
		operator double*() const{
			JNode::JType curType = JNode::JType::DOUBLE;

			NO_TYPE_EXCE(nullptr);

			double* dnum = static_cast<double*>(Cur_Node->ptype);

			return dnum;
		}
		//bool��ȯ
		operator bool() const{
			JNode::JType curType = JNode::JType::BOOL;

			NO_TYPE_EXCE(false);

			bool* bol = static_cast<bool*>(Cur_Node->ptype);

			return *bol;
		}
		operator bool*() const{
			JNode::JType curType = JNode::JType::BOOL;

			NO_TYPE_EXCE(nullptr);

			bool* bol = static_cast<bool*>(Cur_Node->ptype);

			return bol;
			try{
			}
			catch (const std::invalid_argument& e){
				std::cout << "����: " << e.what() << std::endl;
				return nullptr;
			}
		}
		/*char operator=(int index) const{
		���ڸ� ����ϴ°� �ϴ� ���� 
		}*/
		//���ڿ� ��ȯ
		operator char*() const{
			JNode::JType curType = JNode::JType::STRING;

			NO_TYPE_EXCE(nullptr);

			Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(Cur_Node->ptype);

			return str->Get_Str();
		}

		//���ڹ�ȯ
		operator char() const{
			JNode::JType curType = JNode::JType::CHAR;

			NO_TYPE_EXCE('\0');

			char* c = static_cast<char*>(Cur_Node->ptype);

			return c[0];
		}


	private:
		JNode* Root_Node;
		JNode* Cur_Node;
		JNode* Next_Node;
	};
}

