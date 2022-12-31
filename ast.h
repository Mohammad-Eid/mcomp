/**************************************
 *
 * University of Haifa
 *
 * Theory of compilation
 *
 * P machine compiler - code generation project
 *
 * Yosi Ben Asher
 * Email: yosibenasher@gmail.com
 *
 * Mariah Akree
 * Email: mariah.uni.haifa@gmail.com
 *
 **************************************/
/*   PROGRAM = 258,
     BBEGIN = 259,
     END = 260,
     DECLARE = 261,
     PROCEDURE = 262,
     FUNCTION = 263,
     LABEL = 264,
     INTEGER = 265,
     REAL = 266,
     RECORD = 267,
     BOOLEAN = 268,
     ARRAY = 269,
     OF = 270,
     ASSIGN = 271,
     LC = 272,
     RC = 273,
     IF = 274,
     THEN = 275,
     ELSE = 276,
     WHILE = 277,
     REPEAT = 278,
     FI = 279,
     DO = 280,
     OD = 281,
     READ = 282,
     WRITE = 283,
     TRUE = 284,
     FALSE = 285,
     ADD = 286,
     MIN = 287,
     MUL = 288,
     DIV = 289,
     GOTO = 290,
     MOD = 291,
     LES = 292,
     LEQ = 293,
     EQU = 294,
     NEQ = 295,
     GRE = 296,
     GEQ = 297,
     AND = 298,
     OR = 299,
     NOT = 300,
     CASE = 301,
     FOR = 302,
     FIN = 303,
     IDENTICAL = 304,
     FROM = 305,
     BY = 306,
     TO = 307,
     NEW = 308,
     INTCONST = 309,
     IDE = 310,
     REALCONST = 311,
     STRING = 312,
     DUMMY = 313*/

#ifndef AST_H
#define AST_H

#include <iostream>
#include <assert.h>
#include <string>
#include <iomanip>
#include <vector>
extern int loop_counter;
extern int if_counter;
extern int stacksize;
extern bool codel;
extern bool is_var;
extern bool is_add;
extern bool inc_flag;
extern bool left_flag;
const int MAX = 100;
using namespace std;
extern bool is_switch;
extern bool is_if;
extern double inc_val;
extern bool is_real_const;
extern int switch_counter;
extern int max_switch_count;
extern int cases_counter;
extern bool is_assign;
extern bool is_expr;
extern bool was_expr;
extern bool is_equal;
extern bool is_print;
extern bool is_unary;
extern bool is_var_assign;
extern bool is_var_loop;
extern bool is_address_type;
extern bool is_new;
extern bool is_address_ref;
extern bool is_intern ;
extern bool is_var_declaration;
extern string Extern_name ;
extern int dim_counter;
extern bool is_dim;
extern string ActiveArray;
extern bool is_record_type_var;
extern string extern_name_main;
extern vector<string> recordsPrintVector;
extern string ActiveArray2;
extern bool is_array_ind;
extern bool is_record_ref;
extern bool is_extern;
extern bool print_ixa_in_expr;
extern int print_ixa_in_expr_val;
extern bool got_array_pointer;
extern string array_point_name  ;
extern bool stupid_flag ;
extern string externName2;
/**
 * classes
 */

// base class for tree nodes
class Object {
public:
    virtual void print (ostream& os) = 0;
    virtual string getType() {return " ";};
    virtual int getSize(){return 1;}
    virtual void setArraySizes(string name_){}
    virtual string getName(){ return ""; }
    virtual string getInner(){return "";}
    virtual int getFactor(){return 1;}
    virtual void pcodegen(ostream& os) = 0;
    virtual Object * clone () const {return NULL;}
    virtual ~Object () {}
};


class Variable {

    /* Think! what does a Variable contain? */
    string identifier, type, ptr;
    int address,size;
    Variable* next;

public:
    Variable()
    {
        next = NULL;
    }

    Variable(string key, string type, int address, int size, string ptr)
    {
        this->identifier = key;
        this->size = size;
        this->type = type;
        this->address = address;
        this->ptr = ptr;
        next = NULL;
    }
    friend class SymbolTable;

};




class ArrayClass {

    /* Think! what does a Variable contain? */
    string name;
    int dim;
    int typeSize;
    string innerType;
    vector <int> array_d;
    vector <int> array_l;
    int size;
public:


    ArrayClass(string name_){
        this->name = name_;
        dim = 0;
        innerType = "SimpleType";
        typeSize = 1 ;

    }

    const string &getName() const;

    void setName(const string &name);

    int getSize() const {
        return size;
    }

    void setSize(int size) {
        ArrayClass::size = size;
    }

    int getDim(){
        return dim;
    }

    void setDim(int dim);


    void setArrayD(const vector<int> &arrayD);


    void setArrayL(const vector<int> &arrayL);

    int getSubpart(){
        dim = array_d.size();
        int total=0;
        for (int i =0; i < dim ; i++  ){
            int sum1 =1;
            for (int j = i+1; j< dim ;j++){
                sum1 =  sum1 * array_d[j];
            }
            total += array_l[i] *typeSize * sum1;

        }
        return total;
    }

    void setInnerType(string inner_name){
        innerType=inner_name;
    }
    string getInner(){
        return innerType;
    }
    int getIxaAtIndex(int index){
        int total = 1;

        //dim = array_d.size();
        for(int i=index+1; i < dim+1 ; i++){//check if dim or dim +1
            total *= array_d[i-1];
        }
        return total * typeSize;

    }
    void setFactor(int factor){
        typeSize = factor;
    }

    int getFactor(){
        return typeSize;
    }


    string getName()  {
        return name;
    }

    void setD(int d){
        array_d.push_back(d);
        dim++;
    }

    void setL(int L){
        array_l.push_back(L);
    }
    vector<int> getArrayD()  {
        return array_d;
    }


    vector<int> getArrayL()  {
        return array_l;
    }

    friend class ArraysTable;


};

class ArraysTable{

    vector <ArrayClass> arraysVector;

public:


    ArrayClass& find(const string& name_){

        for(int i=0; i < arraysVector.size(); i++){
            if(arraysVector[i].getName() == name_){
                return arraysVector[i];
            }
        }
    }

    vector<ArrayClass> &getArraysVector()  {
        return arraysVector;
    }

    void setArraysVector(const vector<ArrayClass> &arraysVector) {
        ArraysTable::arraysVector = arraysVector;
    }


};

class RecordField{
    string name;
    string type;
    // if is basic means it's not a record or an array
    bool isBasic;
    int size;
    //if is basic false then we use this field if not is_array then it's a struct
    bool is_array;

public:

    RecordField(const string &name, const string &type, bool isBasic, int addressInRecord, bool isArray)
            : name(name), type(type),
              isBasic(isBasic),
              size(
                      addressInRecord) {}

    string &getName()  {
        return name;
    }

    void setName(const string &name) {
        RecordField::name = name;
    }

    string &getType()  {
        return type;
    }

    void setType(const string &type) {
        RecordField::type = type;
    }

    bool isBasic1() {
        return isBasic;
    }

    void setIsBasic(bool isBasic) {
        RecordField::isBasic = isBasic;
    }

    int getSizeInRecord()  {
        return size;
    }

    void setAddressInRecord(int addressInRecord) {
        RecordField::size = addressInRecord;
    }
};


class Record{
    string name;
    vector<RecordField> fieldsInRecord;
    int size;

public:
    Record(const string &name ="", const vector<RecordField> &fieldsInRecord = vector<RecordField>(), int size = 0) : name(name),
                                                                                                                      fieldsInRecord(fieldsInRecord),
                                                                                                                      size(size) {}

    string &getName() {
        return name;
    }

    void setName(const string &name) {
        Record::name = name;
    }

    vector<RecordField> &getFieldsInRecord(){
        return fieldsInRecord;
    }

    void setFieldsInRecord(const vector<RecordField> &fieldsInRecord) {
        Record::fieldsInRecord = fieldsInRecord;
    }

    int getSize(){
        return size;
    }

    void setSize(int size) {
        Record::size = size;
    }

    RecordField& getRecordFieldByName(string name_){
        for(int i = 0; i < fieldsInRecord.size() ; i++){
            if(fieldsInRecord[i].getName() == name_){
                return fieldsInRecord[i];
            }

        }
    }
    int getFieldAddressInRecordByName(string name){

        int address = 0;
        for(int i =0; i < fieldsInRecord.size(); i++){
            if(fieldsInRecord[i].getName() == name){
                return  address;
            }
            address+= fieldsInRecord[i].getSizeInRecord();
        }
    }

    int getRecordActualSize(){
        int RecordSize =0 ;
        for(int i =0; i < fieldsInRecord.size();i++){
            RecordSize += fieldsInRecord[i].getSizeInRecord();
        }
        return RecordSize;
    }

    string getFieldTypeByName(string name){

        for(int i =0; i < fieldsInRecord.size();i++){
            if(fieldsInRecord[i].getName() == name){
                return fieldsInRecord[i].getType();
            }
        }
    }

};


class RecordsTable{
    vector<Record> RecordsVector;
public:
    RecordsTable() = default;

    RecordsTable(const vector<Record> &recordsVector) : RecordsVector(recordsVector) {}

    vector<Record> &getRecordsVector() {
        return RecordsVector;
    }

    void setRecordsVector(const vector<Record> &recordsVector) {
        RecordsVector = recordsVector;
    }

    Record& getRecordByName(string name_){
        for(int i = 0; i < RecordsVector.size(); i++){
            if(RecordsVector[i].getName() == name_ ){
                return RecordsVector[i];
            }
        }
    }

    int getAddressOfField(vector<string> vec){


        if(vec.size() == 2){
            return this->getRecordByName(vec[0]).getFieldAddressInRecordByName(vec[1]);
        }
        vector<string> tmp;
        tmp.push_back(this->getRecordByName(vec[0]).getFieldTypeByName(vec[1]));
        for(int i=2;i<vec.size();i++){
            tmp.push_back(vec[i]);
        }
        return getAddressOfField(tmp);

    }


};

class SymbolTable {
    /* Think! what can you add to  symbol_table */
    Variable* head[MAX];

public:
    SymbolTable()
    {
        for (int i = 0; i < MAX; i++)
            head[i] = NULL;
    }

    // Function to find an identifier
    string findTypeByName(string id)
    {
        int index = hashf(id);
        Variable* start = head[index];

        if (start == NULL)
            return "";

        while (start != NULL) {

            if (start->identifier == id) {
                return start->type;
            }

            start = start->next;
        }

        return ""; // not found
    }


    int find(string id)
    {
        int index = hashf(id);
        Variable* start = head[index];

        if (start == NULL)
            return -1;

        while (start != NULL) {

            if (start->identifier == id) {
                return start->address;
            }

            start = start->next;
        }

        return -1; // not found
    }

    int findType(string id)
    {
        int index = hashf(id);
        Variable* start = head[index];

        if (start == NULL)
            return -1;

        while (start != NULL) {

            if (start->identifier == id) {
                if(start->type == "AddressType") {
                    return this->findType(start->ptr);
                }
                return start->address;
            }
            start = start->next;
        }

        return -1; // not found
    }


    int sizeFind(string id){
        int index = hashf(id);
        Variable* start = head[index];

        if (start == NULL)
            return -1;

        while (start != NULL) {

            if (start->identifier == id) {
                if(start->type == "AddressType") {
                    return this->sizeFind(start->ptr);
                }
//                if(start->type == "RecordType"){
//                    return
//                }
                return start->size;
            }

            start = start->next;
        }

        return -1; // not found
    }

    // Function to insert an identifier
    bool insert(string id, string type, int address, int size, string ptr)
    {
        if (find(id) != -1)
            return false;

        int index = hashf(id);
        Variable* p = new Variable(id, type, address, size, ptr);

        if (head[index] == NULL) {
            head[index] = p;
            return true;
        }

        else {
            Variable* start = head[index];
            while (start->next != NULL)
                start = start->next;
            start->next = p;
            return true;
        }

        return false;
    }

    int hashf(string id)
    {
        int asciiSum = 0;

        for (int i = 0; i < id.length(); i++) {
            asciiSum = asciiSum + id[i];
        }

        return (asciiSum % MAX);
    }

    void print()
    {
        cout << "Name\t\tType\t\tAdress\t\tSize" << endl;

        for (int h = 0; h < MAX; h++)
        {
            if (head[h] == NULL) continue;
            cout << head[h]->identifier << "\t\t";
            cout << head[h]->type << "\t\t";
            cout << head[h]->address << "\t\t";
            cout << head[h]->size << endl;
        }
    }



};
extern SymbolTable ST ;
extern  ArraysTable ArraysST;
extern RecordsTable RecordsST;


class Expr : public Object {
public :
    // Unary operations
    Expr (int op, Object * atom) : op_(op), atom_(atom), left_(NULL), right_(NULL), unary_(true) {}
    // Binary operations
    Expr (int op, Object * left, Object * right) : op_(op), left_(left), right_(right), atom_(NULL), unary_(false) {}

    Expr(const Expr & exp){
        unary_= exp.unary_;
        op_ = exp.op_;
        left_ = exp.left_->clone();
        right_ = exp.right_->clone();
        atom_ = exp.atom_->clone();
    }

    virtual ~Expr(){
        if (left_) delete left_;
        if (right_) delete right_;
        if (atom_) delete atom_;
    }


    void print (ostream& os) {
        os<<"Node name : Expr"<<endl;
        assert(op_);
        if (unary_){
            os<<"Unary op is : "<<op_;
            assert(atom_);
            atom_->print(os);
        }else{
            os<<"Binary op is : "<<op_;
            assert(left_ && right_);
            left_->print(os);
            right_->print(os);
        }
    }
    void pcodegen(ostream& os) {
        assert(op_);
        codel = false;
        if( op_ >= 292 && op_ <= 297){
            is_equal = true;
        } else if(!left_flag){
            is_equal = false;
        }

        if(op_ == 286 || op_==287){
            is_add = true;
        } else if (!left_flag){
            is_add = false;
        }
        if (unary_) {
            assert(atom_);
            if (op_ == 287){
                is_unary = true;
            }
            atom_->pcodegen(os);
            is_unary = false;
            switch(op_){
                case 287:
                    os << "neg"<<endl;
                    break;
                case 300:
                    os << "not"<<endl;
                    break;
            }

        }
        else {

            assert(left_ && right_);
            left_flag = false;
            right_->pcodegen(os);
            codel = false;
            left_flag = true;
            is_expr = true;
            if(op_ == 286 || op_==287){
                is_add = true;
            } else{
                is_add = false;
            }
            if( op_ >= 292 && op_ <= 297){
                is_equal = true;
            } else {
                is_equal = false;
            }
            left_->pcodegen(os);

            switch(op_) {
                case 286:
                    if(inc_flag){
                        if(is_real_const) {
                            os << "inc " << inc_val << endl; //todo double check
                            is_real_const= false;
                        }
                        else{
                            os << "inc " << (int)inc_val << endl;
                        }
                        inc_flag = false;
                    }
                    else
                        os << "add"<< endl;
                    break;
                case 287:
                    if(inc_flag){
                        if(is_real_const) {
                            os << "dec " << inc_val << endl; //todo double check
                            is_real_const= false;
                        }
                        else{
                            os << "dec " << (int)inc_val << endl;
                        }
                        inc_flag = false;
                    }
                    else
                        os << "sub"<< endl;
                    break;
                case 296:
                    os << "grt" << endl;
                    break;
                case 292:
                    os << "les" << endl;
                    break;
                case 298:
                    os << "and" << endl;
                    break;
                case 294:
                    os << "equ" << endl;
                    break;
                case 288:
                    os << "mul" << endl;
                    break;
                case 289:
                    os << "div" << endl;
                    break;
                case 297:
                    os << "geq" << endl;
                    break;
                case 293:
                    os << "leq" << endl;
                    break;
                case 299:
                    os << "or" << endl;
                    break;
                case 308:
                    os << "new" << endl;
                    break;
            }
//            is_equal = false;
            // somemessage
            inc_flag = false;
            is_var = false;
            is_expr = false;
        }
    }
    virtual Object * clone () const { return new Expr(*this);}

private:
    bool unary_;
    int op_;
    Object * left_;
    Object * right_;
    Object * atom_;
};

class ExprList : public Object {
public :
    ExprList (Object * expr) : expr_(expr),expr_list_(NULL){assert(expr_);}
    ExprList (Object * expr, Object * expr_list) : expr_(expr),expr_list_(expr_list) {assert(expr_ && expr_list_);}

    ExprList(const ExprList& exp){
        expr_ = exp.expr_->clone();
        expr_list_ = exp.expr_list_->clone();
    }

    virtual ~ExprList(){
        if (expr_) delete expr_;
        if (expr_list_) delete expr_list_;
    }

    void print (ostream& os) {
        os<<"Node name : ExprList";
        assert( expr_);
        expr_->print(os);
        if (expr_list_){
            expr_list_->print(os);
        }
    }
    void pcodegen(ostream& os) {
        assert(expr_);
        expr_->pcodegen(os);
        if (expr_list_) {
            expr_list_->pcodegen(os);
        }
    }
    virtual Object * clone () const { return new ExprList(*this);}

private:
    Object * expr_;
    Object * expr_list_;
};

class Dim : public Object {
public:
    Dim (Object * exp) : exp_(exp), dim_(NULL) {assert(exp_);}
    Dim (Object * exp, Object * dim) : exp_(exp),dim_(dim) {assert(exp_ && dim_);}

    Dim(const Dim& d){
        exp_ = d.exp_->clone();
        dim_ = d.dim_->clone();
    }

    virtual ~Dim(){
        if (exp_) delete exp_;
        if (dim_) delete dim_;
    }

    void print (ostream& os) {
        os<<"Node name : Dim"<<endl;
        assert(exp_);
        exp_->print(os);
        if (dim_){
            dim_->print(os);
        }
    }
    void pcodegen(ostream& os) {
        assert(exp_);

        exp_->pcodegen(os);

        int counterBackup = dim_counter;
        string arrayBackup = ActiveArray;
        if(print_ixa_in_expr){
            print_ixa_in_expr = false;
            os << "ixa " << print_ixa_in_expr_val << endl;
        }
        if(dim_counter == ArraysST.find(ActiveArray).getDim()){
            os<<"dec "<<ArraysST.find(ActiveArray).getSubpart() <<endl;
            ActiveArray = ArraysST.find(ActiveArray).getInner();
            dim_counter = 0;

            if(((is_assign  && !is_add ) || is_equal || is_print ) && ActiveArray.empty() ){
                os<<"ind"<<endl;
            }
        }

        dim_counter++;
        if (dim_) {
            dim_->pcodegen(os);
        }
//        dim_counter = counterBackup;
        //os<<"dec "<<ArraysST.find(ActiveArray).getSubpart() <<endl;//dec 4lt
    }
    virtual Object * clone () const { return new Dim(*this);}

private:
    Object * exp_;
    Object * dim_;
};

class Atom : public Object {
};

class IntConst : public Atom {
public:
    IntConst(const int i) : i_(i) {}
    IntConst(const IntConst& in) : i_(in.i_) {}

    void print (ostream& os) {
        os<<"Node name : IntConst. Value is :"<<i_<<endl;
    }
    void pcodegen(ostream& os) {
        is_real_const=false;
        if(!is_var_declaration) {
            if (left_flag && ((is_equal || is_assign || is_add) && is_expr)) {
                inc_flag = true;
            }
            if (!left_flag || !is_add ||
                ((is_print || is_assign) && !is_expr) || is_unary ||
                (is_switch && !is_expr)) {

                if (is_dim && !is_add ) {
                    int temp = ArraysST.find(ActiveArray).getIxaAtIndex(
                            dim_counter);
                    os << "ldc " << i_ << endl;
                    if(is_expr){
                        print_ixa_in_expr = true;
                        print_ixa_in_expr_val = temp;
                    }else {

                        os << "ixa " << temp << endl;
                    }
                } else {
                    os << "ldc " << i_ << endl;
                }


            }
        }
        inc_val = i_;
//           is_var =false;
    }
    virtual Object * clone () const { return new IntConst(*this);}

private:
    const int i_;
};

class RealConst : public Atom {
public:
    RealConst(const double r) : r_(r) {}
    RealConst(const RealConst& in) : r_(in.r_) {}

    void print (ostream& os) {
        os<<"Node name : RealConst. Value is :"<<r_<<endl;
    }
    void pcodegen(ostream& os) {
        is_real_const = true;
        if(!is_var_declaration) {
            if (left_flag && ((is_equal || is_assign || is_add) && is_expr)) {
                inc_flag = true;
            }
            if (!left_flag || !is_add ||
                ((is_print || is_assign) && !is_expr) || is_unary) {

                os << "ldc " << std::fixed << std::setprecision(1) << r_
                   << endl;

            }
            inc_val = r_;
        }
    }
    virtual Object * clone () const { return new RealConst(*this);}

private:
    const double r_;
};

class True : public Atom {
public:
    void print (ostream& os) {
        os<<"Node name : trueConst. Value is true"<<endl;
    }
    void pcodegen(ostream& os) {
    }
    virtual Object * clone () const { return new True();}

};

class False : public Atom {
public :
    void print (ostream& os) {
        os<<"Node name : trueConst. Value is false"<<endl;
    }
    void pcodegen(ostream& os) {
    }
    virtual Object * clone () const { return new False();}
};

class Var : public Atom {
};

class ArrayRef : public Var {
public :
    ArrayRef (Object * var, Object * dim) : var_(var),dim_(dim) {assert(var_ && dim_);}
    ArrayRef(const ArrayRef& arr){
        var_ = arr.var_->clone();
        dim_ = arr.dim_->clone();
    }

    virtual ~ArrayRef(){
        if (var_) delete var_;
        if (dim_) delete dim_;
    }

//    string getName(){
//        return var_->getName();
//}
    void print (ostream& os) {
        os<<"Node name : ArrayRef"<<endl;
        assert(var_ && dim_);
        var_->print(os);
        dim_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(var_ && dim_);
        is_array_ind = true;
        var_->pcodegen(os);
        ActiveArray = var_->getName();
        dim_counter = 1;
        is_dim =true;
        is_array_ind = false;

        dim_->pcodegen(os);

//        os<<"dec "<<ArraysST.find(ActiveArray).getSubpart() <<endl;//dec 4lt
        is_dim = false;
        ActiveArray2 = var_->getName();
        dim_counter = 1;
    }
    virtual Object * clone () const { return new ArrayRef(*this);}

private:
    Object * var_;
    Object * dim_;
};

class RecordRef : public Var {
public :
    RecordRef (Object * varExt, Object * varIn) : varExt_(varExt),varIn_(varIn) {assert(varExt_ && varIn_);}
    RecordRef(const RecordRef& rec){
        varExt_ = rec.varExt_->clone();
        varIn_ = rec.varIn_->clone();
    }

    virtual ~RecordRef(){
        if (varExt_) delete varExt_;
        if (varIn_) delete varIn_;
    }

    void print (ostream& os) {
        os<<"Node name : RecordRef"<<endl;
        assert(varExt_ && varIn_);
        varExt_->print(os);
        varIn_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(varExt_ && varIn_);
        //extern_name_main =  varExt_->getName();
        is_record_ref = true;
        is_extern = true;
        varExt_->pcodegen(os);
        is_record_ref = false;
        is_intern = true;
        Extern_name = varExt_->getName();
        stupid_flag =true;
        externName2 = varExt_->getName();
        recordsPrintVector.push_back(Extern_name);
        varIn_->pcodegen(os);
        recordsPrintVector.clear();
        is_intern = false;
        is_extern= false;
    }
    virtual Object * clone () const { return new RecordRef(*this);}

private:
    Object * varExt_;
    Object * varIn_;
};

class AddressRef : public Var {
public :
    AddressRef (Object * var) : var_(var) {assert(var_);}
    AddressRef(const AddressRef& addre){
        var_ = addre.var_->clone();
    }

    virtual ~AddressRef(){
        if (var_) delete var_;
    }

//    string getName(){
//    if (stupid_flag){
//        stupid_flag= false;
//        string temp = var_->getName();
//        return ArraysST.find(temp).getInner();
//    }
//    else
//        return var_->getName();
//    }

    void print (ostream& os) {
        os<<"Node name : AddressRef"<<endl;
        assert(var_);
        var_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(var_);
        is_address_ref = true;
        var_->pcodegen(os);
        is_address_ref = false;


    }
    virtual Object * clone () { return new AddressRef(*this);}

private:
    Object * var_;
};

class Statement : public Object {
};

class NewStatement : public Statement {
public :
    NewStatement (Object * var) : var_(var) {assert(var_);}
    NewStatement(const NewStatement& ns){
        var_ = ns.var_->clone();
    }

    virtual ~NewStatement(){
        if (var_) delete var_;
    }

    void print (ostream& os) {
        os<<"Node name : NewStatement";
        assert(var_);
        var_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(var_);
        is_new = true;
        var_->pcodegen(os);
//            os<< "ldc" <<"   new_size_should_be_here"<<endl;
//            os << "new"<<endl;

        is_new = false;
    }

    virtual Object * clone () { return new NewStatement(*this);}

private:
    Object * var_;
};

class WriteStrStatement : public Statement {
public :
    WriteStrStatement (const char * str) {
        str_ = new string(str);
    }

    WriteStrStatement(const WriteStrStatement& ns){
        str_ = new string(*ns.str_);
    }

    virtual ~WriteStrStatement () {
        if (str_) delete str_;
    }
    void print (ostream& os) {
        os<<"Node name : WriteStrStatement";
        assert(str_);
        os<<"Str statement is: "<<str_<<endl;
    }
    void pcodegen(ostream& os) {
        assert(str_);
    }
    virtual Object * clone () { return new WriteStrStatement(*this);}

private:
    string * str_;
};

class WriteVarStatement : public Statement {
public :
    WriteVarStatement (Object * exp) : exp_(exp) {assert(exp_);}

    WriteVarStatement(const WriteVarStatement& ex){
        exp_ = ex.clone();
    }

    virtual ~WriteVarStatement(){
        if (exp_) delete exp_;
    }

    void print (ostream& os) {
        os<<"Node name : WriteVarStatement";
        assert(exp_);
        exp_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(exp_);
        is_print =true;
        exp_->pcodegen(os);
        if(was_expr){
            os<<"ind"<<endl;
            was_expr = false;
        }
        is_print = false;
        os <<"print"<< endl;
    }
    virtual Object * clone () const { return new WriteVarStatement(*this);}

private:
    Object * exp_;
};

class ProcedureStatement : public Statement {
public :
    ProcedureStatement (const char * str) {
        str_ = new string(str);
    }

    ProcedureStatement (Object * expr_list, const char * str) : expr_list_(expr_list) {
        assert(expr_list_);
        str_ = new string(str);
    }

    ProcedureStatement(const ProcedureStatement& ps){
        expr_list_ = ps.expr_list_->clone();
        str_ = new string(*ps.str_);
    }

    virtual ~ProcedureStatement () {
        if (str_) delete str_;
        if (expr_list_) delete expr_list_;
    }

    void print (ostream& os) {
        os<<"Node name : ProcedureStatement. Proc name : "<<str_<<endl;
        if (expr_list_ ){
            expr_list_->print(os);
        }
    }
    void pcodegen(ostream& os) {
        if (expr_list_) {
            expr_list_->pcodegen(os);
        }
    }
    virtual Object * clone () const { return new ProcedureStatement(*this);}

private:
    Object * expr_list_;
    string * str_;
};

class Case : public Object {
public :
    Case (Object * stat_list, int val) : leafChild_ (NULL), stat_list_(stat_list) {
        // note the special treatment in miny.y for this case (makenode)
        leafChild_ = new IntConst(val);
        assert(stat_list_);
    }

    Case(const Case& c){
        stat_list_ = c.stat_list_->clone();
        leafChild_ = c.leafChild_->clone();
    }

    virtual ~Case () {
        if (stat_list_) delete stat_list_;
        if (leafChild_) delete leafChild_;
    }

    void print (ostream& os) {
        os<<"Node name : Case";
        assert(stat_list_);
        stat_list_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(stat_list_);
        int switch_count=switch_counter;
        stat_list_->pcodegen(os);
        os << "ujp "<<"end_switch_"<< switch_count<<endl;
    }
    virtual Object * clone () const { return new Case(*this);}

private:
    Object * stat_list_;
    Object * leafChild_;
};


class CaseList : public Object {
public :
    CaseList (Object * ccase) : case_(ccase),case_list_(NULL) {assert(case_);}
    CaseList (Object * ccase, Object * case_list) : case_(ccase),case_list_(case_list) {assert(case_ && case_list_);}

    CaseList(const CaseList& cl){
        case_ = cl.case_->clone();
        case_list_ = cl.case_list_->clone();
    }

    virtual ~CaseList () {
        if (case_) delete case_;
        if (case_list_) delete case_list_;
    }

    void print (ostream& os) {
        os<<"Node name : CaseList";
        assert( case_ );
        case_->print(os);
        if (case_list_){
            case_list_->print(os);
        }
    }
    void pcodegen(ostream& os) {
        assert(case_);
        cases_counter+=1;
        os << "case_"<<cases_counter<<"_"<<switch_counter<<":"<<endl;
        int counter_backup = cases_counter;
        int switch_backup = switch_counter;
        cases_counter = 0 ;
        case_->pcodegen(os);
        cases_counter = counter_backup; // new line
        switch_counter = switch_backup; // new line
        if (case_list_) {
            case_list_->pcodegen(os);
        }

        os << "ujp "<<"case_"<<cases_counter<<"_"<<switch_backup<<endl;
        cases_counter--;
    }
    virtual Object * clone () const { return new CaseList(*this);}

private:
    Object * case_;
    Object * case_list_;
};

class CaseStatement : public Statement {
public :
    CaseStatement (Object * exp, Object * case_list) : exp_(exp),case_list_(case_list) {assert(exp_ && case_list_);}

    CaseStatement(const CaseStatement& cs){
        exp_ = cs.exp_->clone();
        case_list_ = cs.case_list_->clone();
    }

    virtual ~CaseStatement () {
        if (exp_) delete exp_;
        if (case_list_) delete case_list_;
    }

    void print (ostream& os) {
        os<<"Node name : CaseStatement";
        assert( exp_ && case_list_);
        exp_->print(os);
        case_list_->print(os);
    }
    void pcodegen(ostream& os) {
        max_switch_count++;
        switch_counter +=1;
        if(switch_counter!=max_switch_count){
            switch_counter=max_switch_count;
        }
        assert(exp_ && case_list_);
        is_switch=true;
        exp_->pcodegen(os);
        is_switch=false;
        if((is_expr||is_var)&&!codel){
            os << "ind"<<endl; // this shouldn't be always printed
        }
        os<< "neg"<<endl;
        os << "ixj end_switch_" <<switch_counter<< endl;
        int switch_counter_backup = switch_counter;
        case_list_->pcodegen(os);
//        for(int i = cases_counter; i > 0 ; i--){
//            os << "ujp "<<"case_"<<i<<"_"<<switch_counter<<endl;
//        }
        cases_counter = 0;
        os <<"end_switch_"<<switch_counter_backup<<":"<<endl;
        //switch_counter--;
    }
    virtual Object * clone () const { return new CaseStatement(*this);}

private:
    Object * exp_;
    Object * case_list_;
};

class LoopStatement : public Statement {
public :
    LoopStatement (Object * exp, Object * stat_list) : exp_(exp),stat_list_(stat_list) {assert(exp_ && stat_list_);}

    LoopStatement(const LoopStatement& ls){
        exp_ = ls.exp_->clone();
        stat_list_ = ls.stat_list_->clone();
    }

    virtual ~LoopStatement () {
        if (exp_) delete exp_;
        if (stat_list_) delete stat_list_;
    }

    void print (ostream& os) {
        os<<"Node name : LoopStatement";
        assert( exp_ && stat_list_);
        exp_->print(os);
        stat_list_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(exp_ && stat_list_);
        int loop_counter_2 = ++loop_counter;
        os << "loop_" << loop_counter_2<<":" << endl;
        is_var_loop = true;
        exp_->pcodegen(os);
        is_var_loop = false;
        os << "fjp end_loop_"<< loop_counter_2 << endl;
        stat_list_->pcodegen(os);
        os << "ujp loop_"<< loop_counter_2 << endl;
        os << "end_loop_"<< loop_counter_2 << ":"<< endl;
        //loop_counter++;
    }
    virtual Object * clone () const { return new LoopStatement(*this);}

private:
    Object * exp_;
    Object * stat_list_;
};


class ConditionalStatement : public Statement {
public :
    ConditionalStatement (Object * exp, Object * stat_list_if) : exp_(exp),stat_list_if_(stat_list_if), stat_list_else_(NULL) {assert(exp_ && stat_list_if_);}
    ConditionalStatement (Object * exp, Object * stat_list_if, Object * stat_list_else) : exp_(exp),stat_list_if_(stat_list_if), stat_list_else_(stat_list_else) {assert(exp_ && stat_list_if_ && stat_list_else_ );}

    ConditionalStatement(const ConditionalStatement& cs){
        exp_ = cs.exp_->clone();
        stat_list_if_ = cs.stat_list_if_->clone();
        stat_list_else_ = cs.stat_list_else_->clone();
    }

    virtual ~ConditionalStatement () {
        if (exp_) delete exp_;
        if (stat_list_if_) delete stat_list_if_;
        if (stat_list_else_) delete stat_list_else_;
    }

    void print (ostream& os) {
        os<<"Node name : ConditionalStatement";
        assert( exp_ && stat_list_if_);
        exp_->print(os);
        stat_list_if_->print(os);
        if (stat_list_else_){
            stat_list_else_->print(os);
        }
    }
    void pcodegen(ostream& os) {
        int counter =++if_counter;
        assert(exp_ && stat_list_if_);
        is_if = true;
        exp_->pcodegen(os);
        is_if = false;

        if(!stat_list_else_){
            os << "fjp end_if_"<< counter << endl;
            stat_list_if_->pcodegen(os);
        }

        if (stat_list_else_) {

            os << "fjp else_if_"<< counter << endl;
            stat_list_if_->pcodegen(os);
            os << "ujp end_if_"<< counter << endl;
            os<<"else_if_"<<counter << ":" << endl;
            stat_list_else_->pcodegen(os);


        }
        os << "end_if_"<< counter <<":"<< endl;

    }
    virtual Object * clone () const { return new ConditionalStatement(*this);}

private:
    Object * exp_;
    Object * stat_list_if_;
    Object * stat_list_else_;
};


class Assign : public Statement {
public :
    Assign (Object * var, Object * exp) : var_(var), exp_(exp) {assert(var_ && exp_);}

    Assign(const Assign& as){
        var_ = as.var_->clone();
        exp_ = as.exp_->clone();
    }

    virtual ~Assign () {
        if (exp_) delete exp_;
        if (var_) delete var_;
    }

    void print (ostream& os) {
        os<<"Node name : Assign";
        assert(var_ && exp_);
        exp_->print(os);
        var_->print(os);

    }
    void pcodegen(ostream& os) {
        assert(var_ && exp_);
        //os << "hello from class assign" << endl;
        codel = true;

        exp_->pcodegen(os);
        is_assign = true;
        is_var_assign =true;
        var_->pcodegen(os);
        is_assign = false;
        is_expr = false;
        is_var_assign = false;
        os << "sto" << endl;

    }
    virtual Object * clone () const { return new Assign(*this);}

private:
    Object * var_;
    Object * exp_;
};

class StatementList : public Object {
public :
    StatementList (Object * stat) : stat_list_(NULL), stat_(stat)  { assert(stat_);}
    StatementList (Object * stat_list, Object * stat) : stat_list_(stat_list),stat_(stat)  {assert(stat_list_ && stat);}

    StatementList (const StatementList& sl){
        stat_ = sl.stat_->clone();
        stat_list_ = sl.stat_list_->clone();
    }

    virtual ~StatementList () {
        if (stat_) delete stat_;
        if (stat_list_) delete stat_list_;
    }

    void print (ostream& os) {
        if (stat_list_){
            stat_list_->print(os);
            os<<"Node name : StatementList"<<endl;
        }
        assert(stat_);
        stat_->print(os);
    }
    void pcodegen(ostream& os) {
        if (stat_list_) {
            stat_list_->pcodegen(os);
        }
        assert(stat_);
        stat_->pcodegen(os);
    }
    virtual Object * clone () const { return new StatementList(*this);}

private:
    Object * stat_;
    Object * stat_list_;
};

class RecordList : public Object {
public :
    RecordList (Object * var_decl) : record_list_(NULL), var_decl_(var_decl)  { assert(var_decl_);}
    RecordList (Object * record_list, Object * var_decl) : record_list_(record_list),var_decl_(var_decl)  {assert(record_list_ && var_decl);}

    RecordList(const RecordList& li){
        var_decl_= li.var_decl_->clone();
        record_list_ = li.record_list_->clone();
    }

    virtual ~RecordList () {
        if (var_decl_) delete var_decl_;
        if (record_list_) delete record_list_;
    }

    void print (ostream& os) {
        os<<"Node name : RecordList"<<endl;
        if (record_list_){
            record_list_->print(os);
        }
        assert(var_decl_);
        var_decl_->print(os);
    }
    void pcodegen(ostream& os) {
        if (record_list_) {
            record_list_->pcodegen(os);
        }
        assert(var_decl_);
        var_decl_->pcodegen(os);
    }
    virtual Object * clone () const { return new RecordList(*this);}

private:
    Object * var_decl_;
    Object * record_list_;
};

class Type : public Object {
};

class SimpleType : public Type {
public:
    SimpleType (const char * name) {
        name_ = new string(name);
    }

    virtual ~SimpleType () {
        if (name_ )delete name_;
    }

    SimpleType(const SimpleType& s){
        name_ = new string(*s.name_);
    }

    void print (ostream& os) {
        os<<"Node name : SimpleType"<<endl;;
        os<<"Type is : "<< (*name_) <<endl;
    }
    string getType(){
        return "SimpleType";
    }
    int getSize(){
        return 1;
    }
    void pcodegen(ostream& os) {
    }
    virtual Object * clone () const { return new SimpleType(*this);}

private:
    string * name_;
};



class IdeType : public Type {
public:
    IdeType (const char * name) {
        name_ = new string(name);
    }

    virtual ~IdeType () {
        if (name_) delete name_;
    }

    IdeType(const IdeType& s){
        name_ = new string(*s.name_);
    }

    string getName(){
        return  *name_;
    }

    void print (ostream& os) {
        os<<"Node name : IdeType"<<endl;
    }
    string getType(){
        return "IdeType";
    }
    int getSize(){
        return 1;
    }
    void pcodegen(ostream& os) {
        is_var =true;
        if(!is_address_type && !is_var_declaration) { //////////////////////////////////////////////////////////////////////////////////////
            bool addresTypeDeref = false;
            if(is_intern){
                int a = ST.find(*name_);
                int b =ST.findType(Extern_name);
                string typeInSt = ST.findTypeByName(Extern_name);
                string s = *name_;
                string main_string_betsa = extern_name_main;
                extern_name_main = Extern_name;

                if(typeInSt == "IdeType") {
                    recordsPrintVector.push_back(*name_);
                        os << "inc " << RecordsST.getAddressOfField(recordsPrintVector) << endl;
                } else if(typeInSt == "AddressType"){
                    os << "inc " << ST.find(*name_)-ST.findType(Extern_name) << endl;
                    addresTypeDeref = true;
                }else if(typeInSt == "" && ST.findTypeByName(ActiveArray2) == "ArrayType" ){
                    os<<"inc "<<RecordsST.getRecordByName(ArraysST.find(ActiveArray2).getInner()).getFieldAddressInRecordByName(*name_)<<endl;
                }else if(Extern_name == ""){
                    os<<"inc "<<RecordsST.getRecordByName(externName2).getFieldAddressInRecordByName(*name_)<<endl;
                    addresTypeDeref = true;
                }else{

                    os<<"inc "<<RecordsST.getRecordByName(Extern_name).getFieldAddressInRecordByName(*name_)<<endl;

                }

            }
            else{
                os << "ldc " << ST.find(*name_) << endl;
            }
            if(is_address_ref && is_record_ref){
                os << "ind" <<endl;
            }


            if(!is_array_ind && !is_record_ref ) {
                // we have to change the 5 here so when it actually increases
                if ((!codel && !is_new) || is_print || is_var_assign || is_if || is_var_loop ||
                    (is_switch && !is_expr) || is_address_ref ) {
                    os << "ind" << endl;
                    codel = true;
                }
            }



            if(is_new && !is_address_ref) {
                os << "ldc"  <<" "<< ST.sizeFind(*name_)<< endl;
                os << "new" << endl;
            }
        }



    }
    virtual Object * clone () const { return new IdeType(*this);}

private:
    string * name_;
};

class ArrayType : public Type {
public :
    ArrayType (int l,int u, Object * type) : low_(l),up_(u),type_(type)  { assert(type_);}

    ArrayType(const ArrayType& a) : low_(a.low_), up_(a.up_){
        type_ = a.type_->clone();
    }

    virtual ~ArrayType () {
        if (type_) delete type_;
    }

    void print (ostream& os) {
        os<<"Node name : ArrayType: low bound is: "<<low_<<", up bound is: "<<up_<<endl;
        assert(type_);
        type_->print(os);
    }
    string getType(){
        return "ArrayType";
    }
    string getName(){
        return type_->getName();
    }
    int getSize(){
        int size = up_ - low_ +1 ;
        size *= type_->getSize();
        return size;
    }
    int getFactor(){
        if (type_->getType() != "SimpleType"){
            int temp = ST.sizeFind(type_->getName());
            if (temp == -1){
                return 1;
            }
            else{
                return temp;
            }
        }
        else
            return 1;
    }

    string getInner(){
        if (type_->getType() != "SimpleType"){
            return type_->getName();
        }
        else return "SimpleType";
    }

    void setArraySizes(string name_){

        int tmp = up_-low_+1;
        ArraysST.find(name_).setD(tmp);

        ArraysST.find(name_).setL(low_);

        if(getType() != "SimpleType") {
            type_->setArraySizes(name_);
        }

    }

    void pcodegen(ostream& os) {
        assert(type_);
        type_->pcodegen(os);
    }
    virtual Object * clone () const { return new ArrayType(*this);}

private:
    Object * type_;
    int low_,up_;
};

class RecordType : public Type {
public :
    RecordType (Object * record_list) : record_list_(record_list)  { assert(record_list_);}

    RecordType(const RecordType& y){
        record_list_ = y.record_list_->clone();
    }

    virtual ~RecordType () {
        if (record_list_) delete record_list_;
    }

    void print (ostream& os) {
        os<<"Node name : RecordType"<<endl;
        assert(record_list_);
        record_list_->print(os);
    }
    string getType(){
        return "RecordType";
    }
    int getSize(){
        return 1;
    }
    void pcodegen(ostream& os) {
        assert(record_list_);
        record_list_->pcodegen(os);
    }
    virtual Object * clone () const { return new RecordType(*this);}

private:
    Object * record_list_;
};


class AddressType : public Type {
public :
    AddressType (Object * type) : type_(type)  { assert(type_);}

    AddressType(const AddressType& t){
        type_ = t.type_->clone();
    }

    virtual ~AddressType () {
        if (type_) delete type_;
    }

    void print (ostream& os) {
        os<<"Node name : AddressType"<<endl;
        assert(type_);
        type_->print(os);
    }
    string getType(){
        return "AddressType";
    }

    string getInner(){
    return type_->getType();
}
    string getName(){
        return  type_->getName();
    }
    void pcodegen(ostream& os) {
        assert(type_);
        if(got_array_pointer){
            got_array_pointer =false;
            ArraysST.getArraysVector().push_back(ArrayClass(array_point_name));
            int typeSize = type_->getFactor();
            ArraysST.find(array_point_name).setInnerType(type_->getInner());
            int size = typeSize*type_->getSize();
            ArraysST.find(array_point_name).setFactor(typeSize);
            ArraysST.find(array_point_name).setSize(size);
            type_->setArraySizes(array_point_name);

        }
        is_address_type = true;
        type_->pcodegen(os);
        is_address_type = false;
    }
    virtual Object * clone () const { return new AddressType(*this);}

private:
    Object * type_;
};


class Declaration : public Object {
};

class VariableDeclaration : public Declaration {
public:
    VariableDeclaration (Object * type, const char * str) : type_(type){
        assert(type_);
        name_ = new string(str);
    }

    VariableDeclaration(const VariableDeclaration& p){
        type_ = p.type_->clone();
        name_ = new string(*p.name_);
    }

    virtual ~VariableDeclaration () {
        if (type_) delete type_;
        delete name_;
    }

    void print (ostream& os) {
        os<<"Node name : VariableDeclaration. Var name is: "<<*name_<<endl;
        assert(type_);
        type_->print(os);
    }
    void pcodegen(ostream& os) {
        is_var_declaration =true;
        assert(type_);
        int size=1;
        int current_size=1;

        if(type_->getType()=="ArrayType"){
            ArraysST.getArraysVector().push_back(ArrayClass(*name_));
            int typeSize = type_->getFactor();
            ArraysST.find(*name_).setInnerType(type_->getInner());
            size = typeSize*type_->getSize();
            ArraysST.find(*name_).setFactor(typeSize);
            ArraysST.find(*name_).setSize(size);
            type_->setArraySizes(*name_);

        }
        if(is_record_type_var){
            // if idetype means it's an array or a record
            //adding a new field to the last record which is the record we are in.
            if(type_->getType() == "IdeType"){
                if(ST.findTypeByName(type_->getName()) == "ArrayType") {
                    RecordField field(*name_, type_->getName(), false, ArraysST.find(type_->getName()).getSize(), true);
                    RecordsST.getRecordsVector()[RecordsST.getRecordsVector().size() - 1].getFieldsInRecord().push_back(
                            field);
                } else {
                    RecordField field(*name_, type_->getName(), false, RecordsST.getRecordByName(type_->getName()).getRecordActualSize(), false);
                    RecordsST.getRecordsVector()[RecordsST.getRecordsVector().size() - 1].getFieldsInRecord().push_back(
                            field);
                }
            } else if(type_->getType() == "ArrayType"){
                RecordField field(*name_, type_->getName(), false, ArraysST.find(*name_).getSize(), true);
                RecordsST.getRecordsVector()[RecordsST.getRecordsVector().size()-1].getFieldsInRecord().push_back(field);
            }else{
                RecordField field(*name_, type_->getName(), true, 1, false);
                RecordsST.getRecordsVector()[RecordsST.getRecordsVector().size()-1].getFieldsInRecord().push_back(field);
            }
        }

        if(type_->getType()=="RecordType"){
            is_record_type_var = true;
            current_size = stacksize;
            Record newRecord;
            RecordsST.getRecordsVector().push_back(newRecord);
        }
        if(type_->getType()=="AddressType"){
            if(type_->getInner() == "ArrayType"){
                got_array_pointer = true;
                array_point_name = *name_;
            }

        }

        type_->pcodegen(os);
        int address = stacksize;
        if(type_->getType()=="RecordType"){
            size = stacksize-current_size;
            address = stacksize -size;
        }
        if(ST.insert(*name_,type_->getType(),address,size,type_->getName())){
            if(type_->getType()!="RecordType"){
                stacksize+=size;

            }else{
                RecordsST.getRecordsVector()[RecordsST.getRecordsVector().size()-1].setName(*name_);
                RecordsST.getRecordsVector()[RecordsST.getRecordsVector().size()-1].setSize(RecordsST.getRecordsVector()[RecordsST.getRecordsVector().size()-1].getRecordActualSize());
                is_record_type_var = false;
            }

        }

        ST.print();
        is_var_declaration =false;
    }
    virtual Object * clone () const { return new VariableDeclaration(*this);}

private:
    Object * type_;
    string * name_;
};

class Parameter : public Object {
public :
    Parameter (Object * type, const char * name) : type_(type){
        assert(type_);
        name_ = new string(name);
    }

    Parameter(const Parameter& p){
        type_ = p.type_->clone();
        name_ = new string(*p.name_);
    }
    string getName(){
        return *name_;
    }

    virtual ~Parameter () {
        if (type_) delete type_;
        delete name_;
    }

    void print (ostream& os) {
        printWayOfPassing(os);
        os<<"Parameter name :" <<name_<<endl;
        assert(type_);
        type_->print(os);
    }
    void pcodegen(ostream& os) {
        printWayOfPassing(os);
        assert(type_);
        type_->pcodegen(os);
    }
protected:
    virtual void printWayOfPassing (ostream& os) const = 0;

private:
    Object * type_;
    string * name_;
};

class ByReferenceParameter : public Parameter {
public :
    ByReferenceParameter (Object * type, const char * name) : Parameter (type,name) {}
    virtual Object * clone () const { return new ByReferenceParameter(*this);}
protected:
    void printWayOfPassing (ostream& os) const{
        os<<"Node name : ByReferenceParameter ";
    }
};

class ByValueParameter : public Parameter {
public :
    ByValueParameter (Object * type, const char * name) : Parameter(type,name) {}
    virtual Object * clone () const { return new ByValueParameter(*this);}
protected:
    void printWayOfPassing (ostream& os) const{
        os<<"Node name : ByValueParameter ";
    }
};

class ParameterList : public Object {
public :
    ParameterList (Object * formal) : formal_(formal),  formal_list_(NULL) { assert(formal_);}
    ParameterList (Object * formal, Object * formal_list) : formal_(formal), formal_list_(formal_list) {assert(formal_ && formal_list_);}

    ParameterList(const ParameterList& pl){
        formal_ = pl.formal_->clone();
        formal_list_ = pl.formal_list_->clone();
    }

    virtual ~ParameterList () {
        if (formal_) delete formal_;
        if (formal_list_) delete formal_list_;
    }

    void print (ostream& os) {
        os<<"Node name : ParameterList"<<endl;
        if (formal_list_){
            formal_list_->print(os);
        }
        assert(formal_);
        formal_->print(os);
    }
    void pcodegen(ostream& os) {
        if (formal_list_) {
            formal_list_->pcodegen(os);
        }
        assert(formal_);
        formal_->pcodegen(os);
    }
    virtual Object * clone () const { return new ParameterList(*this);}

private:
    Object * formal_;
    Object * formal_list_;
};

class FunctionDeclaration : public Declaration {
public :
    FunctionDeclaration (Object * type, Object * block, const char * name) : type_(type), block_(block), formal_list_(NULL) {
        assert(type_ && block_);
        name_ = new string(name);
    }

    FunctionDeclaration (Object * type, Object * formal_list, Object * block,  const char * name) : type_(type), formal_list_(formal_list),block_(block) {
        assert(type_ && formal_list_ && block_ );
        name_ = new string(name);
    }

    virtual ~FunctionDeclaration () {
        if (type_) delete type_;
        if (block_) delete block_;
        if (formal_list_) delete formal_list_;
        if (name_) delete name_;
    }

    FunctionDeclaration(const FunctionDeclaration& fd){
        type_ = fd.type_->clone();
        block_ = fd.block_->clone();
        formal_list_ = fd.formal_list_->clone();
        name_ = new string(*fd.name_);
    }
    string getName(){
        return *name_;
    }

    void print (ostream& os) {
        os<<"Node name : FunctionDeclaration. Func name is: "<<*name_<<endl;
        assert(type_ && block_);
        type_->print(os);
        if (formal_list_){
            formal_list_->print(os);
        }
        block_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(type_ && block_);
        type_->pcodegen(os);
        if (formal_list_) {
            formal_list_->pcodegen(os);
        }
        block_->pcodegen(os);
    }
    virtual Object * clone () const { return new FunctionDeclaration(*this);}

private:
    Object * type_;
    Object * block_;
    Object * formal_list_;
    string * name_;
};

class ProcedureDeclaration : public Declaration {
public :
    ProcedureDeclaration (Object * block, const char * name) : formal_list_(NULL), block_(block) {
        assert(block_);
        name_ = new string(name);
    }

    ProcedureDeclaration (Object * formal_list, Object * block, const char * name) : formal_list_(formal_list),block_(block)  {
        assert(formal_list_ && block_);
        name_ = new string(name);
    }

    virtual ~ProcedureDeclaration () {
        if (block_) delete block_;
        if (formal_list_) delete formal_list_;
        if (name_) delete name_;
    }

    ProcedureDeclaration(const ProcedureDeclaration& pd){
        block_ = pd.block_->clone();
        formal_list_ = pd.formal_list_->clone();
        name_ = new string(*pd.name_);
    }
    string getName(){
        return *name_;
    }
    void print (ostream& os) {
        os<<"Node name : ProcedureDeclaration. Proc name is: "<<*name_<<endl;
        assert(block_);
        if (formal_list_){
            formal_list_->print(os);
        }
        block_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(block_);
        if (formal_list_) {
            formal_list_->pcodegen(os);
        }
        block_->pcodegen(os);
    }
    virtual Object * clone () const { return new ProcedureDeclaration(*this);}

private:
    Object * block_;
    Object * formal_list_;
    string * name_;
};

class DeclarationList : public Object {
public :
    DeclarationList (Object * decl) : decl_(decl), decl_list_(NULL) { assert(decl_);}
    DeclarationList (Object * decl_list, Object * decl) : decl_list_(decl_list),decl_(decl)  {assert(decl_list_ && decl);}

    DeclarationList(const DeclarationList& dl){
        decl_ = dl.decl_->clone();
        decl_list_ = dl.decl_list_->clone();
    }

    virtual ~DeclarationList () {
        if (decl_) delete decl_;
        if (decl_list_) delete decl_list_;
    }

    void print (ostream& os) {
        os<<"Node name : DeclarationList"<<endl;
        if (decl_list_){
            decl_list_->print(os);
        }
        assert(decl_);
        decl_->print(os);
    }
    void pcodegen(ostream& os) {
        if (decl_list_) {
            decl_list_->pcodegen(os);
        }
        assert(decl_);
        decl_->pcodegen(os);
    }
    virtual Object * clone () const { return new DeclarationList(*this);}

private:
    Object * decl_;
    Object * decl_list_;
};

class Block : public Object {
public :
    Block (Object * stat_seq) : stat_seq_(stat_seq), decl_list_(NULL)  { assert(stat_seq_);}
    Block (Object * decl_list, Object * stat_seq) : decl_list_(decl_list), stat_seq_(stat_seq)  {assert(decl_list_ && stat_seq_);}

    Block(const Block& b){
        decl_list_ = b.decl_list_->clone();
        stat_seq_ = b.stat_seq_->clone();
    }

    virtual ~Block () {
        if (stat_seq_) delete stat_seq_;
        if (decl_list_) delete decl_list_;
    }

    void print (ostream& os) {
        os<<"Node name : Begin"<<endl;
        if (decl_list_){
            decl_list_->print(os);
        }
        assert(stat_seq_);
        stat_seq_->print(os);
    }
    void pcodegen(ostream& os) {
        if (decl_list_) {
            decl_list_->pcodegen(os);
        }
        assert(stat_seq_);
        stat_seq_->pcodegen(os);
    }

    virtual Object * clone () const { return new Block(*this);}

private:
    Object * decl_list_;
    Object * stat_seq_;
};

class Program : public Object {
public :
    Program (Object * block, const char * str) : block_(NULL) {
        block_ = dynamic_cast<Block *>(block);
        assert(block_);
        name_ = new string(str);
    }

    Program(const Program& prog){
        block_ = dynamic_cast<Block *>(prog.block_->clone());
        assert(block_);
        name_ = new string(*prog.name_);
    }

    virtual ~Program () {
        if (block_) delete block_;
        delete name_;
    }

    string getName(){
        return *name_;
    }

    void print (ostream& os) {
        os<<"Node name : Root/Program. Program name is: "<<*name_<<endl;
        assert(block_);
        block_->print(os);
    }
    void pcodegen(ostream& os) {
        assert(block_);
        block_->pcodegen(os);
    }
    virtual Object * clone () const { return new Program(*this);}

private:
    Block * block_;
    string * name_;
};


#endif //AST_H