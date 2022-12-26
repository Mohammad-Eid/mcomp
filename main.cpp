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

#include "main.h"

extern int yydebug;

bool is_real_const = false;
int loop_counter = 0;
int if_counter =0;
int switch_counter = 0;
int max_switch_count =0;
int cases_counter = 0;
double inc_val = 0 ;
int stacksize = 5;
bool left_flag = false;
bool is_add = false;
bool codel=false;
bool inc_flag=false;
bool is_var = false;
bool is_switch = false;
bool is_if = false;
bool is_assign = false;
bool is_expr =false;
bool is_print = false;
bool is_unary = false;
bool was_expr = false;
bool is_var_assign = false;
bool is_var_loop =false;
bool is_address_type = false;
bool is_new = false;
bool is_address_ref = false;
bool is_intern = false;
int dim_counter = 0;
bool is_dim = false;
string Extern_name;
string ActiveArray;
SymbolTable ST;
ArraysTable ArraysST;
bool is_equal = false;
void writeAST(Object* r, fstream& file)
{
    //Perform recursive tree print
    r->print(file);
}
void Pcode(Object* r, fstream& file)
{
    //Perform recursive tree print
    r->pcodegen(file);
}

Object* getTree(char* progFile)
{

    yydebug = 1;
    assert(progFile);

    // yyin is an external variable that been used in yyparse as pointer to the source file.
    yyin = fopen(progFile, "r");
    if (!yyin) {
        cerr << "Error: file " << progFile << " does not exst. Aborting ..." << endl;;
        exit(1);
    }
    assert(yyin);

    // yyparse is yacc function that parse the program, checks syntax and builds the program AST.
    yyparse();

    fclose(yyin);

    // root was initialized in yyparse while it was building the tree.
    // root is the pointer of the returning tree.
    assert(root);
    return(root);
}



int main(int argc, char* argv[])
{
    CodeGenerator codeGen;

    //check input arguments.
    if (argc < 2) {
        cerr << endl << "Input file is missing. Aborting ..." << endl;
        exit(1);
    }

    Object* theProgram = getTree(argv[1]);
    assert(theProgram == root);

    fstream treeFile(TREE_OUTPUT_TEXT_FILE, ios::out);
    fstream pcodeFile(OUTPUT_CODE_TEXT, ios::out);
    treeFile << AST_FILE_HEADER << endl;
//    pcodeFile << AST_FILE_HEADER << endl;
    writeAST(theProgram, treeFile);
    Pcode(theProgram, pcodeFile);
    treeFile.close();
    return (0);
}


