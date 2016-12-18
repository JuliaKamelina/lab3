#include "polish.h"
#include "stack.h"

int polish::getPrt(char op) {
  int prt;
  switch(op) {
    case '(': prt = 0; break;
    case '+': prt = 2; break;
    case '-': prt = 2; break;
    case '_': prt = 2; break;  //  унарный минус
    case '*': prt = 3; break;
    case '/': prt = 3; break;
    case '^': prt = 4; break;
    case '|': prt = 0; break;
  }
  return prt;
}

bool polish::isOperation(char op) {
  if ((op == '+') || (op == '-') || (op == '*') || (op == '/') || (op == '^') || (op == '_'))
    return true;
  else return false;
}

bool polish::isNumber(char num) {
  if ((num >= '0') && (num <= '9') || (num == '.'))
    return true;
  else return false;
}

bool polish::isVar(char var) {
  if ((var >= 'a') && (var <= 'z'))
    return true;
  else return false;
}

int polish::addNumber(char *str, CNode **Lex, CNode **last) {
  int j = 1;
  CNode *node = new CNode;
  
  while (isNumber(str[j])) {
    j++;
  }
  node->data = new char[j];
   
  for(int i = 0; i < j; i++) {
    node->data[i] = str[i];
  }

  node->data[j] = 0;
  node->flag = 0;
  
  if (*Lex == 0) {
    node->next = *Lex;
    *Lex = node;
    *last = node;
  }
  else {
    (*last)->next = node;
    node->next = NULL;
    *last = node;
  }
  return j;
}

void polish::add(char ch, CNode **Lex, CNode **last) {
  CNode *node = new CNode;
  node->data = new char[2];
  
  node->data[0] = ch;
  node->data[1] = 0;
  node->flag = 1;
  
  if (*Lex == 0) {
    node->next = *Lex;
    *Lex = node;
    *last = node;
  }
  else {
    node->next = NULL;
    (*last)->next = node;
    *last = node;
  }
}

CNode * polish::divideLex(char *exp) {
  if (exp == 0)
    throw std::logic_error("empty expression");
  else {
  CNode *Lex = 0;
  CNode *last = 0;
  int stage = 0;
  int countBr = 0, countMod = 0;
  int i = 0;
  
  while(exp[i] != 0) {
    if((isNumber(exp[i])) && (!stage)) {
      i += addNumber(&exp[i], &Lex, &last);
      stage = 1;
    }
  
    else {
      if((isOperation(exp[i])) && (stage)) {
        add(exp[i], &Lex, &last);
        stage = 0;
        i++;
      }
      
      else {
        if ((isVar(exp[i])) && (!stage)) {
          add(exp[i], &Lex, &last);
          last->flag = 3;
          stage = 1;
          i++;
        }
        
        else {
          switch(exp[i]) {
            case ' ': i++; break;
            case '(':
              add(exp[i], &Lex, &last);
              last->flag = 10;
              stage = 0;
              countBr++;
              i++;
              break;
            
            case ')':
              add(exp[i], &Lex, &last);
              last->flag = 11;
              stage = 1;
              countBr--;
              i++;
              break;
            
            case '|':
              if ((i == 0) || (last->flag == 1) || (last->flag == 10)) {
                add(exp[i], &Lex, &last);
                last->flag = 10;  //  открывающийся
                stage = 0;
                countMod++;
              }
              else {
                if ((last->flag == 0) || (last->flag == 3) || (last->flag == 11)) {
                  add(exp[i], &Lex, &last);
                  last->flag = 11;  //  закрывающийся
                  stage = 1;
                  countMod--;
                } else
                  throw std::logic_error("expression is incorrect (||)");
              }
              i++;
              break;
              
            case '-':  //  унарный минус
              if ((i == 0) || (exp[i + 1] == '(') || (exp[i + 1] == '|') || (last->flag == 10)) {
                add('_', &Lex, &last);
                i++;
                stage = 0;
              }
              else {
                if (isVar(exp[i + 1])) {
                  CNode *node = new CNode;
                  node->data = new char[3];
                  
                  node->data[0] = exp[i];
                  node->data[1] = exp[i + 1];
                  node->data[2] = 0;
                  node->flag = 3; 
                  
                  last->next = node;
                  node->next = NULL;
                  last = node;
                 
                  i += 2;
                  stage  = 1;
                }
                
                else
                  throw std::logic_error("expression is incorrect (-)");
              }
              break;
              
            default: throw std::logic_error("expression is incorrect (unknown simbol)");
          }
        }
      }
    }
  }
  if ((countBr != 0) || (countMod != 0))
    throw std::logic_error("expression is incorrect (unpaired brackets)");
  else return Lex;
  }
}

void polish::copyInOut(CNode **Out, char *data, CNode **lastOut) {
  CNode *node = new CNode;
  node->data = new char[strlen(data) + 1];
  int i = 0;
  
  while (data[i] != 0) {
    node->data[i] = data[i];
    i++;
  }
  node->data[i] = 0;
  node->flag = 0;
  if (*Out == 0) {
    node->next = *Out;
    *Out = node;
    *lastOut = node;
  }
  else {
    (*lastOut)->next = node;
    node->next = NULL;
    *lastOut = node;
  }
}

CNode * polish::convert(char *exp) {
  stack<char *> s;
  CNode *polForm = 0;
  CNode *Lex = divideLex(exp);
  CNode *last = 0;
  
  while (Lex != 0 ) {
    if ((Lex->flag == 0) || (isVar(Lex->data[0])))  //  число или переменная
      copyInOut(&polForm, Lex->data, &last);
    
    else {
      if (isOperation(Lex->data[0])) {
        while ((!s.empty()) && (getPrt(Lex->data[0]) <= getPrt(s.getTop()[0]))) {
           copyInOut(&polForm, s.getTop(), &last);
           s.pop();  
          }
        s.push(Lex->data);
      }
    
      else {
        if ((Lex->flag == 10))  //  10 -- открывающийся модуль или скобка
          s.push(Lex->data);
      
        else {
          while ((s.getTop()[0] != '(') && (s.getTop()[0] != '|')) {
           copyInOut(&polForm, s.getTop(), &last);
           s.pop();
          }
          if (s.getTop()[0] == '|')
            copyInOut(&polForm, s.getTop(), &last);
          s.pop();
        }
      }
    }
    Lex = Lex->next;
  }
  
  while (!s.empty()) {
    copyInOut(&polForm, s.getTop(), &last);
    s.pop();
  }
  return polForm;
}

double polish::getVal(char *exp, double *var, int size) {
  stack<double> s;
  double tmp;
  int i = 0;
  CNode *polForm = convert(exp);
  
  while (polForm != 0) {
    if (isNumber(polForm->data[0]) || isNumber(polForm->data[1]))
      s.push(atof(polForm->data));
      
    else {
      if (isVar(polForm->data[0]) || isVar(polForm->data[1])) {
        if (i < size) {
          if (polForm->data[0] == '-')
            var[i] *= -1;
            
          s.push(var[i]);
          i++;
        }
        else throw std::exception("not enough variables");
      }
        
      else {
        switch(polForm->data[0]) {
          case '_':
            tmp = s.getTop();
            tmp *= -1;
            s.pop();
            
            s.push(tmp);
            break;
          
          case '-':
            tmp = s.getTop();
            s.pop();
            tmp = s.getTop() - tmp;
            s.pop();
            
            s.push(tmp);
          break;
          
          case '+':
            tmp = s.getTop();
            s.pop();
            tmp = s.getTop() + tmp;
            s.pop();
            
            s.push(tmp);
            break;
        
          case '*':
            tmp = s.getTop();
            s.pop();
            tmp = s.getTop() * tmp;
            s.pop();
            
            s.push(tmp);
            break;
        
          case '/':
            tmp = s.getTop();
            s.pop();
            tmp = s.getTop()/tmp;
            s.pop();
            
            s.push(tmp);
            break;
        
          case '^':
            tmp = s.getTop();
            s.pop();
            tmp = pow(s.getTop(), tmp);
            s.pop();
          
            s.push(tmp);
            break;

          case '|':
            tmp = s.getTop();
            s.pop();
            
            s.push(abs(tmp));
            break;
        }
      }
    }
    polForm = polForm->next;
  }
  return s.getTop();
}
