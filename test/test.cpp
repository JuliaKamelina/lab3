#include <gtest/gtest.h>
#include "polish.h"

TEST(polish, can_divide_into_lexemes)
{
  char *s = "0.5*2+(-2)";
  CNode nodes[8];
  CNode *expLex = 0;
  polish l;
  CNode *lex = 0;
  for (int i = 0; i < 7; i++)
    nodes[i].next = &nodes[i + 1];
  nodes[7].next = NULL;
  
  nodes[0].data = "0.5";
  nodes[0].flag = 0;
  nodes[1].data = "*";
  nodes[1].flag = 1;
  nodes[2].data = "2";
  nodes[2].flag = 0;
  nodes[3].data = "+";
  nodes[3].flag = 1;
  nodes[4].data = "(";
  nodes[4].flag = 10;
  nodes[5].data = "_";
  nodes[5].flag = 1;
  nodes[6].data = "2";
  nodes[6].flag = 0;
  nodes[7].data = ")";
  nodes[7].flag = 11;
  expLex = nodes;
  
  lex = l.divideLex(s);
  
  for (int i = 0; i < 8; i++) {
    for (int j = 0; (expLex->data[j] != 0) || (lex->data[j] != 0); j++)
      EXPECT_EQ(expLex->data[j], lex->data[j]);
    EXPECT_EQ(expLex->flag, lex->flag);
    expLex = expLex->next;
    lex = lex->next;
  }
}

TEST(polish, can_divide_with_abs) {
  char *s = "2 + |3-|-7||";
  CNode nodes[10];
  CNode *expLex = 0;
  polish l;
  CNode *lex = 0;
  for (int i = 0; i < 9; i++)
    nodes[i].next = &nodes[i + 1];
  nodes[9].next = NULL;
  
  nodes[0].data = "2";
  nodes[0].flag = 0;
  nodes[1].data = "+";
  nodes[1].flag = 1;
  nodes[2].flag = 10;
  nodes[2].data = "|";
  nodes[3].flag = 0;
  nodes[3].data = "3";
  nodes[4].data = "-";
  nodes[4].flag = 1;
  nodes[5].flag = 10;
  nodes[5].data = "|";
  nodes[6].flag = 1;
  nodes[6].data = "_";
  nodes[7].data = "7";
  nodes[7].flag = 0;
  nodes[8].data = "|";
  nodes[8].flag = 11;
  nodes[9].data = "|";
  nodes[9].flag = 11;
  expLex = nodes;
  
  lex = l.divideLex(s);
  
  for (int i = 0; i < 10; i++) {
    for (int j = 0; (expLex->data[j] != 0) || (lex->data[j] != 0); j++)
      EXPECT_EQ(expLex->data[j], lex->data[j]);
    EXPECT_EQ(expLex->flag, lex->flag);
    expLex = expLex->next;
    lex = lex->next;
  }
}

TEST(polish, can_divide_with_brackets) {
  char *s = "((-2)^2 + 7)";
  CNode nodes[10];
  CNode *expLex = 0;
  polish l;
  CNode *lex = 0;
  for (int i = 0; i < 9; i++)
    nodes[i].next = &nodes[i + 1];
  nodes[9].next = NULL;
  
  nodes[0].flag = 10;
  nodes[0].data = "(";
  nodes[1].flag = 10;
  nodes[1].data = "(";
  nodes[2].data = "_";
  nodes[2].flag = 1;
  nodes[3].data = "2";
  nodes[3].flag = 0;
  nodes[4].data = ")";
  nodes[4].flag = 11;
  nodes[5].data = "^";
  nodes[5].flag = 1;
  nodes[6].flag = 0;
  nodes[6].data = "2";
  nodes[7].data = "+";
  nodes[7].flag = 1;
  nodes[8].flag = 0;
  nodes[8].data = "7";
  nodes[9].data = ")";
  nodes[9].flag = 11;
  
  expLex = nodes;
  
  lex = l.divideLex(s);
  
  for (int i = 0; i < 10; i++) {
    for (int j = 0; (expLex->data[j] != 0) || (lex->data[j] != 0); j++)
      EXPECT_EQ(expLex->data[j], lex->data[j]);
    EXPECT_EQ(expLex->flag, lex->flag);
    expLex = expLex->next;
    lex = lex->next;
  }
}

TEST(polish, throws_when_incorrect_expr) {
  char *s = "4 + /81";
  polish l;
  
  ASSERT_ANY_THROW(l.divideLex(s));
}

TEST(polish, throws_when_unpaired_brackets) {
  char *s = "|-2)+6.4^2|";
  polish l;
  
  ASSERT_ANY_THROW(l.divideLex(s));
}

TEST(polish, can_convert_with_abs) {
  char *s = "5*6+|(-3.9) + 2|";
  CNode *expPol =0;
  CNode nodes[9];
  for (int i = 0; i < 8; i++) {
    nodes[i].next = &nodes[i+1];
    nodes[i].flag = 0;
  }
    
  nodes[8].next = NULL;
  nodes[8].flag = 0;
  
  nodes[0].data = "5";
  nodes[1].data = "6";
  nodes[2].data = "*";
  nodes[3].data = "3.9";
  nodes[4].data = "_";
  nodes[5].data = "2";
  nodes[6].data = "+";
  nodes[7].data = "|";
  nodes[8].data = "+";
  
  expPol = nodes;
  
  polish l;
  CNode *pol = 0;
  
  pol = l.convert(s);
  
  for (int i = 0; i < 8; i++) {
    for (int j = 0; (expPol->data[j] != 0) || (pol->data[j] != 0); j++)
      EXPECT_EQ(expPol->data[j], pol->data[j]);
    EXPECT_EQ(expPol->flag, pol->flag);
    expPol = expPol->next;
    pol = pol->next;
  }
}

TEST(polish, can_convert_when_starts_with_negative) {
  char *s = "-3^2";
  CNode *expPol = 0;
  CNode nodes[4];
  
  for(int i = 0; i < 3; i++) {
    nodes[i].next = &nodes[i + 1];
    nodes[i].flag = 0;
  }
  nodes[3].next = NULL;
  nodes[3].flag = 0;
  
  nodes[0].data = "3";
  nodes[1].data = "2";
  nodes[2].data = "^";
  nodes[3].data = "_";
  
  expPol = nodes;
  
  polish l;
  CNode *pol = 0;
  
  pol = l.convert(s);
  
  for (int i = 0; i < 4; i++) {
    for (int j = 0; (expPol->data[j] != 0) || (pol->data[j] != 0); j++)
      EXPECT_EQ(expPol->data[j], pol->data[j]);
    EXPECT_EQ(expPol->flag, pol->flag);
    expPol = expPol->next;
    pol = pol->next;
  }
}

TEST(polish, can_convert_to_polish) {
  char *s = "3-5*7";
  CNode *expPol =0;
  CNode nodes[5];
  for (int i = 0; i < 4; i++) {
    nodes[i].next = &nodes[i+1];
    nodes[i].flag = 0;
  }
    
  nodes[4].next = NULL;
  nodes[4].flag = 0;
  
  nodes[0].data = "3";
  nodes[1].data = "5";
  nodes[2].data = "7";
  nodes[3].data = "*";
  nodes[4].data = "-";
    
  expPol = nodes;
  
  polish l;
  CNode *pol = 0;
  
  pol = l.convert(s);
  
  for (int i = 0; i < 5; i++) {
    for (int j = 0; (expPol->data[j] != 0) || (pol->data[j] != 0); j++)
      EXPECT_EQ(expPol->data[j], pol->data[j]);
    EXPECT_EQ(expPol->flag, pol->flag);
    expPol = expPol->next;
    pol = pol->next;
  }
}

TEST(polish, can_convert_with_variables) {
  char *s = "3 * 0.5+x";
    CNode *expPol =0;
  CNode nodes[5];
  for (int i = 0; i < 4; i++) {
    nodes[i].next = &nodes[i+1];
    nodes[i].flag = 0;
  }
    
  nodes[4].next = NULL;
  nodes[4].flag = 0;
  
  nodes[0].data = "3";
  nodes[1].data = "0.5";
  nodes[2].data = "*";
  nodes[3].data = "x";
  nodes[4].data = "+";
  
  expPol = nodes;
  
  polish l;
  CNode *pol = 0;
  
  pol = l.convert(s);
  
  for (int i = 0; i < 5; i++) {
    for (int j = 0; (expPol->data[j] != 0) || (pol->data[j] != 0); j++)
     EXPECT_EQ(expPol->data[j], pol->data[j]);
    EXPECT_EQ(expPol->flag, pol->flag);
    expPol = expPol->next;
    pol = pol->next;
  }
}

TEST(polish, can_get_value) {
  char *s = "3 * 2 -|x|*y";
  double var[] = {2, 4};
  polish l;
  
  EXPECT_EQ(-2, l.getVal(s, var, 2));
}

TEST(polish, can_get_value_with_negative_var) {
  char *s = "3 * 2 +|-x|*y";
  double var[] = {2, 4};
  polish l;
  
  EXPECT_EQ(14, l.getVal(s, var, 2));
}

TEST(polish, can_get_value_without_vars) {
  char *s = "3+ 6/2-0.3";
  polish l;
  
  EXPECT_EQ(5.7, l.getVal(s, 0, 0));
}

TEST(polish, throws_when_not_enought_vars) {
  char *s = "3 * 2 +|-x|*y-z";
  double var[] = {2, 4};
  polish l;
  
  ASSERT_ANY_THROW(l.getVal(s, var, 2));
}

TEST(polish, throws_when_empty_expression) {
  char *s = 0;
  polish l;
  
  ASSERT_ANY_THROW(l.getVal(s, 0, 0));
}

TEST(polish, throws_when_brakets_abs) {
  char *s = "|()|";
  polish l;

  ASSERT_ANY_THROW(l.getVal(s, 0, 0));
}

TEST(polish, can_count_with_abs) {
  char *s = "||-3 + 2|+|-3||";
  polish l;
  
  EXPECT_EQ(4, l.getVal(s, 0, 0));
}

TEST(polish, throws_when_dot_and_letter) {
  char *s = "0.a";
  polish l;
  
  ASSERT_ANY_THROW(l.convert(s));
}

TEST(polish, can_calculate_variable) {
  char *s = "a";
  polish l;
  double var[] = {4}; 
  
  EXPECT_EQ(4, l.getVal(s, var, 1));
}

TEST(polish, works_when_negative_degree) {
  char *s = "3+4^(-0.5)";
  polish l;
  
  EXPECT_EQ(l.getVal(s, 0, 0), 3.5);
}

TEST(polish, abs_and_degrees) {
  char * s = "||-3^2+|-5|^3+|-5^2||-40|";
  polish l;
  
  EXPECT_EQ(101, l.getVal(s, 0, 0));
}
  

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}