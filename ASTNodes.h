#ifndef ASTNODES_H
#define ASTNODES_H

//#define PRINT_JOSONGEN
//#define PRINT_NUM_OF_VALID_NODES

#include <llvm/IR/Value.h>
#include <json/json.h>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <stdint.h>

class CodeGenContext;
class NBlock;
class NStatement;
class NExpression;
class NVariableDeclaration;

using std::endl;
using std::string;
using std::cout;
using std::shared_ptr;
using std::make_shared;
using std::vector;

typedef vector<shared_ptr<NStatement>> StatementList;
typedef vector<shared_ptr<NExpression>> ExpressionList;
typedef vector<shared_ptr<NVariableDeclaration>> VariableList;


static uint64_t nodeNum = 0;
static uint64_t intNum = 0;
static uint64_t doubleNum = 0;
static uint64_t methodNum = 0;
static uint64_t exprNum = 0;
static uint64_t blockNum = 0;


class Node {
protected:
	const char m_COLON = ':';
	const char* m_PREFIX = "----";
public:
	Node() {
		nodeNum++;
#ifdef PRINT_JOSONGEN
		cout << nodeNum << endl;
#endif
	}
	virtual ~Node() {}
	virtual string getTypeName() const = 0; 
	virtual llvm::Value *codeGen(CodeGenContext &context) { return (llvm::Value *)0; }

#ifdef PRINT_JOSONGEN
	virtual void print(string prefix) const {}
	virtual Json::Value jsonGen() const { return Json::Value(); }
#endif

};


class NStatement : public Node {
public:
	NStatement(){}

	string getTypeName() const override {
		return "NStatement";
	}
#ifdef PRINT_JOSONGEN
	virtual void print(string prefix) const override {
		cout << prefix << getTypeName() << endl;
	}
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		return root;
	}
#endif
};


class NExpression : public Node {
public:
	NExpression() {}

	string getTypeName() const override {
		return "NExpression";
	}
#ifdef PRINT_JOSONGEN
	virtual void print(string prefix) const override {
		cout << prefix << getTypeName() << endl;
	}
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		return root;
	}
#endif
};


class NDouble : public NExpression {
public:
	double value;
	uint64_t index;

	NDouble() { doubleNum++; }

	NDouble(double value)
		:value(value){
		doubleNum++; 
	}

	string getTypeName() const override {
		return "NDouble";
	}

#ifdef PRINT_JOSONGEN
	void print(string prefix) const override {
		cout << prefix << getTypeName() << this->m_COLON << value << endl;
	}

	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName() + this->m_COLON + std::to_string(value);
		return root;
	}
#endif

	virtual llvm::Value* codeGen(CodeGenContext&) override;
};

class NInterger : public NExpression {
public:
	uint64_t value;
	uint64_t index;

	NInterger() { intNum++; }

	NInterger(uint64_t value)
		:value(value) {
		intNum++;
	}

	string getTypeName() const override {
		return "NInteger";
	}

#ifdef PRINT_JOSONGEN
	void print(string prefix) const override {
		cout << prefix << getTypeName() << this->m_COLON << value << endl;
	}

	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName() + this->m_COLON + std::to_string(value);
		return root;
	}
#endif

	operator NDouble() {
		return NDouble(value);
	}

	virtual llvm::Value* codeGen(CodeGenContext&) override;
};

class NIdentifier : public NExpression {
public:
	string name;
	bool isType = false;
	bool isArray = false;
	shared_ptr<ExpressionList> arraySize = make_shared<ExpressionList>();

	NIdentifier() { exprNum++; }

	NIdentifier(const string &name)
		: name(name) {
		exprNum++;
	}
	
	string getTypeName() const override {
		return "NIdentifier";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName() + this->m_COLON + name + (isArray ? "(Array)" : "");
		for (auto child = arraySize->begin(); child != arraySize->end(); child++) {
			root["children"].append((*child)->jsonGen());
		}
		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << name << (isArray ? "(Array)" : "") << endl;
		if (isArray && arraySize() > 0) {
			for (auto child = arraySize->begin(); child != arraySize->end(); child++) {
				(*child)->print(nPrefix);
			}
		}
	}
#endif
	virtual llvm::Value* codeGen(CodeGenContext&) override;
};

class NMethodCall : public NExpression {
public:
	const shared_ptr<NIdentifier> id;
	shared_ptr<ExpressionList> arguments = make_shared<ExpressionList>();

	NMethodCall() {
		methodNum++;
	}

	NMethodCall(const shared_ptr<NIdentifier> id, shared_ptr<ExpressionList> arguments)
		:id(id), arguments(arguments) {
		methodNum++;
	}

	NMethodCall(const shared_ptr<NIdentifier> id)
		:id(id) {
		methodNum++;
	}

	string getTypeName() const override {
		return "NMethodCall";
	}

#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		root["children"].append(this->id->jsonGen());
		for (auto argument = arguments->begin(); argument != arguments->end(); argument++) {
			root["children"].append((*argument)->jsonGen());
		}
		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;
		this->id->print(nPrefix);
		for (auto argument = arguments->begin(); argument != arguments->end(); argument++) {
			(*argument)->print(nPrefix);
		}
	}
#endif

	virtual llvm::Value* codeGen(CodeGenContext&) override;
};

class NBinaryOperator : public NExpression {
public:
	int op;
	shared_ptr<NExpression> lchild;
	shared_ptr<NExpression> rchild;

	NBinaryOperator() {}

	NBinaryOperator(shared_ptr<NExpression> lchild, int op, shared_ptr<NExpression> rchild)
		:lchild(lchild), op(op), rchild(rchild) {

	}

	string getTypeName() const override {
		return "NBinaryOperator";
	}

#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName() + this->m_COLON + std::to_string(op);
		root["children"].append(lchild->jsonGen());
		root["children"].append(rchild->jsonGen());

		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << op << endl;
		
		lchild->print(nPrefix);
		rchild->print(nPrefix);

	}
#endif
	virtual llvm::Value* codeGen(CodeGenContext&) override;
};


class NAssignment : public NExpression {
public:
	shared_ptr<NExpression> lchild;
	shared_ptr<NExpression> rchild;

	NAssignment() {}

	NAssignment(shared_ptr<NExpression> lchild, shared_ptr<NExpression> rchild)
		:lchild(lchild), rchild(rchild) {
	}

	string getTypeName() const override {
		return "NAssignment";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		root["children"].append(lchild->jsonGen());
		root["children"].append(rchild->jsonGen());

		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;
		lchild->print(nPrefix);
		rchild->print(nPrefix);
	}
#endif
	virtual llvm::Value* codeGen(CodeGenContext&) override;
};

class NBlock : public NExpression {
public:
	shared_ptr<StatementList> statements = make_shared<StatementList>();

	NBlock() {
		blockNum++;
	}

	string getTypeName() const override {
		return "NBlock";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		for (auto state = statements->begin(); state != statements->end(); state++) {
			root["children"].append((*state)->jsonGen());
		}
		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;
		for (auto state = statements->begin(); state != statements->end(); state++) {
			(*state)->print(nPrefix);
		}
	}
#endif
	virtual llvm::Value* codeGen(CodeGenContext&) override;
};

class NExpressionStatement : public NStatement {
public:
	shared_ptr<NExpression> expr;

	NExpressionStatement() {}

	NExpressionStatement(shared_ptr<NExpression> expression)
		:expr(expression) {
	}

	string getTypeName() const override {
		return "NExpressionStatement";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		root["children"].append(expr->jsonGen());
		return root;
	}
	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;
		expr->print(nPrefix);
	}
#endif

	virtual llvm::Value* codeGen(CodeGenContext&) override;
};

class NVariableDeclaration : public NStatement {
public:
	const shared_ptr<NIdentifier> type;
	shared_ptr<NIdentifier> id;
	shared_ptr<NExpression> expr = nullptr;
	int32_t index;

	NVariableDeclaration() {}

	NVariableDeclaration(const shared_ptr<NIdentifier> type, shared_ptr<NIdentifier> id, shared_ptr<NExpression> expr = nullptr)
		:type(type), id(id), expr(expr) {
		// check type.
		assert(type->isType);
		assert(!type->isArray || (type->isArray && type->arraySize != nullptr));
	}
	string getTypeName() const override {
		return "NVariableDeclaration";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		root["children"].append(type->jsonGen());
		root["children"].append(id->jsonGen());
		if (expr != nullptr) {
			root["children"].append(expr->jsonGen());
		}
		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;
		type->print(nPrefix);
		id->print(nPrefix);
		if (expr != nullptr) {
			expr->print(nPrefix);
		}
	}
#endif
	virtual llvm::Value* codeGen(CodeGenContext&) override;
};

class NFunctionDeclaration : public NStatement {
public:
	shared_ptr<NIdentifier> type;
	shared_ptr<NIdentifier> id;
	shared_ptr<VariableList> arguments = make_shared< VariableList>();
	shared_ptr<NBlock> block;
	bool external = false;

	NFunctionDeclaration() {}

	NFunctionDeclaration(shared_ptr<NIdentifier> type, shared_ptr<NIdentifier> id, shared_ptr<VariableList> arguments, shared_ptr<NBlock> block, bool external = false)
		:type(type), id(id), arguments(arguments), block(block), external(external) {
		assert(type->isType);
	}

	string getTypeName() const override {
		return "NFunctionDeclaration";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		root["children"].append(type->jsonGen());
		root["children"].append(id->jsonGen());

		for (auto argument = arguments->begin(); argument != arguments->end(); argument++) {
			root["children"].append((*argument)->jsonGen());
		}

		assert(external || block != nullptr);
		if (block) {
			root["children"].append(block->jsonGen());
		}
		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;

		type->print(nPrefix);
		id->print(nPrefix);

		for (auto argument = arguments->begin(); argument != arguments->end(); argument++) {
			(*argument)->print(nPrefix);
		}
		assert(external || block != nullptr);
		if (block) {
			block->print(nPrefix);
		}

	}

#endif
	virtual llvm::Value* codeGen(CodeGenContext&) override;
};

class NStructDeclaration : public NStatement {
public:
	shared_ptr<NIdentifier> id;
	shared_ptr<VariableList> members = make_shared<VariableList>();

	NStructDeclaration() {}

	NStructDeclaration(shared_ptr<NIdentifier> id, shared_ptr<VariableList> members)
		:id(id), members(members) {
	}

	string getTypeName() const override {
		return "NStructDeclaration";
	}

#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName() + this->m_COLON + id->name;
		
		for (auto member = members->begin(); member != members->end(); member++) {
			root["children"].append((*member)->jsonGen());
		}

		return root;
	}
	void print(string prefix)const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << id->name << endl;

		for (auto member = members->begin(); member != members->end(); member++) {
			(*member)->print(nPrefix);
		}

	}

#endif
	virtual llvm::Value* codeGen(CodeGenContext& context) override;
};

class NReturnStatement : public NStatement {
public:
	shared_ptr<NExpression> expr;

	NReturnStatement(){}

	NReturnStatement(shared_ptr<NExpression> expr)
		:expr(expr) {
	}

	string getTypeName() const override {
		return "NReturnStatement";
	}

#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		root["children"].append(expr->jsonGen());
		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;
		expr->print(nPrefix);
	}

#endif
	virtual llvm::Value* codeGen(CodeGenContext& context)override;

};

class NIfStatement : public NStatement {
public:

	shared_ptr<NExpression> condition;
	shared_ptr<NBlock> tBlock;
	shared_ptr<NBlock> fBlock;

	NIfStatement(){}

	NIfStatement(shared_ptr<NExpression> condition, shared_ptr<NBlock> tBlock, shared_ptr<NBlock> fBlock = nullptr)
		:condition(condition), tBlock(tBlock), fBlock(fBlock) {
	}

	string getTypeName() const override {
		return "NIfStatement";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();
		root["children"].append(condition->jsonGen());
		root["children"].append(tBlock->jsonGen());
		if (fBlock) {
			root["children"].append(fBlock->jsonGen());
		}
		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;

		condition->print(nPrefix);
		tBlock->print(nPrefix);

		if (fBlock) {
			fBlock->print(nPrefix);
		}
	}

#endif

	llvm::Value *codeGen(CodeGenContext&) override;

};


class NForStatement : public NStatement {
public:
	shared_ptr<NExpression> initial, condition, increase;
	shared_ptr<NBlock> block;

	NForStatement(){}

	NForStatement(shared_ptr<NBlock> block, shared_ptr<NExpression> initial = nullptr, shared_ptr<NExpression> condition = nullptr, shared_ptr<NExpression> increase = nullptr)
		:block(block), initial(initial), condition(condition), increase(increase) {
		if (condition == nullptr) {
			condition = make_shared<NInterger>(1);
		}
	}

	string getTypeName() const override {
		return "NForStatement";
	}

#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();

		if (initial)
			root["children"].append(initial->jsonGen());
		if (condition)
			root["children"].append(condition->jsonGen());
		if (increase)
			root["children"].append(increase->jsonGen());
		
		return root;
	}

	void print(string prefix) const override {
		
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;

		if (initial)
			initial->print(nPrefix);
		if (condition)
			condition->print(nPrefix);
		if (increase)
			increase->print(nPrefix);

		block->print(nPrefix);


	}

#endif

	llvm::Value *codeGen(CodeGenContext&) override;

};

class NStructMember : public NExpression {
public:
	shared_ptr<NIdentifier> id;
	shared_ptr<NIdentifier> member;

	NStructMember(){}

	NStructMember(shared_ptr<NIdentifier> structId, shared_ptr<NIdentifier> member)
		:id(structId), member(member) {
	}

	string getTypeName() const override {
		return "NStructMember";
	}

#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();

		root["children"].append(id->jsonGen());
		root["children"].append(member->jsonGen());

		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;

		id->print(nPrefix);
		member->print(nPrefix);

	}

#endif

	llvm::Value *codeGen(CodeGenContext&) override;

};

class NArrayIndex : public NExpression {
public:
	shared_ptr<NIdentifier> arrayId;
	shared_ptr<ExpressionList> indexs = make_shared<ExpressionList>();
	int32_t size;

	NArrayIndex(){}

	NArrayIndex(shared_ptr<NIdentifier> arrayId, shared_ptr<NExpression> expr)
		: arrayId(arrayId) {
		indexs->push_back(expr);
	}

	NArrayIndex(shared_ptr<NIdentifier> arrayId, shared_ptr<ExpressionList> list)
		:arrayId(arrayId), indexs(list) {
	}

	string getTypeName() const override {
		return "NArrayIndex";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();

		root["children"].append(arrayId->jsonGen());
		for (auto index = indexs->begin(); index != indexs->end(); index++) {
			root["children"].append((*index)->jsonGen());
		}
		return root;

	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;

		arrayId->print(nPrefix);
		for (auto index = indexs->begin(); index != indexs->end(); index++) {
			(*index)->print(nPrefix);
		}

	}
#endif
	llvm::Value *codeGen(CodeGenContext&) override;

};

class NArrayAssignment : public NExpression {
public:
	shared_ptr<NArrayIndex> arrayInx;
	shared_ptr<NExpression> expr;

	NArrayAssignment(){}

	NArrayAssignment(shared_ptr<NArrayIndex> arrayIndex, shared_ptr<NExpression> expr)
		:arrayInx(arrayIndex), expr(expr){
	}
	
	string getTypeName() const override {
		return "NArrayAssignment";
	}

#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();

		root["children"].append(arrayInx->jsonGen());
		root["children"].append(expr->jsonGen());

		return root;
	}
	
	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;

		arrayInx->print(nPrefix);
		expr->print(nPrefix);
	}
#endif
	llvm::Value *codeGen(CodeGenContext&) override;

};

class NArrayInitialization : public NStatement {
public:

	shared_ptr<NVariableDeclaration> declaration;
	shared_ptr<ExpressionList> expressionList = make_shared<ExpressionList>();

	NArrayInitialization(){}
	NArrayInitialization(shared_ptr<NVariableDeclaration> decl, shared_ptr<ExpressionList> list)
		:declaration(decl), expressionList(list) {
	}

	string getTypeName() const override {
		return "NArrayInitialization";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();

		root["children"].append(declaration->jsonGen());
		for (auto expr = expressionList->begin(); expr != expressionList->end(); expr++)
			root["children"].append((*expr)->jsonGen());

		return root;
	}
	void print(string prefix) const override {

		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;

		declaration->print(nPrefix);
		for (auto expr = expressionList->begin(); expr != expressionList->end(); expr++)
			(*expr)->print(nPrefix);

	}
#endif

	llvm::Value *codeGen(CodeGenContext &context) override;

};

class NStructAssignment : public NExpression {
public:
	shared_ptr<NStructMember> structMember;
	shared_ptr<NExpression> expression;

	NStructAssignment(){}

	NStructAssignment(shared_ptr<NStructMember> member, shared_ptr<NExpression> expr)
		:structMember(member), expression(expr) {
	}

	string getTypeName() const override {
		return "NStructAssignment";
	}

#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName();

		root["children"].append(structMember->jsonGen());
		root["children"].append(expression->jsonGen());

		return root;
	}

	void print(string prefix) const override {
		string nPrefix = prefix + this->m_PREFIX;
		cout << prefix << getTypeName() << this->m_COLON << endl;

		structMember->print(nPrefix);
		expression->print(nPrefix);

	}


#endif

	llvm::Value *codeGen(CodeGenContext&) override;
};


class NLiteral : public NExpression {
public:
	string value;

	NLiteral(){}

	NLiteral(const string &str) {
		value = str.substr(1, str.length() - 2);
	}

	string getTypeName() const override {
		return "NLiteral";
	}
#ifdef PRINT_JOSONGEN
	Json::Value jsonGen() const override {
		Json::Value root;
		root["name"] = getTypeName() + this->m_COLON + value;
		return root;
	}
	void print(string prefix) const override {
		cout << prefix << getTypeName() << this->m_COLON << value << endl;
	}

#endif

	llvm::Value *codeGen(CodeGenContext&) override;

};

std::unique_ptr<NExpression> LogError(const char* str);

#endif
