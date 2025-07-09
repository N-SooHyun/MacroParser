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
			DELETE		//삭제된 타입(추가적인 용도로 사용 가능)
		};
		JNode() : type(JType::NULL_TYPE), ptype(nullptr){}
		JNode(JType nodeType) : type(nodeType), ptype(nullptr) {
			Set_Type(nodeType); // 타입 설정
		}

		void Set_Type(JType nodeType);

		~JNode() {
			delete ptype; // 동적 할당된 메모리 해제
			ptype = nullptr; // 포인터 초기화
			type = JType::DELETE; // 타입을 DELETE로 설정
		}

	private:
		JType type; //노드 타입
		void* ptype; //노드 타입에 따른 데이터 포인터

		friend class JObj;
		friend class JArr;
		friend class JsonCtrl;
		friend class Obj_Ctrl;
	};


	class JObj {
	public: 
		JObj() : key(128) , value(JNode::JType::NULL_TYPE), next(nullptr){}
		~JObj() {
			delete next; // 다음 객체를 가리키는 포인터 해제 
		}

		void Set_Key(const char* k);  // 키 값 설정
		void Set_Value(JNode::JType nodeType);// 값 타입 설정

	private:
		Dynamic::DynamicStr key; // 키 값
		JNode value; // 값 (Json_Model 타입으로 정의)
		JObj* next; // 다음 객체를 가리키는 포인터 (연결 리스트 형태로 구현 가능)
		friend class JArr;
		friend class JsonCtrl;
		friend class Obj_Ctrl;
	};


	class JArr {
	public:
		JArr() : next(nullptr), value(JNode::JType::NULL_TYPE){}
		~JArr() {
			delete next; // 다음 배열 요소를 가리키는 포인터 해제
		}

		void Set_Value(JNode::JType nodeType);
		void Set_Next(JArr* nextNode);

	private:
		JArr* next; // 다음 배열 요소를 가리키는 포인터 (연결 리스트 형태로 구현 가능)
		JNode value; // 배열 요소의 값 (Json_Model 타입으로 정의)
	};


	//---------------------------------------------------------------------------------
	//-------------------------------Json사용자 호출부분-------------------------------
	//---------------------------------------------------------------------------------


	using namespace std;
	using namespace Dynamic;
	using namespace JSON;

#define JSON_OBJ JSON::JObj()
#define JSON_ARR JSON::JArr()
#define OPER_ASSIGN(para_type, Jenum_type)\
void operator=(const para_type& value){\
		if(key == nullptr) {\
			cout << "JsonCtrl is Not Obj." << endl;\
			return;\
		}\
		JObj* obj = static_cast<JObj*>(parent.root->ptype);\
		obj->Set_Key(key->Get_Str()); \
		obj->Set_Value(Jenum_type); \
		*static_cast<para_type*>(obj->value.ptype) = value; \
		parent.Obj = parent.Obj->next;\
	}

#define OPER_DEFAULT

	//Json 처리 부분 자료구조 사용 API라고 생각하면 될거 같습니다.
	class JsonCtrl {
	public:
		JsonCtrl() : root(nullptr), Child(nullptr), Obj(nullptr) {}
		JsonCtrl(JNode::JType rootType) : root(new JNode(rootType)), Child(nullptr), Obj(nullptr) {
			if (rootType == JNode::JType::OBJECT)
				Obj = static_cast<JObj*>(root->ptype);
		}
		~JsonCtrl() {
			if (root != nullptr) {
				delete root;
				root = nullptr; // 포인터 초기화
			}
			if (Child != nullptr) {
				delete Child;
				Child = nullptr; // 포인터 초기화
			}
			if (Obj != nullptr) {
				delete Obj;
				Obj = nullptr; // 포인터 초기화
			}
		}

		class Obj_Ctrl {		//무조건 객체 타입일때만 사용되는 Push용 클래스라고 보면 됩니다.
			Dynamic::DynamicStr* key; // 키 값
			JsonCtrl& parent; // 부모 JsonCtrl 객체 참조

		public:
			Obj_Ctrl(JsonCtrl& parentCtrl, const char* k) : parent(parentCtrl) {
				if (k == "" || k == "arr") {
					cout << "This Node Not Obj Type" << endl;
					key = nullptr;
				}

				JObj* Excep_Key = static_cast<JObj*>(parent.root->ptype);

				for (; ;) {
					if (Excep_Key == nullptr) break; //이미 존재하는 키가 없으면 종료

					if (Excep_Key->key.Get_Str() == k) {
						// Key에대한 Value가 이미 존재하는 경우 값 덮어쓰기
						key = &(Excep_Key->key);
						return;
					}
					Excep_Key = Excep_Key->next; // 다음 객체로 이동
				}

				key = new Dynamic::DynamicStr(128); // 키 값 동적 할당
				key->Set_Str(k); // 키 값 설정
			}
			~Obj_Ctrl() {
				if (key != nullptr) {
					delete key; // 동적 할당된 키 값 해제
					key = nullptr; // 포인터 초기화
					parent.Child->key = nullptr;
				}
			}

			//자동 타입으로 하면 개발자가 어떤 타입인지 알기가 어려우니 여러 연산자 오버로딩으로 노가다 해야할듯
			
			//1. Obj 타입
			OPER_ASSIGN(JSON::JObj, JNode::JType::OBJECT)
			//2. Arr 타입
			OPER_ASSIGN(JSON::JArr, JNode::JType::ARRAY)
			//3. Str 타입
			void operator=(const char* value) {
				if (key == nullptr) {
					cout << "JsonCtrl is Not Obj." << endl; return;
				} 
				JObj* obj = static_cast<JObj*>(parent.root->ptype); 
				obj->Set_Key(key->Get_Str()); 
				obj->Set_Value(JNode::JType::STRING);
				Dynamic::DynamicStr* str = static_cast<Dynamic::DynamicStr*>(obj->value.ptype);
				str->Set_Str(value);
				parent.Obj = parent.Obj->next; // 다음 객체로 이동
			}
			//4. Num 타입
			OPER_ASSIGN(int, JNode::JType::NUMBER)
			//5. Bol 타입
			OPER_ASSIGN(bool, JNode::JType::BOOL)
			//6. Double 타입
			OPER_ASSIGN(double, JNode::JType::DOUBLE)
			//7. NULL 타입
			void operator=(const void* value) {
				JObj* Excep_Key = static_cast<JObj*>(parent.root->ptype);

				for (; ;) {
					if (Excep_Key == nullptr) break; //이미 존재하는 키가 없으면 종료

					if (Excep_Key->key.Get_Str() == value) {
						cout << "The Key Already Exists" << endl;
						return; // 이미 존재하는 키인 경우 예외 처리
					}
					Excep_Key = Excep_Key->next; // 다음 객체로 이동
				}

				if (key == nullptr) {
					cout << "JsonCtrl is Not Obj." << endl; return;
				} JObj* obj = static_cast<JObj*>(parent.root->ptype); 
				obj->Set_Key(key->Get_Str()); 
				obj->Set_Value(JNode::JType::NULL_TYPE);
				parent.Obj = parent.Obj->next; // 다음 객체로 이동
			}

			operator int() {
				if (key == nullptr) {
					cout << "JsonCtrl is Not Obj." << endl;
					return 0;
				}
			}

			operator double() {
				printf("Test2");
				return 0.0;
			}
			
		};

		//사용자 API 함수들
		//Push 넣기
		//{} 객체 기준 var["key"] = value;
		Obj_Ctrl& operator[](const char* key) {
			// JsonCtrl 객체가 초기화되지 않은 경우 혹은 객체가 아닌 경우
			if (root == nullptr) {
				//nullptr 인 경우 예외 처리
				static Obj_Ctrl dummy(*this, "");
				return dummy;
			}
			else if (root->type == JNode::JType::ARRAY) {
				//배열 타입인 경우 넘겨주기
				static Obj_Ctrl dummy(*this, "arr");
				return dummy;
			}


			if (root->type == JNode::JType::OBJECT) {
				Child = new Obj_Ctrl(*this, key);
				return *Child; // Obj_Ctrl 객체 반환
			}
			else {
				//잘못된 타입인 경우 예외 처리
				static Obj_Ctrl dummy(*this, "");
				return dummy;
			}


		}

		//Pop 빼기
		

	private:
		JObj* Obj;			// Json 객체
		Obj_Ctrl* Child;	// 리소스 하위 객체
		JNode* root;		// Json의 루트 노드
	};
}

