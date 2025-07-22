#pragma once

namespace JSON {
	//.Json 파일이 가능한 조건 2개
	// 구조적타입 - {}, []
	// 원시적타입 - "str", true, 1, 123.2, null
	//1. Key값 무조건 Stirng 타입
	//2. Value : Number (int, float), String, Boolean (true, false), Array ([]), Object ({}), null

	//---------------------------------------------------------------------------------
	//-------------------------------Json자료구조 부분---------------------------------
	//---------------------------------------------------------------------------------

	class JNodeBase {
	public:
		JNodeBase(){}
		virtual ~JNodeBase() {}
	};

	class JNode {
	public:
		enum class JType {
			//구조적 타입
			OBJECT,		//{}
			ARRAY,		//[]
			//원시적 타입
			STRING,		//문자열
			NUMBER,		//숫자
			BOOL,		//true false
			DOUBLE,		//소수
			NULL_TYPE,	//null
		};
		JNode() : type(JType::NULL_TYPE), ptype(nullptr){}
		JNode(JType nodeType) : type(nodeType), ptype(nullptr) {
			Set_Type(nodeType); // 타입 설정
		}

		void Set_Type(JType nodeType);
		void* Get_Ptype();
		template<typename T>
		JNode::JType GetJsonType();

		//복사생성자
		JNode(const JNode& other) : type(other.type), ptype(nullptr){
			Set_Type(other.type); //ptype 설정
		}

		~JNode() {
			//delete ptype;
			if (type == JNode::JType::ARRAY || type == JNode::JType::OBJECT) {
				// JObj나 JArr 타입의 경우, ptype은 해당 타입의 객체를 가리키므로 동적 할당 해제
				delete static_cast<JNodeBase*>(ptype);
			}
			else if (type == JNode::JType::STRING) {
				// DynamicStr 타입의 경우, ptype은 문자열을 가리키므로 동적 할당 해제
				delete static_cast<Dynamic::DynamicStr*>(ptype);
			}
			else if (type == JNode::JType::NUMBER || type == JNode::JType::BOOL || type == JNode::JType::DOUBLE) {
				delete ptype;
			}
			ptype = nullptr;
			type = JNode::JType::NULL_TYPE; // 타입을 INVALID로 설정하여 안전하게 초기화
		}

		//private:
		JType type; //노드 타입
		void* ptype; //노드 타입에 따른 데이터 포인터

		friend class JObj;
		friend class JArr;
		friend class JsonCtrl;
		friend class JsonAPI;
	};


	class JObj : public JNodeBase {
	public:
		JObj() : key(128), value(JNode::JType::NULL_TYPE), next(nullptr), ObjCnt(-1){}
		~JObj() {
			if (next != nullptr) {
				delete next;
				next = nullptr;
			}
		}

		//복사생성자 깊은복사
		JObj(const JObj& other) : key(other.key), value(other.value), next(other.next), ObjCnt(other.ObjCnt){			
		}

		//이동생성자
		JObj(JObj&& other) : key(other.key), value(other.value), next(other.next), ObjCnt(other.ObjCnt){
		}

		void Set_Key(const char* k);  // 키 값 설정
		void Set_Value(JNode::JType nodeType);// 값 타입 설정
		JNode* Get_Value();

	//private:
		Dynamic::DynamicStr key; // 키 값
		int ObjCnt;
		JNode value; // 값 (Json_Model 타입으로 정의)
		JObj* next; // 다음 객체를 가리키는 포인터 (연결 리스트 형태로 구현 가능)
		friend class JArr;
		friend class JsonAPI;
	};


	class JArr : public JNodeBase{
	public:
		JArr() : next(nullptr), value(JNode::JType::NULL_TYPE), ArrCnt(-1){}
		~JArr() {
			if (next != nullptr) {
				delete next;
				next = nullptr;
			}
		}

		//복사생성자 깊은복사
		JArr(const JArr& other) : value(other.value), next(other.next), ArrCnt(other.ArrCnt){}

		//이동생성자
		JArr(const JArr&& other) : value(other.value), next(other.next), ArrCnt(other.ArrCnt){}

		void Set_Value(JNode::JType nodeType);
		void Set_Next(JArr* nextNode);

	//private:
		JArr* next; // 다음 배열 요소를 가리키는 포인터 (연결 리스트 형태로 구현 가능)
		JNode value; // 배열 요소의 값 (Json_Model 타입으로 정의)
		int ArrCnt;

		friend class JsonAPI;
	};


	//---------------------------------------------------------------------------------
	//-------------------------------Json사용자 호출부분--------------------------------
	//---------------------------------------------------------------------------------

	//JSON 사용 규칙
	// {} 객체일때
	// JNode Js = "{"Key" : "Value"}"; 
	// 1. Insert 추가하기
	// Js.a = "b";  -> {"Key" : "Value", "a" : "b"}
	// Js["a"] = "b";  -> {"Key" : "Value", "a" : "b"}
	// char* var = "a";    Js[var] = "b";  -> {"Key" : "Value", "a" : "b"}

	// 2. Get 값 가져오기 및 찾기?
	// Js.get("a");  -> "b"
	// Js["a"];  -> "b"
	// char* var = "a";    Js[var];  -> "b"

	// 3. Delete 삭제하기
	// Js.del("a");  -> {"Key" : "Value"}
	// Js["a"].del();  -> {"Key" : "Value"}
	// char* var = "a";    Js[var].del();  -> {"Key" : "Value"}

	// 4. Clear 비우기
	// Js.clear();  -> {}


	// [] Arr일때
	// 1. Insert 추가하기
	// Js[0] = "a";  -> ["a"]
	// Js[1] = "b";  -> ["a", "b"]
	// int idx = 2;    Js[idx] = "c";  -> ["a", "b", "c"]

	// 2. Get 값 가져오기 및 찾기?
	// Js[0];  -> "a"
	// Js[1];  -> "b"
	// int idx = 2;    Js[idx];  -> "c"

	// 3. Delete 삭제하기
	// Js[0].del();  -> ["b", "c"]

	// 4. Clear 비우기
	// Js.clear();  -> []


	// 단일 타입일때
	// 1. Insert 추가하기 혹은 덮어쓰기겠죠?
	// Js = "Hello";  -> "Hello"
	// Js = 123;  -> 123

	// 2. Get 값 가져오기
	// Js.get();  -> "Hello" 혹은 123
	// Js.get<int>();  -> 123

	// 3. Delete 삭제하기
	// Js.del();  -> null 혹은 0

	// 4. Clear 비우기
	// Js.clear();  -> null 혹은 0

	//---------------------------------------------------------------------------------

	class JsonData {
	private:
		JNode* node;

	public:
		//기본생성자
		JsonData() : node(nullptr){}
		//생성자
		JsonData(JNode* n) : node(n){}
		//객체 생성자 키값을 통해 밸류값 찾아서 전달 해줘야함
		JsonData(JNode* n, const char* key) : node(n){
			try{
				JNode* Value;
				JObj* obj = static_cast<JObj*>(node->ptype);

				for (int i = 0; i<=obj->ObjCnt; i++){
					//문자열 비교 해야함
					if (obj->key.StrCat(key)){
						node = &obj->value;
						return;
					}
					obj = obj->next;
				}
				throw std::invalid_argument("맞는 Key값이 존재 하지 않습니다.");
			}
			catch (const std::invalid_argument& e){
				std::cout << "예외 발생: " << e.what() << std::endl;
				node = nullptr;
			}
		}
		//배열 생성자 index를 통해 값 찾아서 전달해줘야함
		JsonData(JNode* n, int index) : node(n){

			try{
				JNode* Value;
				JArr* arr = static_cast<JArr*>(node->ptype);

				if (arr->ArrCnt < index){
					throw std::invalid_argument("해당 인덱스 값은 존재하지 않습니다. 배열의 크기를 확인해주세요");
				}

				for (int i = 0; i<=arr->ArrCnt; i++){
					//해당 index위치까지 도달해야함 연결리스트 이거는 쩔 수 없을듯
					arr = arr->next;
				}
				node = &arr->value;
			}
			catch (const std::invalid_argument& e){
				std::cout << "예외 발생: " << e.what() << std::endl;
				node = nullptr;
			}


		}
		operator JNode&() const { 
			if (node != nullptr)
				return *node; 
			return JNode();
		}
		operator JNode*() const { 
			if (node != nullptr)
				return node; 
			return nullptr;
		}
		
	};


	// 뭐든지 JNode가 시작이며 이걸 기준으로 진행하도록

	class JsonCtrl {
	public:
		//생성자로 생성시에는 무조건 Root_Node가 초기호 되어야 하고 Cur_Node, Next_node는 가리키는 Focus의 역할만 한다
		JsonCtrl() : Root_Node(new JNode(JNode::JType::NULL_TYPE)), Cur_Node(Root_Node), Next_Node(nullptr){}
		JsonCtrl(JNode::JType rootType) : Cur_Node(new JNode(JNode::JType::NULL_TYPE)), Next_Node(nullptr){
			Root_Node = new JNode(rootType);
			Cur_Node = Root_Node;
		}

		~JsonCtrl() {
			if (Root_Node != nullptr) {
				delete Root_Node;
				Root_Node = nullptr;
			}
		}

#define DELETE_ROOT \
	delete Root_Node; \
	Root_Node = nullptr; \
	Root_Node = new JNode(JNode::JType::NULL_TYPE); \
	Cur_Node = Root_Node;

#define NO_TYPE_THROW \
	if (Is_Null()) {\
		throw std::invalid_argument("안에 값이 없습니다.");\
	}\
	if (Not_Match_Type(curType)) {\
		throw std::invalid_argument("타입이 맞지 않습니다.");\
	}

		//예외처리
		//JNode가 nullptr일때
		bool Is_Null() const {
			return Cur_Node == nullptr;
		}
		bool Overwrite() {
			if (Is_Null()) {
				return false; // root가 nullptr이면 덮어쓸 수 없음
			}
			DELETE_ROOT
				return true;
		}
		bool Not_Match_Type(JNode::JType CallType) const {
			if (CallType != Cur_Node->type){
				return true;
			}
			return false;
		}


//<대입 연산자 오버로딩 - Get 부분>--------------------------------------------------------------------------------------------------------------
		//enum으로 받을때
		void operator=(JNode::JType rootType) {
			Overwrite();
			Root_Node = new JNode(rootType);
			Cur_Node = Root_Node;
		}

		//객체 배열 타입일때(JNode::JType::OBJECT, JNode::JType::ARRAY)
		//Object 타입일때
		/*void operator=(JObj& obj) {
		root->type = JNode::JType::OBJECT;
		root->ptype = static_cast<void*>(obj);
		}*/
		//동적변수 받을때 무조건 덮어쓰기가 진행이 되어야 해서 Root_Node에 넣어줘야함
		void operator=(JObj* obj) {
			Overwrite();
			Root_Node->type = JNode::JType::OBJECT;
			Root_Node->ptype = static_cast<void*>(obj);
			Cur_Node = Root_Node;
		}

		//JNode 문자열 객체 파싱해줘야 할때
		bool Str_Obj_Parse(const char* str) {
			Root_Node->Set_Type(JNode::JType::OBJECT);
			Dynamic::DynamicStr strPtr(512);

			strPtr.Set_Str(str);

			//파싱 본격적으로 시작
			int i = 0;
			int last_focus = strPtr.str_last_focus;

			for (; i < last_focus; i++) {

			}


			return true;
		}

		//class["a"] = 대입
		

		//Array 타입일때
		void operator=(JArr* arr) {
			Overwrite();
			Root_Node->type = JNode::JType::ARRAY;
			Root_Node->ptype = static_cast<void*>(arr);
			Cur_Node = Root_Node;
		}

		//JNode 문자열 배열 파싱해줘야할때
		bool Str_Arr_Parse(const char* str) {
			Root_Node->Set_Type(JNode::JType::ARRAY);
			Dynamic::DynamicStr* strPtr = new Dynamic::DynamicStr(512);

			strPtr->Set_Str(str);

			//파싱 본격적으로 시작
			std::cout << "Parsing JSON Array: " << str << std::endl;
			std::cout << strPtr->Get_Str() << std::endl;

			return true;
		}

		//class[1] = 대입
		

		//단일 타입일때(int, double, string, bool 등)
		// 1. 덮어쓰기가 가능하도록
		//String 타입일때
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
				//객체 임을 판단
				DELETE_ROOT
					// JNode 파싱해주는 함수 호출
					if (Str_Obj_Parse(str)) {
					return;
					}
					else {
						std::cout << "JsonCtrl::Str_Obj_Parse() Error" << std::endl;
					}
			}
			else if (strPtr->Get_Str()[first_focus] == '[' && strPtr->Get_Str()[last_focus] == ']') {
				//배열 임을 판단
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

		//Number 타입일때
		void operator=(int number) {
			Overwrite();
			Root_Node->Set_Type(JNode::JType::NUMBER);
			int* numPtr = static_cast<int*>(Root_Node->Get_Ptype());
			*numPtr = number;
			Cur_Node = Root_Node;
		}

		//Boolean 타입일때
		void operator=(bool boolean) {
			Overwrite();
			Root_Node->Set_Type(JNode::JType::BOOL);
			bool* boolPtr = static_cast<bool*>(Root_Node->Get_Ptype());
			*boolPtr = boolean;
			Cur_Node = Root_Node;
		}

		//Double 타입일때
		void operator=(double number) {
			Overwrite();
			Root_Node->Set_Type(JNode::JType::DOUBLE);
			double* doublePtr = static_cast<double*>(Root_Node->Get_Ptype());
			*doublePtr = number;
			Cur_Node = Root_Node;
		}

		//Null 타입일때
		void operator=(std::nullptr_t) {
			Overwrite();
			Root_Node->Set_Type(JNode::JType::NULL_TYPE);
			Cur_Node = Root_Node;
		}


//JObj, JArr를 읽기 쓰기가 가능하도록 만들어주는 연산자 오버로딩 ----------------------------------------------------------------------------------

		JsonData operator[](const char* key){
			try{
				JNode::JType curType = JNode::JType::OBJECT;

				NO_TYPE_THROW

					JsonData pop(Cur_Node, key);

				return pop;
			}
			catch (const std::invalid_argument& e){
				std::cout << "에러: " << e.what() << std::endl;
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
				std::cout << "에러: " << e.what() << std::endl;
				return JsonData();
			}
		}
//---------------------------------------------------------------------------------------------------------------------------------------------

//<반환 연산자 오버로딩 - Pop 부분>--------------------------------------------------------------------------------------------------------------
		//객체 타입 반환
		//JObj를 반환 그 자체를 반환
		operator JObj*() const{
			try{
				JNode::JType curType = JNode::JType::OBJECT;

				NO_TYPE_THROW

				JObj* jobj = static_cast<JObj*>(Cur_Node->ptype);

				return jobj;
			}
			catch (const std::invalid_argument& e){
				std::cout << "에러: " << e.what() << std::endl;
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
				std::cout << "에러: " << e.what() << std::endl;
				return JObj();	//기본생성자 NULLTYPE 반환
			}
		}

		//배열 타입 반환 
		//JNode& operator[](int index) {}
		

		operator JArr*() const{
			try{
				JNode::JType curType = JNode::JType::ARRAY;

				NO_TYPE_THROW

				JArr* jarr = static_cast<JArr*>(Cur_Node->ptype);

				return jarr;
			}
			catch (const std::invalid_argument& e){
				std::cout << "에러: " << e.what() << std::endl;
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
				std::cout << "에러: " << e.what() << std::endl;
				return JArr();	//기본생성자 NULLTYPE 반환
			}
		}


		//단일 타입 반환
		//정수반환
		operator int() const{
			try{
				JNode::JType curType = JNode::JType::NUMBER;

				NO_TYPE_THROW

				int *num = static_cast<int*>(Cur_Node->ptype);

				return *num;

			}catch(const std::invalid_argument& e){
				std::cout << "에러: " << e.what() << std::endl;
				return -1;
			}
		}
		//실수반환
		operator double() const{
			try{
				JNode::JType curType = JNode::JType::DOUBLE;

				NO_TYPE_THROW

				double* dnum = static_cast<double*>(Cur_Node->ptype);

				return *dnum;
			}
			catch (const std::invalid_argument& e){
				std::cout << "에러: " << e.what() << std::endl;
				return -1.0;
			}
		}
		//bool반환
		operator bool() const{
			try{
				JNode::JType curType = JNode::JType::BOOL;

				NO_TYPE_THROW

				bool* bol = static_cast<bool*>(Cur_Node->ptype);

				return *bol;
			}
			catch (const std::invalid_argument& e){
				std::cout << "에러: " << e.what() << std::endl;
				return false;
			}
		}

		/*char operator=(int index) const{
		문자만 출력하는건 일단 보류 
		}*/
		//문자열 반환
		operator char*() const{
			try{
				JNode::JType curType = JNode::JType::STRING;

				NO_TYPE_THROW

				Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(Cur_Node->ptype);

				return str->Get_Str();
			}
			catch (const std::invalid_argument& e){
				std::cout << "에러: " << e.what() << std::endl;
				return '\0';
			}
		}


	private:
		JNode* Root_Node;
		JNode* Cur_Node;
		JNode* Next_Node;
	};
}

