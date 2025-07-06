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
			FLOAT,		//소수
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
		friend class Proxy;
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


	//Json 처리 부분 자료구조 사용 API라고 생각하면 될거 같습니다.
	class JsonCtrl {
	public:
		JsonCtrl() : root(nullptr), Child(nullptr) {}
		JsonCtrl(JNode::JType rootType) : root(new JNode(rootType)), Child(nullptr) {}
		~JsonCtrl() {
			if (root != nullptr) {
				delete root;
				root = nullptr; // 포인터 초기화
			}
			if (Child != nullptr) {
				delete Child;
				Child = nullptr; // 포인터 초기화
			}
		}

		class Proxy {
			Dynamic::DynamicStr* key; // 키 값
			JsonCtrl& parent; // 부모 JsonCtrl 객체 참조

		public:
			Proxy(JsonCtrl& parentCtrl, const char* k) : key(new Dynamic::DynamicStr(128)), parent(parentCtrl) {
				key->Set_Str(k); // 키 값 설정
			}
			~Proxy() {
				if (key != nullptr) {
					delete key; // 동적 할당된 키 값 해제
					key = nullptr; // 포인터 초기화
					parent.Child->key = nullptr;
				}
			}

#define JSON_OBJ JSON::JObj{}
#define JSON_ARR JSON::JArr{}
			//자동 타입으로 하면 개발자가 어떤 타입인지 알기가 어려우니 여러 연산자 오버로딩으로 노가다 해야할듯
			template<typename T>
			void operator=(const T& value) {
				JObj* obj = static_cast<JObj*>(parent.root->ptype);

				obj->Set_Key(key->Get_Str()); // 키 값 설정




			}

		};

		//사용자 API 함수들
		//Push 넣기
		//{} 객체 기준 var["key"] = value;
		Proxy operator[](const char* key) {
			Child = new Proxy(*this, key);
			return *Child; // Proxy 객체 반환
		}

		//Pop 빼기


	private:

		Proxy* Child;	// 리소스 하위 객체
		JNode* root;	// Json의 루트 노드
	};
}

