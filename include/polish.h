#ifndef _POLISH_H_
#define _POLISH_H_

#include "stdio.h"
#include "math.h"
#include <stdexcept>
#include "string.h"

struct CNode {
  char *data;
  int flag;
  CNode *next;
};

class polish {
 public :
  CNode * divideLex(char *exp);
  int addNumber(char *str, CNode ** Lex, CNode **last);
  void add(char ch, CNode **Lex, CNode **last);
  void copyInOut(CNode **Out, char *data, CNode **lastOut);
  CNode * convert(char *exp);
  double getVal(char *exp, double *var, int size);
  int getPrt(char op);
  bool isOperation(char op);
  bool isNumber(char num);
  bool isVar(char var);
};

#endif  //  _POLISH_H_