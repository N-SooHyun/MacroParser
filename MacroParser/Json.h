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
			NULL_TYPE,	//null
			DOUBLE,		//소수
			INVALID,	//잘못된 타입
		};
		JNode() : type(JType::NULL_TYPE), ptype(nullptr){}
		JNode(JType nodeType) : type(nodeType), ptype(nullptr) {
			Set_Type(nodeType); // 타입 설정
		}

		void Set_Type(JType nodeType);
		void* Get_Ptype();
		template<typename T>
		JNode::JType GetJsonType() ;


		~JNode() {
			//delete ptype;
			if(type == JNode::JType::ARRAY || type == JNode::JType::OBJECT) {
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
			type = JNode::JType::INVALID; // 타입을 INVALID로 설정하여 안전하게 초기화
		}

		JNode(const JNode& other) = delete; // 복사 생성자 삭제
		JNode& operator=(const JNode& other) = delete; // 복사 대입 연산자 삭제

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
		JObj() : key(128) , value(JNode::JType::NULL_TYPE), next(nullptr){}
		~JObj() {
			if (next != nullptr) {
				delete next;
				next = nullptr;
			}
		}

		void Set_Key(const char* k);  // 키 값 설정
		void Set_Value(JNode::JType nodeType);// 값 타입 설정
		JNode* Get_Value();

	//private:
		Dynamic::DynamicStr key; // 키 값
		JNode value; // 값 (Json_Model 타입으로 정의)
		JObj* next; // 다음 객체를 가리키는 포인터 (연결 리스트 형태로 구현 가능)
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

		void Set_Value(JNode::JType nodeType);
		void Set_Next(JArr* nextNode);

	private:
		JArr* next; // 다음 배열 요소를 가리키는 포인터 (연결 리스트 형태로 구현 가능)
		JNode value; // 배열 요소의 값 (Json_Model 타입으로 정의)

		friend class JsonAPI;
	};


	//---------------------------------------------------------------------------------
	//-------------------------------Json사용자 호출부분-------------------------------
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

	// 뭐든지 JNode가 시작이며 이걸 기준으로 진행하도록

	class JsonCtrl {
	public:
		JsonCtrl() : root(nullptr) {}
		JsonCtrl(JNode::JType rootType) {
			root = new JNode(rootType);
		}

		~JsonCtrl() {
			if (root != nullptr) {
				delete root;
				root = nullptr;
			}
		}


		//<대입 연산자 오버로딩>
		//enum으로 받을때
		void operator=(JNode::JType rootType) {
			root = new JNode(rootType);
		}

		//객체 배열 타입일때(JNode::JType::OBJECT, JNode::JType::ARRAY)
		//Object 타입일때
		/*void operator=(JObj& obj) {
			root->type = JNode::JType::OBJECT;
			root->ptype = static_cast<void*>(obj);
		}*/
		//동적변수 받을때
		void operator=(JObj* obj) {
			root->type = JNode::JType::OBJECT;
			root->ptype = static_cast<void*>(obj);
		}

		//Array 타입일때
		void operator=(JArr* arr) {
			root->type = JNode::JType::ARRAY;
			root->ptype = static_cast<void*>(arr);
		}

		//단일 타입일때(int, double, string, bool 등)
		// 1. 덮어쓰기가 가능하도록
		//String 타입일때
		void operator=(const char* str) {
			root->Set_Type(JNode::JType::STRING);
			Dynamic::DynamicStr* strPtr = static_cast<Dynamic::DynamicStr*>(root->Get_Ptype());
			strPtr->Set_Str(str);
		}

		//Number 타입일때
		void operator=(int number) {
			root->Set_Type(JNode::JType::NUMBER);
			int* numPtr = static_cast<int*>(root->Get_Ptype());
			*numPtr = number;
		}

		//Boolean 타입일때
		void operator=(bool boolean) {
			root->Set_Type(JNode::JType::BOOL);
			bool* boolPtr = static_cast<bool*>(root->Get_Ptype());
			*boolPtr = boolean;
		}

		//Double 타입일때
		void operator=(double number) {
			root->Set_Type(JNode::JType::DOUBLE);
			double* doublePtr = static_cast<double*>(root->Get_Ptype());
			*doublePtr = number;
		}

		//Null 타입일때
		void operator=(std::nullptr_t) {
			root->Set_Type(JNode::JType::NULL_TYPE);
		}



	private:
		JNode* root;
	};

}

